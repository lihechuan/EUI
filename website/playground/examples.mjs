// Adapt Gallery excerpts to the component page's own preview surface.
export function prepareExamples(name, samples, language) {
  const zh = language === 'zh'
  const desktop = {
    EECharts: zh ? '此组件使用 Qt WebEngine。请在 Gallery 中运行这些图表示例。' : 'This component uses Qt WebEngine. Run these chart examples in the Gallery.',
    EMediaPlayer: zh ? '音视频播放依赖桌面多媒体后端，请在 Gallery 中体验。' : 'Media playback requires the desktop multimedia backend. Try it in the Gallery.',
    EFilePicker: zh ? '文件选择涉及本机文件系统，请在 Gallery 中体验。' : 'File selection uses the local filesystem. Try it in the Gallery.',
    EFolderPicker: zh ? '文件夹选择涉及本机文件系统，请在 Gallery 中体验。' : 'Folder selection uses the local filesystem. Try it in the Gallery.',
    EUpload: zh ? '此示例需要选择本地文件及配置上传服务，请在 Gallery 中运行。' : 'This example requires local file selection and an upload service. Run it in the Gallery.',
  }
  return samples.map(sample => {
    let code = sample.code.replace(/^(?:\\x20)+\s*$/gm, '')
      .replace(/"\$\{(qsTr\("[^"\n]*"\))\}"/g, '$1')
      .replaceAll('${playground.invocation}', 'EAnimation.play(animationCard, "bounce")')
    const imports = new Set(code.match(/^import .+$/gm) || [])
    imports.add('import QtQuick.Controls 2.15')
    imports.add('import QtQuick.Layouts 1.15')
    let body = code.replace(/^import .+\n/gm, '').trim()
    if (/^EApplicationWindow\s*\{/.test(body) && name !== 'EAppBar') {
      body = body.replace(/^EApplicationWindow/, 'Item').replace(/^ {4}(?:visible|flags|title):.*\n/gm, '')
    }
    if (name === 'EAnimation') body = body.replaceAll('"${', '"')
    if (name === 'EImageViewer') body = body.replace(/^EImage\s*\{/, 'EImageViewer {')
    body = body.replace(/allowDownload:\s*true/g, 'allowDownload: false')
    if (!/allowDownload:/.test(body)) body = body.replace(/\b(EImage|EImageViewer)\s*\{/g, '$1 {\n    allowDownload: false')
    if (name === 'EAppBar') body = `EAppBar {
    width: parent.width
    title: ${JSON.stringify(zh ? '我的应用' : 'My application')}
    showClose: false
    showMaximize: false
    showMinimize: false

    EThemeSwitch {}
}`
    if (name === 'EAffix') body = `Flickable {
    id: flickable

    clip: true
    height: 280
    width: parent.width
    contentHeight: 760

    Column {
        width: parent.width
        spacing: 20

        EText { text: ${JSON.stringify(zh ? '向下滚动，按钮会固定在顶部' : 'Scroll down to pin the button')} }
        EAffix {
            target: flickable
            EButton { text: ${JSON.stringify(zh ? '固定操作' : 'Pinned action')} }
        }
        Repeater {
            model: 14
            EText { text: ${JSON.stringify(zh ? '内容 ' : 'Content ')} + (index + 1) }
        }
    }
}`
    if (name === 'EAnchor' || name === 'EBacktop') body = `Item {
    width: parent.width
    height: 280

    Flickable {
        id: scroll

        clip: true
        width: parent.width - 130
        height: parent.height
        contentHeight: sections.height

        Column {
            id: sections

            width: parent.width
            spacing: 20

            ECard {
                id: overview

                height: 240
                width: parent.width
                header: ${JSON.stringify(zh ? '概览' : 'Overview')}
            }
            ECard {
                id: details

                height: 240
                width: parent.width
                header: ${JSON.stringify(zh ? '详细内容' : 'Details')}
            }
        }
    }
    ${name === 'EAnchor' ? `EAnchor {
        x: parent.width - 120
        target: scroll
        model: [{ title: ${JSON.stringify(zh ? '概览' : 'Overview')}, target: overview },
            { title: ${JSON.stringify(zh ? '详细内容' : 'Details')}, target: details }]
    }` : 'EBacktop { target: scroll }'}
}`
    if (name === 'EPopconfirm' || name === 'EPopover') body = `Column {
    spacing: 16

    EButton {
        id: referenceButton

        text: ${JSON.stringify(zh ? '点击查看' : 'Click to open')}
    }
    ${name} {
        reference: referenceButton
        text: ${JSON.stringify(zh ? (name === 'EPopconfirm' ? '确定执行此操作？' : '这里是弹出内容') : (name === 'EPopconfirm' ? 'Confirm this action?' : 'Popover content'))}
    }
}`
    if (name === 'ETour' && /startButton/.test(body)) body = `Column {
    EButton {
        id: startButton

        text: ${JSON.stringify(zh ? '启动引导' : 'Start tour')}
        onClicked: tour.start()
    }
    ${body.split('\n').join('\n    ')}
}`
    if (name === 'EInfiniteScroll' && /model: items/.test(body)) body = body.replace('EInfiniteScroll {', `EInfiniteScroll {
    property var items: ["Alpha", "Beta", "Gamma", "Delta"]
    property var nextItems: ["Epsilon", "Zeta", "Eta", "Theta"]

    height: 240
    width: parent.width`)
    if (/^(EDialog|EDrawer)\s*\{/.test(body)) body = `Column {
    EButton {
        text: ${JSON.stringify(zh ? '打开' : 'Open')}
        onClicked: examplePopup.open()
    }
    ${body.replace(/^(\w+)\s*\{/, '$1 {\n    id: examplePopup').split('\n').join('\n    ')}
}`
    const roots = [...body.matchAll(/^[A-Z]\w*\s*\{/gm)]
    if (roots.length > 1) body = `Column {
    spacing: 16
    width: parent.width

${body.split('\n').map(line => line ? '    ' + line : '').join('\n')}
}`
    if ((name === 'EMessageBox' || name === 'ENotification') && !/text:/.test(body)) {
      body = body.replace('EButton {', `EButton {\n        text: ${JSON.stringify(zh ? '打开' : 'Open')}`)
    }
    body = body.replace(/^ {4}width: (640|760)\s*$/m, '    width: Math.min($1, parent.width)')
    code = [...imports].sort((a, b) => {
      const category = line => line.startsWith('import EUI') ? 0 : /^import QtQuick \d/.test(line) ? 1 : 2
      return category(a) - category(b) || a.length - b.length || a.localeCompare(b)
    }).join('\n') + '\n\n' + body
    return { ...sample, code, ...(desktop[name] ? { reason: desktop[name] } : {}) }
  })
}
