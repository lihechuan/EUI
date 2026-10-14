import argparse
import ctypes
import hashlib
import json
import msvcrt
import os
import struct
import subprocess
import sys
import tempfile
import zipfile
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


parser = argparse.ArgumentParser(description='Verify Windows runtime files, architecture, and background startup.')
parser.add_argument('package', nargs='?', type=Path)
parser.add_argument('--arch', choices=('x86', 'x64'), default='x64')
parser.add_argument('--archive', type=Path, help='Portable ZIP to compare with the runtime directory')
arguments = parser.parse_args()
root = Path(__file__).resolve().parents[1]
package = arguments.package.resolve() if arguments.package else root / "dist" / "EUI-Gallery"
required = [
    "EUI-Gallery.exe", "EUI/qmldir", "EUI/euiplugin.dll",
    "Qt5Core.dll", "Qt5Quick.dll", "Qt5WebEngineCore.dll", "QtWebEngineProcess.exe",
    "Qt5Multimedia.dll", "Qt5Svg.dll", "opengl32sw.dll", "platforms/qwindows.dll",
    "imageformats/qsvg.dll", "QtQuick.2/qmldir", "QtQuick/Controls.2/qmldir",
    "QtWebEngine/qmldir", "QtMultimedia/qmldir", "qt.conf", "README.txt",
    "resources/icudtl.dat", "resources/qtwebengine_resources.pak",
    "translations/qtwebengine_locales/en-US.pak", "vcruntime140.dll", "msvcp140.dll",
    "licenses/OpenSSL-LICENSE.txt", "licenses/Qt-LGPL-3.0.txt",
    "licenses/EUI-LICENSE.txt", "licenses/THIRD_PARTY_NOTICES.md",
    "licenses/ECharts-LICENSE.txt", "licenses/ECharts-NOTICE.txt",
    "licenses/QR-Code-generator-LICENSE.txt", "licenses/Skill-Icons-LICENSE.txt",
    "quick3d-runtime/manifest.json", "quick3d-runtime/EUI-Gallery-Qt6.exe",
    "quick3d-runtime/Qt6Core.dll", "quick3d-runtime/Qt6Quick3D.dll",
    "quick3d-runtime/Qt6QuickTimeline.dll", "quick3d-runtime/platforms/qwindows.dll",
    "quick3d-runtime/qml/QtQuick3D/qmldir", "quick3d-runtime/qml/QtQuick/Timeline/qmldir",
    "quick3d-runtime/qt.conf", "quick3d-runtime/licenses/LittlestTokyo-ATTRIBUTION.md",
]
suffix = '-x64' if arguments.arch == 'x64' else ''
required += [f'libssl-1_1{suffix}.dll', f'libcrypto-1_1{suffix}.dll']
if arguments.arch == 'x64':
    required.append('vcruntime140_1.dll')
for name in required:
    assert (package / name).is_file(), f"Missing runtime file: {name}"
assert not list(package.rglob("*.pdb")), "Debug symbols should not be distributed"
assert not list(package.rglob("*.log")), "Build/test logs should not be distributed"

exe = package / "EUI-Gallery.exe"
binary = exe.read_bytes()
pe = struct.unpack_from("<I", binary, 0x3C)[0]
assert binary[pe:pe + 4] == b"PE\0\0", "Invalid executable"
machine = 0x8664 if arguments.arch == 'x64' else 0x14c
assert struct.unpack_from("<H", binary, pe + 4)[0] == machine, f"Expected {arguments.arch} executable"
assert struct.unpack_from("<H", binary, pe + 24 + 68)[0] == 2, "Expected Windows GUI subsystem"
for path in list(package.rglob('*.dll')) + list(package.rglob('*.exe')):
    content = path.read_bytes()
    offset = struct.unpack_from('<I', content, 0x3c)[0]
    assert content[offset:offset + 4] == b'PE\0\0', f'Invalid runtime DLL: {path.name}'
    expected = 0x8664 if path.relative_to(package).parts[0] == 'quick3d-runtime' else machine
    assert struct.unpack_from('<H', content, offset + 4)[0] == expected, f'Wrong runtime architecture: {path}'

manifest = json.loads((package / 'quick3d-runtime/manifest.json').read_text(encoding='utf-8'))
assert manifest['schema'] == 1 and manifest['architecture'] == 'x64', 'Invalid Qt 6 runtime manifest'
for record in manifest['files']:
    path = package / 'quick3d-runtime' / record['path']
    assert path.resolve().is_relative_to((package / 'quick3d-runtime').resolve()), 'Unsafe runtime path'
    assert path.stat().st_size == record['size'], f'Runtime size mismatch: {path}'
    with path.open('rb') as stream:
        assert hashlib.file_digest(stream, 'sha256').hexdigest() == record['sha256'], f'Runtime checksum mismatch: {path}'
    required.append('quick3d-runtime/' + record['path'])

archives = [arguments.archive.resolve()] if arguments.archive else list(
    (root / "dist").glob(f"EUI-Gallery-*-windows-{arguments.arch}-portable.zip"))
assert archives, "Portable ZIP was not generated"
archive = max(archives, key=lambda path: path.stat().st_mtime)
with zipfile.ZipFile(archive) as zipped:
    assert zipped.testzip() is None, "Portable ZIP checksum failure"
    for name in required:
        assert "EUI-Gallery/" + name in zipped.namelist(), f"Missing ZIP entry: {name}"
        assert zipped.read("EUI-Gallery/" + name) == (package / name).read_bytes(), f"Package file differs from ZIP: {name}"

environment = dict(os.environ)
for name in list(environment):
    if name != "QT_OPENGL_DLL" and name.startswith(("QT_", "QML", "QTWEBENGINE_")):
        del environment[name]
environment.update(
    PATH=str(Path(environment["WINDIR"]) / "System32"),
    QT_QPA_PLATFORM="windows", QT_FORCE_STDERR_LOGGING="1",
)
user32 = ctypes.WinDLL("user32", use_last_error=True)
kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)
user32.CreateDesktopW.argtypes = [wintypes.LPCWSTR, wintypes.LPCWSTR, ctypes.c_void_p,
                                  wintypes.DWORD, wintypes.DWORD, ctypes.c_void_p]
user32.CreateDesktopW.restype = wintypes.HANDLE
user32.CloseDesktop.argtypes = [wintypes.HANDLE]
user32.GetForegroundWindow.restype = wintypes.HWND
kernel32.CreateProcessW.argtypes = [wintypes.LPCWSTR, wintypes.LPWSTR, ctypes.c_void_p, ctypes.c_void_p,
                                   wintypes.BOOL, wintypes.DWORD, ctypes.c_void_p, wintypes.LPCWSTR,
                                   ctypes.POINTER(StartupInfo), ctypes.POINTER(ProcessInfo)]
kernel32.WaitForSingleObject.argtypes = [wintypes.HANDLE, wintypes.DWORD]
kernel32.GetExitCodeProcess.argtypes = [wintypes.HANDLE, ctypes.POINTER(wintypes.DWORD)]
kernel32.TerminateProcess.argtypes = [wintypes.HANDLE, wintypes.UINT]
kernel32.CloseHandle.argtypes = [wintypes.HANDLE]
callback_type = ctypes.WINFUNCTYPE(wintypes.BOOL, wintypes.HWND, wintypes.LPARAM)
user32.EnumDesktopWindows.argtypes = [wintypes.HANDLE, callback_type, wintypes.LPARAM]
user32.GetWindowThreadProcessId.argtypes = [wintypes.HWND, ctypes.POINTER(wintypes.DWORD)]
user32.IsWindowVisible.argtypes = [wintypes.HWND]
desktop_name = "EUIPackageCheck" + str(os.getpid())
desktop = user32.CreateDesktopW(desktop_name, None, None, 0, 0x01FF, None)
if not desktop:
    raise ctypes.WinError(ctypes.get_last_error())
process = ProcessInfo()
try:
    with tempfile.TemporaryDirectory(prefix="eui-package-check-") as workdir, tempfile.TemporaryFile() as stderr:
        log_handle = msvcrt.get_osfhandle(stderr.fileno())
        os.set_handle_inheritable(log_handle, True)
        startup = StartupInfo()
        startup.cb = ctypes.sizeof(startup)
        startup.lpDesktop = "WinSta0\\" + desktop_name
        startup.dwFlags = 0x100
        startup.hStdOutput = log_handle
        startup.hStdError = log_handle
        env_block = ctypes.create_unicode_buffer("\0".join(f"{key}={value}" for key, value in sorted(environment.items())) + "\0\0")
        if not kernel32.CreateProcessW(str(exe), ctypes.create_unicode_buffer(f'"{exe}"'), None, None,
                                      True, 0x08000400, env_block, workdir,
                                      ctypes.byref(startup), ctypes.byref(process)):
            raise ctypes.WinError(ctypes.get_last_error())
        try:
            result = kernel32.WaitForSingleObject(process.hProcess, 8000)
            if result != 258:
                code = wintypes.DWORD()
                kernel32.GetExitCodeProcess(process.hProcess, ctypes.byref(code))
                stderr.seek(0)
                raise AssertionError(f"Packaged app exited: {code.value}\n{stderr.read().decode(errors='replace')}")
            windows = []

            @callback_type
            def record_window(hwnd, _):
                pid = wintypes.DWORD()
                user32.GetWindowThreadProcessId(hwnd, ctypes.byref(pid))
                if pid.value == process.dwProcessId and user32.IsWindowVisible(hwnd):
                    windows.append(hwnd)
                return True

            user32.EnumDesktopWindows(desktop, record_window, 0)
            assert windows, "Packaged app did not create a visible window on the isolated desktop"
            foreground_pid = wintypes.DWORD()
            user32.GetWindowThreadProcessId(user32.GetForegroundWindow(), ctypes.byref(foreground_pid))
            assert foreground_pid.value != process.dwProcessId, "The packaged app took foreground focus"
        finally:
            kernel32.TerminateProcess(process.hProcess, 0)
            kernel32.WaitForSingleObject(process.hProcess, 5000)
        stderr.seek(0)
        log = stderr.read().decode("utf-8", errors="replace")
        for error in ("failed to load", "is not installed", "Cannot load library", "TypeError", "ReferenceError"):
            assert error not in log, log
finally:
    if process.hThread:
        kernel32.CloseHandle(process.hThread)
    if process.hProcess:
        kernel32.CloseHandle(process.hProcess)
    user32.CloseDesktop(desktop)
print(f"PASS: {arguments.arch} GUI executable, runtime files, ZIP integrity, and a visible window with no developer PATH")
subprocess.run([sys.executable, root / 'packaging/check_quick3d_handoff.py', exe], check=True)
