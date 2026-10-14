import ctypes
import json
import os
import argparse
import msvcrt
import re
import subprocess
import sys
import tempfile
from ctypes import wintypes
from pathlib import Path


class StartupInfo(ctypes.Structure):
    _fields_ = [
        ("cb", wintypes.DWORD), ("lpReserved", wintypes.LPWSTR), ("lpDesktop", wintypes.LPWSTR),
        ("lpTitle", wintypes.LPWSTR), ("dwX", wintypes.DWORD), ("dwY", wintypes.DWORD),
        ("dwXSize", wintypes.DWORD), ("dwYSize", wintypes.DWORD),
        ("dwXCountChars", wintypes.DWORD), ("dwYCountChars", wintypes.DWORD),
        ("dwFillAttribute", wintypes.DWORD), ("dwFlags", wintypes.DWORD),
        ("wShowWindow", wintypes.WORD), ("cbReserved2", wintypes.WORD),
        ("lpReserved2", ctypes.POINTER(ctypes.c_byte)),
        ("hStdInput", wintypes.HANDLE), ("hStdOutput", wintypes.HANDLE), ("hStdError", wintypes.HANDLE),
    ]


class ProcessInfo(ctypes.Structure):
    _fields_ = [("hProcess", wintypes.HANDLE), ("hThread", wintypes.HANDLE),
                ("dwProcessId", wintypes.DWORD), ("dwThreadId", wintypes.DWORD)]


user32 = ctypes.WinDLL("user32", use_last_error=True)
kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
user32.CreateDesktopW.argtypes = [wintypes.LPCWSTR, wintypes.LPCWSTR, ctypes.c_void_p,
                                  wintypes.DWORD, wintypes.DWORD, ctypes.c_void_p]
user32.CreateDesktopW.restype = wintypes.HANDLE
user32.GetForegroundWindow.restype = wintypes.HWND
user32.GetWindowThreadProcessId.argtypes = [wintypes.HWND, ctypes.POINTER(wintypes.DWORD)]
user32.CloseDesktop.argtypes = [wintypes.HANDLE]
kernel32.CreateProcessW.argtypes = [wintypes.LPCWSTR, wintypes.LPWSTR, ctypes.c_void_p, ctypes.c_void_p,
                                   wintypes.BOOL, wintypes.DWORD, ctypes.c_void_p, wintypes.LPCWSTR,
                                   ctypes.POINTER(StartupInfo), ctypes.POINTER(ProcessInfo)]
kernel32.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
kernel32.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD)]
kernel32.TerminateProcess.argtypes = [wintypes.HANDLE, wintypes.UINT]
kernel32.CloseHandle.argtypes = [wintypes.HANDLE]


parser = argparse.ArgumentParser(description="Verify Qt 5 to Qt 6 3D handoff on an isolated Windows desktop.")
parser.add_argument("executable", type=Path)
parser.add_argument("--qt-bin", type=Path, help="Qt 5 SDK bin, only for an unbundled development executable")
parser.add_argument("--expect-stage", choices=("installing", "verifying", "cached"),
                    help="Assert first installation, legacy verification, or direct cached startup")
arguments = parser.parse_args()
exe = arguments.executable.resolve()
environment = {key: value for key, value in os.environ.items()
               if not key.startswith(("QT_", "QML", "QSG_", "QTWEBENGINE_"))}
environment.update(PATH=str(Path(environment["WINDIR"]) / "System32"),
                   QT_FORCE_STDERR_LOGGING="1", QT_QPA_PLATFORM="windows")
if arguments.qt_bin:
    environment["PATH"] = str(arguments.qt_bin.resolve()) + os.pathsep + environment["PATH"]
desktop_name = "EUI3DHandoff" + str(os.getpid())
desktop = user32.CreateDesktopW(desktop_name, None, None, 0, 0x01FF, None)
if not desktop:
    raise ctypes.WinError(ctypes.get_last_error())
process = ProcessInfo()
try:
    with tempfile.TemporaryFile() as stderr:
        handle = msvcrt.get_osfhandle(stderr.fileno())
        os.set_handle_inheritable(handle, True)
        startup = StartupInfo()
        startup.cb = ctypes.sizeof(startup)
        startup.lpDesktop = "WinSta0\\" + desktop_name
        startup.dwFlags = 0x100
        startup.hStdOutput = handle
        startup.hStdError = handle
        command = subprocess.list2cmdline([str(exe), "--quick3d", "--quick3d-runtime-check"])
        block = ctypes.create_unicode_buffer("\0".join(f"{key}={value}" for key, value in sorted(environment.items())) + "\0\0")
        if not kernel32.CreateProcessW(str(exe), ctypes.create_unicode_buffer(command), None, None, True,
                                      0x08000400, block, str(exe.parent), ctypes.byref(startup), ctypes.byref(process)):
            raise ctypes.WinError(ctypes.get_last_error())
        if kernel32.WaitForSingleObject(process.hProcess, 60000) != 0:
            kernel32.TerminateProcess(process.hProcess, 1)
            raise TimeoutError("3D installation/handoff exceeded 60 seconds")
        code = wintypes.DWORD()
        kernel32.GetExitCodeProcess(process.hProcess, ctypes.byref(code))
        stderr.seek(0)
        log = stderr.read().decode("utf-8", errors="replace")
        stages = re.findall(r"QUICK3D_RUNTIME (\w+) ([0-9.]+)[ \t]*([^\r\n]*)", log)
        assert code.value == 0, f"Gallery exited with {code.value}\n{log[-8000:]}"
        assert stages and stages[-1][:2] == ("ready", "1.000"), f"3D renderer did not acknowledge readiness:\n{log[-8000:]}"
        names = list(dict.fromkeys(stage for stage, _, _ in stages))
        if arguments.expect_stage == "cached":
            assert "installing" not in names and "verifying" not in names, names
        elif arguments.expect_stage:
            assert arguments.expect_stage in names, names
        assert "launching" in names, names
        galleries = [json.loads(report) for report in re.findall(r"QUICK3D_GALLERY (\{[^\r\n]+\})", log)]
        assert len(galleries) == 2, f"Missing current Gallery identity after 3D handoff:\n{log[-8000:]}"
        assert galleries[0] == galleries[1], f"3D handoff changed Gallery version, components or dock: {galleries}"
        assert galleries[1]["footer"] == "EUI " + galleries[1]["version"], galleries[1]
        assert galleries[1]["update"], "The dock update button disappeared after 3D handoff"
        for error in ("failed to load", "is not installed", "Cannot load library", "TypeError", "ReferenceError"):
            assert error not in log, log[-8000:]
        foreground_pid = wintypes.DWORD()
        user32.GetWindowThreadProcessId(user32.GetForegroundWindow(), ctypes.byref(foreground_pid))
        child_pids = {int(pid) for pid in re.findall(r"QUICK3D_CHILD (\d+)", log)}
        assert child_pids, "Missing successor process identity"
        assert foreground_pid.value not in child_pids | {process.dwProcessId}, "A test Gallery took foreground focus"
        print("PASS: " + " -> ".join(names) + "; Qt 6 confirmed a rendered frame; Qt 5 exited after handoff")
        print(f"Gallery {galleries[1]['version']}: {len(galleries[1]['components'])} entries preserved; footer and dock update button verified")
        print("No test Gallery took foreground focus; child uses bundled runtime with no Qt SDK PATH")
finally:
    if process.hThread:
        kernel32.CloseHandle(process.hThread)
    if process.hProcess:
        kernel32.CloseHandle(process.hProcess)
    user32.CloseDesktop(desktop)
