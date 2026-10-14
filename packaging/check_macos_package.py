import argparse
import os
import platform
import re
import subprocess
import tempfile
import time
import zipfile
from pathlib import Path

from github_release import read_version


def startup(executable, environment, directory):
    with tempfile.TemporaryFile() as output:
        process = subprocess.Popen([str(executable)], cwd=directory, env=environment,
                                   stdout=output, stderr=subprocess.STDOUT)
        try:
            time.sleep(15)
            if process.poll() is not None:
                raise ValueError(f'Gallery exited during startup: {process.returncode}')
        finally:
            if process.poll() is None:
                process.terminate()
                try:
                    process.wait(timeout=10)
                except subprocess.TimeoutExpired:
                    process.kill()
                    process.wait()
            output.seek(0)
            log = output.read().decode('utf-8', errors='replace')
            print(log)
        for error in ('failed to load component', 'is not installed', 'is unavailable', 'is not a type',
                      'cannot assign to non-existent property',
                      'cannot load library', 'error compiling shader', 'failed to deserialize'):
            if error in log.lower():
                raise ValueError('Packaged Gallery startup error: ' + error)
        if re.search(r'Type \S+ unavailable|TypeError:|ReferenceError:', log):
            raise ValueError('Packaged Gallery contains a QML runtime error.')


def main():
    parser = argparse.ArgumentParser(description='Check macOS Gallery packages without showing a window.')
    parser.parse_args()
    if platform.system() != 'Darwin' or platform.machine() not in ('x86_64', 'arm64'):
        parser.error('Requires macOS x64 or macOS arm64.')
    root = Path(__file__).resolve().parents[1]
    version = read_version((root / 'CMakeLists.txt').read_text(encoding='utf-8'))
    environment = {key: value for key, value in os.environ.items()
                   if not key.startswith(('QT_', 'QML', 'DYLD_', 'LD_LIBRARY_PATH'))}
    environment.update({'QT_QPA_PLATFORM': 'offscreen', 'QT_QUICK_BACKEND': 'software',
                        'QT_QUICK_CONTROLS_STYLE': 'Basic',
                        'QTWEBENGINE_CHROMIUM_FLAGS': '--disable-gpu', 'QT_FORCE_STDERR_LOGGING': '1',
                        'PATH': '/usr/bin:/bin:/usr/sbin:/sbin'})
    with tempfile.TemporaryDirectory(prefix='eui-package-check-') as directory:
        destination = Path(directory)
        arch = 'arm64' if platform.machine() == 'arm64' else 'x64'
        base = root / 'dist' / f'EUI-Gallery-{version}-macos-{arch}'
        with zipfile.ZipFile(str(base) + '-portable.zip') as archive:
            if archive.testzip() is not None:
                raise ValueError('Corrupt portable ZIP.')
        subprocess.run(['ditto', '-x', '-k', str(base) + '-portable.zip', directory], check=True)
        subprocess.run(['hdiutil', 'verify', str(base) + '.dmg'], check=True)
        app = destination / 'EUI Gallery.app'
        for relative in ('Contents/Resources/qml/EUI/qmldir',
                         'Contents/PlugIns/platforms/libqcocoa.dylib',
                         'Contents/PlugIns/platforms/libqoffscreen.dylib',
                         'Contents/PlugIns/imageformats/libqsvg.dylib'):
            if not (app / relative).is_file():
                raise ValueError('Missing packaged runtime file: ' + relative)
        if (app / 'Contents/MacOS/EUI').exists():
            raise ValueError('QML resources must be outside the executable directory.')
        subprocess.run(['codesign', '--verify', '--deep', '--strict', str(app)], check=True)
        startup(app / 'Contents/MacOS/example', environment, destination)
    print('Installer and portable package verification passed.')


if __name__ == '__main__':
    main()
