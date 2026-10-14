import argparse
import hashlib
import json
import os
import platform
import runpy
import shutil
import signal
import subprocess
import sys
import tempfile
from pathlib import Path


def run(*arguments, **options):
    process = subprocess.Popen([str(value) for value in arguments], start_new_session=True, **options)
    previous = signal.signal(signal.SIGTERM, lambda *_: os.killpg(process.pid, signal.SIGTERM))
    try:
        if process.wait() != 0:
            raise RuntimeError('Command failed: ' + str(arguments[0]))
    finally:
        signal.signal(signal.SIGTERM, previous)


def runtime_manifest(directory, identity, architecture, qt_version):
    files = []
    for path in sorted(directory.rglob('*')):
        relative = path.relative_to(directory).as_posix()
        if path.is_symlink():
            target = path.resolve(strict=True)
            if not target.is_relative_to(directory.resolve() / 'EUI Gallery.app'):
                raise ValueError('Runtime symlink leaves the app bundle: ' + relative)
            link = os.readlink(path)
            if os.path.isabs(link):
                raise ValueError('Runtime symlink must be relative: ' + relative)
            files.append(dict(path=relative, size=0, sha256=hashlib.sha256(b'').hexdigest(), link=link))
        elif path.is_file():
            digest = hashlib.sha256()
            with path.open('rb') as stream:
                for block in iter(lambda: stream.read(1024 * 1024), b''):
                    digest.update(block)
            files.append(dict(path=relative, size=path.stat().st_size, sha256=digest.hexdigest(),
                              executable=os.access(path, os.X_OK)))
    return dict(schema=1, platform='macos', architecture=architecture, qtVersion=qt_version,
                entry='EUI Gallery.app/Contents/MacOS/example', files=files, **identity)


def main():
    parser = argparse.ArgumentParser(description='Prepare the Qt 6.8 Gallery runtime for macOS.')
    parser.add_argument('--qt-dir', type=Path)
    parser.add_argument('--install-qt', action='store_true')
    parser.add_argument('--gallery-version')
    parser.add_argument('--gallery-revision')
    arguments = parser.parse_args()
    architecture = {'x86_64': 'x64', 'arm64': 'arm64'}.get(platform.machine())
    if platform.system() != 'Darwin' or architecture is None:
        parser.error('Requires macOS x64 or macOS arm64.')
    root = Path(__file__).resolve().parents[1]
    ninja = shutil.which('ninja') or shutil.which('ninja', path=str(Path.home() / 'qt/Tools/Ninja'))
    cmake = shutil.which('cmake')
    if not cmake or not ninja:
        raise ValueError('CMake and Ninja are required to prepare a source build.')
    qt = arguments.qt_dir or root / 'build/quick3d-sdk/6.8.3/macos'
    required = ['bin/qmake'] + ['lib/cmake/Qt6' + module + '/Qt6' + module + 'Config.cmake'
                               for module in ('Quick3D', 'QuickTimeline', 'WebEngineQuick')]
    if not all((qt / path).is_file() for path in required):
        if not arguments.install_qt:
            parser.error('Specify --qt-dir or --install-qt.')
        if arguments.qt_dir:
            parser.error('The specified Qt SDK is incomplete.')
        tools = root / 'build/quick3d-tools'
        python = tools / 'bin/python'
        if not python.is_file():
            run(sys.executable, '-m', 'venv', tools)
        run(python, '-m', 'pip', 'install', 'aqtinstall==3.3.0', cwd=root / 'build')
        run(python, '-m', 'aqt', 'install-qt', 'mac', 'desktop', '6.8.3', 'clang_64',
            '-O', root / 'build/quick3d-sdk', '-m', 'qtwebengine', 'qtwebchannel', 'qtpositioning',
            'qt5compat', 'qtshadertools', 'qtmultimedia', 'qtquick3d', 'qtquicktimeline', cwd=root / 'build')
    qt = qt.resolve()
    version = subprocess.check_output([str(qt / 'bin/qmake'), '-query', 'QT_VERSION'], text=True).strip()
    if not ((6, 8) <= tuple(map(int, version.split('.')[:2])) < (7, 0)):
        raise ValueError('Requires Qt 6.8 or later in the Qt 6 series.')
    build = root / 'build/quick3d-gallery-runtime'
    binary = build / 'bin'
    run(cmake, '-S', root, '-B', build, '-G', 'Ninja', '-DCMAKE_BUILD_TYPE=Release',
        '-DCMAKE_PREFIX_PATH=' + str(qt), '-DQT_DIR=' + str(qt / 'lib/cmake/Qt6'),
        '-DCMAKE_RUNTIME_OUTPUT_DIRECTORY=' + str(binary), '-DCMAKE_MAKE_PROGRAM=' + ninja,
        '-DCMAKE_OSX_ARCHITECTURES=' + platform.machine(), '-DCMAKE_OSX_DEPLOYMENT_TARGET=12.0',
        '-DEUI_BUILD_EXAMPLES=ON', '-DEUI_BUILD_TESTS=OFF', '-DQML_PLUGIN_DUMP:FILEPATH=OFF')
    cache = (build / 'CMakeCache.txt').read_text()
    for component in ('Quick3D', 'QuickTimeline'):
        if not any(line.startswith('Qt6' + component + '_DIR:PATH=') and 'NOTFOUND' not in line
                   for line in cache.splitlines()):
            raise ValueError('Missing Qt module: ' + component)
    run(cmake, '--build', build, '--target', 'example', '--parallel', '4')
    identity = json.loads((build / 'gallery-build.json').read_text())
    for field, expected in [('galleryVersion', arguments.gallery_version), ('galleryRevision', arguments.gallery_revision)]:
        if expected and identity[field] != expected:
            raise ValueError('Gallery sources changed; rebuild the main Gallery before installing its runtime.')
    output = root / 'build/quick3d-runtime'
    if output.is_symlink():
        raise ValueError('Runtime output must not be a symlink.')
    if output.exists() and not (output / 'manifest.json').is_file():
        raise ValueError('Refusing to replace a directory without a runtime manifest.')
    if output.exists():
        previous = json.loads((output / 'manifest.json').read_text())
        if (previous.get('schema'), previous.get('platform'), previous.get('entry')) != (
                1, 'macos', 'EUI Gallery.app/Contents/MacOS/example'):
            raise ValueError('Only an existing macOS Gallery runtime can be replaced.')
    with tempfile.TemporaryDirectory(prefix='quick3d-stage-', dir=root / 'build') as directory:
        stage = Path(directory)
        app = stage / 'EUI Gallery.app'
        shutil.copytree(binary / 'example.app', app, symlinks=True)
        runpy.run_path(str(root / 'packaging/package-macos.py'))['deploy_macos'](root, app, qt, run=run)
        shutil.rmtree(stage / 'qml')
        run('lipo', app / 'Contents/MacOS/example', '-verify_arch', platform.machine())
        manifest = runtime_manifest(stage, identity, architecture, version)
        (stage / 'manifest.json').write_text(json.dumps(manifest, indent=2) + '\n', encoding='utf-8')
        if output.exists():
            shutil.rmtree(output)
        stage.rename(output)
    print('Qt 6 runtime prepared:', output, flush=True)


if __name__ == '__main__':
    main()
