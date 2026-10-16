import EUI 1.0
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function descendants(root) {
        const items = []
        const pending = [root]
        while (pending.length > 0) {
            const item = pending.pop()
            items.push(item)
            const children = item.children || []
            for (let index = children.length - 1; index >= 0; --index)
                pending.push(children[index])
        }
        return items
    }

    function test_capture_data() {
        const cases = []
        for (const language of ["zh", "en"]) {
            for (const dark of [false, true]) {
                for (const group of exampleCatalog.groups) {
                    for (const entry of group.children) {
                        if (!entry.demo)
                            cases.push({tag: entry.index + (dark ? "-dark" : "") + "-" + language,
                                language: language, dark: dark, entry: entry})
                    }
                }
            }
        }
        return cases
    }

    function test_capture(data) {
        ELocale.language = data.language
        ETheme.setDark(data.dark)
        const definition = Qt.createComponent("qrc:/example/qml/component/App.qml")
        compare(definition.status, Component.Ready, definition.errorString())
        const window = definition.createObject(null, {width: 1280, height: 1000})
        verify(window !== null)
        try {
            const loader = findChild(window, "galleryLoader")
            tryCompare(loader, "status", Loader.Ready, 20000)
            window.showComponent(data.entry)
            tryCompare(loader, "status", Loader.Ready, 20000)
            const page = loader.item
            tryVerify(function() { return page.previewsReady }, 15000)
            let preview = null
            tryVerify(function() {
                preview = descendants(page).find(function(item) {
                    return item.objectName === "galleryPreview" && item.visible
                        && !descendants(item).slice(1).some(function(child) {
                            return child.objectName === "galleryPreview" && child.visible
                        })
                })
                return !!preview
            }, 15000)
            verify(preview.width > 0 && preview.height > 0)
            page.contentY = Math.max(0, preview.mapToItem(page, 0, 0).y - 24)
            window.update()
            verify(waitForRendering(window.contentItem))
            wait(350)
            for (let depth = 0; depth < 8; ++depth) {
                const section = descendants(preview).find(function(item) {
                    return item.activated !== undefined && item.selected && item.loadedCard === null
                })
                if (section) {
                    section.activated = true
                    tryVerify(function() { return section.loadedCard !== null }, 15000)
                    wait(100)
                }
                const nested = descendants(preview).slice(1).filter(function(item) {
                    return item.objectName === "galleryPreview" && item.visible
                })
                if (nested.length === 0)
                    break
                preview = nested[nested.length - 1]
            }
            page.contentY += preview.mapToItem(page, 0, 0).y - 24
            window.update()
            verify(waitForRendering(window.contentItem))
            let target = preview
            const name = data.entry.index
            const buttons = descendants(preview).filter(function(item) {
                return item.clicked !== undefined && typeof item.text === "string" && item.visible
            })
            if (/^(EDialog|EDrawer|EPopover|EPopconfirm|EDropdown|EMessageBox|EDialogService|EDynamicDialog|ECommandPalette|EImageViewer|ETour)$/.test(name)) {
                verify(buttons.length > 0, "Missing popup trigger: " + name)
                buttons[0].clicked()
                tryVerify(function() {
                    const popups = descendants(window.contentItem.parent).filter(function(item) {
                        return /^QQuickPopupItem/.test(item.toString()) && item.visible && item.opacity > 0
                    })
                    if (popups.length > 0)
                        target = popups[popups.length - 1]
                    return target !== preview
                }, 5000)
                wait(350)
            } else if (name === "EToolTip") {
                verify(buttons.length > 0)
                const contents = buttons[0].data
                let tooltip = null
                for (let index = 0; index < contents.length; ++index) {
                    if (/^EToolTip_/.test(contents[index].toString()))
                        tooltip = contents[index]
                }
                verify(tooltip !== null)
                tooltip.visible = true
                wait(350)
                const popups = descendants(window.contentItem.parent).filter(function(item) {
                    return /^QQuickPopupItem/.test(item.toString()) && item.visible && item.opacity > 0
                })
                verify(popups.length > 0)
                target = popups[popups.length - 1]
            } else if (name === "EMessage" || name === "ENotification") {
                verify(buttons.length > 0)
                buttons[name === "EMessage" ? 1 : 0].clicked()
                tryVerify(function() {
                    const cards = descendants(window.contentItem.parent).filter(function(item) {
                        return /^(MessageCard|NotificationCard)_/.test(item.toString()) && item.visible && item.opacity > 0
                    })
                    if (cards.length > 0)
                        target = cards[0]
                    return target !== preview
                }, 5000)
                wait(350)
            } else if (name === "ELoading") {
                verify(buttons.length > 0)
                buttons[0].clicked()
                wait(150)
            } else if (name === "EECharts") {
                const chart = findChild(page, "galleryApacheChart")
                verify(chart !== null && chart.supported)
                tryCompare(chart, "ready", true, 20000)
                compare(chart.errorString, "")
                wait(1200)
            }
            if (target === preview && target.color !== undefined) {
                target.color = "transparent"
                for (let index = 0; index < target.children.length; ++index) {
                    const child = target.children[index]
                    if (child.height === 1 && child.width === target.width && child.color !== undefined)
                        child.visible = false
                }
            }
            window.update()
            verify(waitForRendering(window.contentItem))
            const url = Qt.resolvedUrl("../../docs/screenshots/" + exampleUpdater.currentVersion
                + "/components/" + data.tag + ".png").toString()
            const file = decodeURIComponent(url.replace(Qt.platform.os === "windows" ? /^file:\/\/\// : /^file:\/\//, ""))
            let saved = false
            verify(target.grabToImage(function(result) { saved = result.saveToFile(file) },
                Qt.size(Math.ceil(target.width * 2), Math.ceil(target.height * 2))))
            tryVerify(function() { return saved }, 5000)
            console.log("PREVIEW", data.tag, target.width, target.height)
        } finally {
            window.close()
            window.destroy()
            wait(0)
            definition.destroy()
        }
    }

    when: windowShown
    name: "DocumentationPreviews"
}
