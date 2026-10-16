"""Build the browser QML runtime from the current EUI sources."""

import argparse
import os
import re
import shutil
import subprocess
from pathlib import Path
from xml.etree.ElementTree import Element, SubElement, ElementTree

from fontTools.ttLib import TTFont
from fontTools import subset
from fontTools.varLib.instancer import instantiateVariableFont


parser = argparse.ArgumentParser()
parser.add_argument('--qt', type=Path, required=True)
parser.add_argument('--emsdk', type=Path, required=True)
parser.add_argument('--python', type=Path, required=True)
parser.add_argument('--make', type=Path, required=True)
parser.add_argument('--font', type=Path)
args = parser.parse_args()
directory = Path(__file__).resolve().parent
root = directory.parents[1]
output = root / 'build/playground/runtime'
output.mkdir(parents=True, exist_ok=True)
environment = os.environ.copy()
environment['PATH'] = os.pathsep.join([str(args.python.parent), str(args.emsdk / 'upstream/emscripten'), str(args.make.parent), environment['PATH']])
environment['EM_CONFIG'] = str(output / 'emscripten.config')
environment['EM_CACHE'] = str(root / 'build/playground/cache')
configuration = '\n'.join(f'{key} = {str(value.as_posix())!r}' for key, value in {
    'LLVM_ROOT': args.emsdk / 'upstream/bin',
    'BINARYEN_ROOT': args.emsdk / 'upstream',
    'EMSCRIPTEN_ROOT': args.emsdk / 'upstream/emscripten',
    'NODE_JS': next((args.emsdk / 'node').glob('*/node.exe')),
    'PYTHON': args.python,
}.items())
configuration_file = output / 'emscripten.config'
if not configuration_file.exists() or configuration_file.read_text(encoding='utf-8') != configuration:
    configuration_file.write_text(configuration, encoding='utf-8')

font = TTFont(args.font or root / 'website/.vitepress/theme/fonts/noto-sans-sc.woff2')
font.flavor = None
font = instantiateVariableFont(font, {'wght': 400})
if args.font:
    characters = set(range(0x20, 0x100)) | set(range(0x2000, 0x2070)) | set(range(0x3000, 0x3040)) | set(range(0xff00, 0xffef))
    characters.update(ord(character) for lead in range(0xa1, 0xf8) for trail in range(0xa1, 0xff)
                      for character in bytes([lead, trail]).decode('gb2312', errors='ignore'))
    subsetter = subset.Subsetter()
    subsetter.populate(unicodes=characters)
    subsetter.subset(font)
font.save(output / 'NotoSansSC.ttf')

resources = Element('RCC')
eui = root / 'source/qt5/imports/eui'
group = SubElement(resources, 'qresource', prefix='/qt/qml/EUI')
for file in sorted((eui / 'controls').rglob('*')):
    if file.suffix in ['.qml', '.js']:
        SubElement(group, 'file', alias=file.relative_to(eui).as_posix()).text = file.as_posix()
registrations = []
for name, file in re.findall(r'^(?:internal )?(\w+)(?: 1\.0)? (controls/.*\.qml)$',
                             (eui / 'qmldir').read_text(encoding='utf-8'), re.M):
    registrations.append((name, file, 'EUI'))
for file in (eui / 'controls/private').glob('*.qml'):
    registrations.append((file.stem, file.relative_to(eui).as_posix(), 'EUI.Private'))
registrations.sort(key=lambda entry: (len(''.join(entry)), entry[0]))
(output / 'qml-types.h').write_text('\n'.join(f'{{"{uri}", "{name}", "{file}"}},'
                                           for name, file, uri in registrations), encoding='utf-8')
for file in ['i18n/en.json', 'i18n/zh.json', 'font/EFont.ttf']:
    SubElement(group, 'file', alias=file).text = (eui / file).as_posix()
group = SubElement(resources, 'qresource', prefix='/example/images/gallery')
for file in sorted((root / 'assets/images/gallery').glob('*.jpg')):
    SubElement(group, 'file', alias=file.name).text = file.as_posix()
group = SubElement(resources, 'qresource', prefix='/playground')
SubElement(group, 'file', alias='Main.qml').text = (directory / 'Main.qml').as_posix()
SubElement(group, 'file', alias='NotoSansSC.ttf').text = (output / 'NotoSansSC.ttf').as_posix()
ElementTree(resources).write(output / 'resources.qrc', encoding='utf-8')

subprocess.run([str(args.qt / 'bin/qmake.exe'), str(directory / 'playground.pro')], cwd=output, env=environment, check=True)
subprocess.run([str(args.make), '-j4'], cwd=output, env=environment, check=True)
runtime = directory / 'runtime'
runtime.mkdir(exist_ok=True)
for file in ['eui-playground.js', 'eui-playground.wasm', 'qtloader.js']:
    shutil.copy2(output / file, runtime / file)
loader = runtime / 'qtloader.js'
loader.write_text(loader.read_text(encoding='utf-8').replace('WebAssembly.compileStreaming(response)',
                  'WebAssembly.compileStreaming(response.clone())'), encoding='utf-8')
print('Built browser runtime:', runtime)
