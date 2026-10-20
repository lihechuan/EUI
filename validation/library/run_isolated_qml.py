import ctypes
import os
import subprocess
import sys
import time
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
user32.CloseDesktop.argtypes = [wintypes.HANDLE]
kernel32.CreateProcessW.argtypes = [wintypes.LPCWSTR, wintypes.LPWSTR, ctypes.c_void_p, ctypes.c_void_p,
                                   wintypes.BOOL, wintypes.DWORD, ctypes.c_void_p, wintypes.LPCWSTR,
                                   ctypes.POINTER(StartupInfo), ctypes.POINTER(ProcessInfo)]
kernel32.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
kernel32.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD)]
kernel32.TerminateProcess.argtypes = [wintypes.HANDLE, wintypes.UINT]
kernel32.CloseHandle.argtypes = [wintypes.HANDLE]

runner, suite, imports = map(Path, sys.argv[1:4])
extra = sys.argv[4:]
root = suite.resolve().parent
log = Path(tempfile.gettempdir()) / ('eui-rendering-' + str(os.getpid()) + '.log')
foreground = user32.GetForegroundWindow()
desktop_name = "EUIRendering" + str(os.getpid())
desktop = user32.CreateDesktopW(desktop_name, None, None, 0, 0x01FF, None)
if not desktop:
    raise ctypes.WinError(ctypes.get_last_error())
process = ProcessInfo()
try:
    os.environ.setdefault("QT_QUICK_BACKEND", "software")
    os.environ.update(QT_QPA_PLATFORM="windows",
                      QT_QPA_FONTDIR=str(Path(os.environ["WINDIR"]) / "Fonts"), QT_FORCE_STDERR_LOGGING="1")
    os.environ["PATH"] = str(runner.resolve().parent) + os.pathsep + os.environ["PATH"]
    startup = StartupInfo()
    startup.cb = ctypes.sizeof(startup)
    startup.dwFlags = 1
    startup.wShowWindow = 4
    startup.lpDesktop = "WinSta0\\" + desktop_name
    command = subprocess.list2cmdline([
        str(runner.resolve()),
        "-input", str(suite.resolve()),
        "-import", str(imports.resolve()), "-platform", "windows",
        "-o", str(log) + ",txt", *extra,
    ])
    if not kernel32.CreateProcessW(None, ctypes.create_unicode_buffer(command), None, None, False,
                                   0, None, str(root), ctypes.byref(startup), ctypes.byref(process)):
        raise ctypes.WinError(ctypes.get_last_error())
    native = None
    if os.environ.get("EUI_NATIVE_PICKER_TESTS") == "1":
        sys.path.insert(0, str(Path(__file__).resolve().parents[2] / "tests"))
        from native_picker_driver import NativePickerDriver
        native = NativePickerDriver(desktop, process.dwProcessId)
    timeout = float(os.environ.get("EUI_QML_TIMEOUT", "300"))
    deadline = time.monotonic() + timeout
    while kernel32.WaitForSingleObject(process.hProcess, 100) != 0 and time.monotonic() < deadline:
        if native:
            native.poll()
    if kernel32.WaitForSingleObject(process.hProcess, 0) != 0:
        kernel32.TerminateProcess(process.hProcess, 1)
        kernel32.WaitForSingleObject(process.hProcess, 5000)
        if log.exists():
            print(log.read_text(encoding="utf-8", errors="replace"), flush=True)
        raise TimeoutError(f"Isolated-desktop visual test exceeded {timeout:g} seconds")
    code = wintypes.DWORD()
    kernel32.GetExitCodeProcess(process.hProcess, ctypes.byref(code))
    print(log.read_text(encoding="utf-8"))
    print("User foreground unchanged:", foreground == user32.GetForegroundWindow())
    if native and code.value == 0:
        native.verify()
    sys.exit(code.value)
finally:
    if process.hProcess and kernel32.WaitForSingleObject(process.hProcess, 0) != 0:
        kernel32.TerminateProcess(process.hProcess, 1)
        kernel32.WaitForSingleObject(process.hProcess, 5000)
    if process.hThread:
        kernel32.CloseHandle(process.hThread)
    if process.hProcess:
        kernel32.CloseHandle(process.hProcess)
    user32.CloseDesktop(desktop)
    log.unlink(missing_ok=True)
