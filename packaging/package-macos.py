import argparse
import hashlib
import os
import platform
import re
import shutil
import subprocess
from pathlib import Path

from github_release import read_version


def run(*arguments, **options):
    subprocess.run([str(value) for value in arguments], check=True, **options)


def licenses(root, destination):
    destination.mkdir(parents=True, exist_ok=True)
    for source, name in (
        (root / 'LICENSE', 'EUI-LICENSE.txt'),
        (root / 'THIRD_PARTY_NOTICES.md', 'THIRD_PARTY_NOTICES.md'),
        (root / 'example/quick3d/littlest-tokyo/assets/ATTRIBUTION.md', 'LittlestTokyo-ATTRIBUTION.md'),
        (root / 'source/qt5/imports/eui/font/icons/LICENSE', 'Skill-Icons-LICENSE.txt'),
        (root / 'source/thirdparty/echarts/LICENSE', 'ECharts-LICENSE.txt'),
        (root / 'source/thirdparty/echarts/NOTICE', 'ECharts-NOTICE.txt'),
    ):
        shutil.copy2(source, destination / name)
    for file in (root / 'packaging/licenses').glob('Qt-*.txt'):
        shutil.copy2(file, destination / file.name)
    shutil.copy2(root / 'packaging/licenses/FFmpeg-LGPL-2.1.txt', destination)
    text = (root / 'source/thirdparty/qrcodegen/qrcodegen.hpp').read_text(encoding='utf-8')
    (destination / 'QR-Code-generator-LICENSE.txt').write_text(
        re.match(r'/\*.*?\*/', text, re.S).group(), encoding='utf-8')


def deploy_macos(root, app, qt, run=run):
    licenses(root, app / 'Contents/Resources/licenses')
    imports = app.parent / 'qml'
    imports.mkdir()
    shutil.move(app / 'Contents/MacOS/EUI', imports / 'EUI')
    (app / 'Contents/Resources/qt.conf').write_text(
        '[Paths]\nPlugins = PlugIns\nImports = Resources/qml\nQmlImports = Resources/qml\n', encoding='utf-8')
    plugin_files = [qt / 'plugins' / relative for relative in (
        'platforms/libqcocoa.dylib', 'platforms/libqoffscreen.dylib',
        'sqldrivers/libqsqlite.dylib',
    )]
    for group in ('imageformats', 'iconengines', 'networkinformation', 'tls', 'printsupport', 'multimedia', 'position'):
        plugin_files += [file for file in (qt / 'plugins' / group).glob('*.dylib')
                         if not file.name.endswith('_debug.dylib') and 'nmea' not in file.name]
    executables = []
    for file in plugin_files:
        relative = file.relative_to(qt / 'plugins')
        destination = app / 'Contents/PlugIns' / relative
        destination.parent.mkdir(parents=True, exist_ok=True)
        shutil.copy2(file, destination)
        executables.append('-executable=' + str(destination))
    run(qt / 'bin/macdeployqt', app, '-always-overwrite', '-no-plugins',
        '-qmldir=' + str(root / 'example'), '-qmlimport=' + str(imports), '-codesign=-', *executables)
    run('codesign', '--verify', '--deep', '--strict', app)


def package_macos(root, stage, output, qt, version, arch):
    app = stage / 'EUI Gallery.app'
    shutil.copytree(root / 'bin/Release/example.app', app, symlinks=True)
    deploy_macos(root, app, qt)
    executable = app / 'Contents/MacOS/example'
    run('lipo', executable, '-verify_arch', 'arm64' if arch == 'arm64' else 'x86_64')
    run('ditto', '-c', '-k', '--sequesterRsrc', '--keepParent', app,
        output / f'EUI-Gallery-{version}-macos-{arch}-portable.zip')
    image = stage / 'dmg'
    image.mkdir()
    shutil.copytree(app, image / app.name, symlinks=True)
    (image / 'Applications').symlink_to('/Applications')
    run('hdiutil', 'create', '-volname', 'EUI Gallery', '-srcfolder', image,
        '-format', 'UDZO', '-ov', output / f'EUI-Gallery-{version}-macos-{arch}.dmg')


def main():
    parser = argparse.ArgumentParser(description='Build native macOS Gallery packages with Qt 6.')
    parser.add_argument('--qt-dir', required=True, type=Path)
    arguments = parser.parse_args()
    arch = {'arm64': 'arm64', 'x86_64': 'x64'}.get(platform.machine())
    if platform.system() != 'Darwin' or arch is None:
        parser.error('Requires macOS x64 or macOS arm64.')
    root = Path(__file__).resolve().parents[1]
    qt = arguments.qt_dir.resolve()
    version = read_version((root / 'CMakeLists.txt').read_text(encoding='utf-8'))
    name = 'macos-' + arch
    build = root / 'build' / (name + '-package')
    stage = build / 'package'
    output = root / 'dist'
    if stage.exists():
        if stage.is_symlink() or stage.resolve() != (build.resolve() / 'package'):
            raise ValueError('Refusing to clean an unexpected package directory.')
        shutil.rmtree(stage)
    stage.mkdir(parents=True)
    output.mkdir(exist_ok=True)
    cmake = ['cmake', '-S', root, '-B', build, '-G', 'Ninja', '-DCMAKE_BUILD_TYPE=Release',
             '-DCMAKE_PREFIX_PATH=' + str(qt), '-DEUI_BUILD_EXAMPLES=ON',
             '-DEUI_BUILD_TESTS=OFF', '-DEUI_BUILD_STATIC_LIB=OFF', '-DQML_PLUGIN_DUMP:FILEPATH=OFF']
    cmake += ['-DCMAKE_OSX_ARCHITECTURES=' + ('arm64' if arch == 'arm64' else 'x86_64'),
              '-DCMAKE_OSX_DEPLOYMENT_TARGET=12.0']
    run(*cmake)
    run('cmake', '--build', build, '--target', 'example', '--parallel', str(os.cpu_count() or 2))
    package_macos(root, stage, output, qt, version, arch)
    packages = sorted(output.glob(f'EUI-Gallery-{version}-{name}*'))
    if len(packages) != 2:
        raise ValueError('Expected exactly an installer and a portable package.')
    checksums = []
    for file in packages:
        with file.open('rb') as stream:
            checksums.append(hashlib.file_digest(stream, 'sha256').hexdigest() + '  ' + file.name)
    (output / 'SHA256SUMS.txt').write_text('\n'.join(checksums) + '\n', encoding='ascii')
    print('Packages created in', output)


if __name__ == '__main__':
    main()
