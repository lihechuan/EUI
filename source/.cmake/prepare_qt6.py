import re
import shutil
import sys
import xml.etree.ElementTree as ET
from pathlib import Path


def replace(text, before, after):
    if before not in text:
        raise ValueError('Qt 6 adaptation no longer matches: ' + before)
    return text.replace(before, after)


def prepare(source, destination):
    shutil.copytree(source / 'imports', destination / 'imports', dirs_exist_ok=True)
    module = destination / 'imports/eui'
    for file in module.rglob('*.qml'):
        text = file.read_text(encoding='utf-8')
        text = text.replace('import Qt.labs.calendar 1.0\n', '')
        text = text.replace('import QtQuick.Controls 2.15', 'import QtQuick.Controls')
        if file.name == 'ECalendar.qml':
            text = replace(text, 'import QtQuick 2.15\n', 'import QtQuick 2.15\nimport QtQuick.Controls\n')
        if re.search(r'\b(QtObject|Timer|Connections|Component|Binding)\b', text):
            text = replace(text, 'import QtQuick 2.15\n', 'import QtQuick 2.15\nimport QtQml 2.15\n')
        file.write_text(text, encoding='utf-8')
    clip = module / 'controls/effects/EClip.qml'
    clip.write_text(replace(clip.read_text(encoding='utf-8'),
                            'import QtGraphicalEffects 1.15', 'import Qt5Compat.GraphicalEffects'), encoding='utf-8')

    media = module / 'controls/data/EMediaPlayer.qml'
    text = media.read_text(encoding='utf-8')
    for before, after in (
        ('import QtMultimedia 5.15', 'import QtMultimedia'),
        ('player.muted', 'audioOutput.muted'),
        ('player.volume', 'audioOutput.volume'),
        ('property alias autoPlay: player.autoPlay', 'property bool autoPlay: false'),
        ('        source: player\n', '        id: videoOutput\n'),
        ('onError: control.failed(errorString)',
         'audioOutput: AudioOutput {\n            id: audioOutput\n        }\n'
         '        videoOutput: videoOutput\n\n'
         '        onErrorOccurred: control.failed(errorString)\n'
         '        onSourceChanged: if (control.autoPlay) play()'),
    ):
        text = replace(text, before, after)
    media.write_text(text, encoding='utf-8')

    upload = module / 'controls/form/EUpload.qml'
    text = upload.read_text(encoding='utf-8')
    for before, after in (
        ('import QtQuick.Dialogs 1.3', 'import QtQuick.Dialogs'),
        ('fileDialogComponent.createObject(control)',
         '(control.directory ? folderDialogComponent : fileDialogComponent).createObject(control)'),
        ('selectFolder: control.directory\n            ', ''),
        ('selectMultiple: !control.directory && control.multiple',
         'fileMode: control.multiple ? FileDialog.OpenFiles : FileDialog.OpenFile'),
        ('control.directory ? control.addDirectories([fileUrl]) : control.addFiles(fileUrls)',
         'control.addFiles(selectedFiles)'),
        ('    Component {\n        id: defaultTrigger',
         '    Component {\n        id: folderDialogComponent\n\n        FolderDialog {\n'
         '            objectName: "uploadFileDialog"\n'
         '            title: EControlConfig.textFor(control, "SelectFiles", ELocale.texts)\n\n'
         '            onAccepted: control.addDirectories([selectedFolder])\n        }\n    }\n'
         '    Component {\n        id: defaultTrigger'),
    ):
        text = replace(text, before, after)
    upload.write_text(text, encoding='utf-8')

    picker = module / 'controls/form/EFilePicker.qml'
    text = picker.read_text(encoding='utf-8')
    for before, after in (
        ('import QtQuick.Dialogs 1.3', 'import QtQuick.Dialogs'),
        ('fileDialogComponent.createObject(control)',
         '(mode === "folder" ? folderDialogComponent : fileDialogComponent).createObject(control)'),
        ('folder: control.folder', 'currentFolder: control.folder'),
        ('            selectFolder: control.mode === "folder"\n', ''),
        ('            selectExisting: control.mode !== "save"\n', ''),
        ('selectMultiple: control.multiple && control.mode === "open"',
         'fileMode: control.mode === "save" ? FileDialog.SaveFile\n'
         '                : control.multiple ? FileDialog.OpenFiles : FileDialog.OpenFile'),
        ('control.acceptSelection(fileUrls)', 'control.acceptSelection(selectedFiles)'),
    ):
        text = replace(text, before, after)
    text = text.rstrip()[:-1] + '''
    Component {
        id: folderDialogComponent

        FolderDialog {
            currentFolder: control.folder
            title: control.dialogTitle

            onRejected: control.rejected()
            onAccepted: control.acceptSelection([selectedFolder])
        }
    }
}
'''
    picker.write_text(text, encoding='utf-8')

    shaders = destination / 'shaders'
    shaders.mkdir(exist_ok=True)
    for path, name in (
        ('controls/data/ECarousel.qml', 'motion-blur'),
        ('controls/form/EThemeSwitch.qml', 'theme-switch'),
        ('controls/basic/EApplicationWindow.qml', 'theme-reveal'),
    ):
        file = module / path
        text = file.read_text(encoding='utf-8')
        match = re.search(r'fragmentShader: (?:\(GraphicsInfo.profile.*?\) \+ )?"((?:[^"\\]|\\.)*)"', text, re.S)
        if not match:
            raise ValueError('Cannot extract shader: ' + path)
        shader = match.group(1)
        declarations = re.findall(r'uniform\s+(?:(?:lowp|highp|mediump)\s+)?(\w+)\s+(\w+)\s*;', shader)
        uniforms = ['    mat4 qt_Matrix;', '    float qt_Opacity;']
        uniforms += [f'    {kind} {key};' for kind, key in declarations
                     if kind != 'sampler2D' and key != 'qt_Opacity']
        samplers = [key for kind, key in declarations if kind == 'sampler2D']
        shader = re.sub(r'uniform\s+[^;]+;', '', shader)
        shader = re.sub(r'varying\s+[^;]+;', '', shader)
        shader = re.sub(r'\b(lowp|highp|mediump)\s+', '', shader)
        shader = shader.replace('texture2D(', 'texture(').replace('gl_FragColor', 'fragColor')
        header = '#version 440\nlayout(location = 0) in vec2 qt_TexCoord0;\n'
        header += 'layout(location = 0) out vec4 fragColor;\nlayout(std140, binding = 0) uniform buf {\n'
        header += '\n'.join(uniforms) + '\n};\n'
        header += ''.join(f'layout(binding = {index}) uniform sampler2D {key};\n'
                          for index, key in enumerate(samplers, 1))
        (shaders / (name + '.frag')).write_text(header + shader.strip() + '\n', encoding='utf-8')
        text = text[:match.start()] + f'fragmentShader: "qrc:/qt/qml/EUI/Shaders/{name}.frag.qsb"' + text[match.end():]
        file.write_text(text, encoding='utf-8')

    resource = ET.parse(source / 'imports/eui.qrc')
    for file in resource.iter('file'):
        alias = file.text
        file.set('alias', file.get('alias', alias))
        file.text = (destination / 'imports' / alias).as_posix()
    resource.write(destination / 'eui.qrc', encoding='utf-8')


if __name__ == '__main__':
    prepare(Path(sys.argv[1]).resolve(), Path(sys.argv[2]).resolve())
