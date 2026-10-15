import EUI 1.0
import "../../example/qml/component" as Gallery
import QtQuick 2.15
import QtTest 1.2
import QtQuick.Window 2.15

TestCase {
    id: testCase

    function cleanup() {
        ETheme.setDark(false)
        ELocale.language = "zh"
    }

    function test_navigationFonts_data() {
        return [{tag: "light-zh", dark: false, language: "zh"},
            {tag: "dark-zh", dark: true, language: "zh"},
            {tag: "light-en", dark: false, language: "en"},
            {tag: "dark-en", dark: true, language: "en"}]
    }

    function test_navigationHover_data() {
        const rows = []
        for (const state of test_navigationFonts_data()) {
            for (const component of ["EBadge", "EInputGroup"])
                rows.push({tag: state.tag + "-" + component, dark: state.dark, language: state.language, component})
        }
        return rows
    }

    function descendants(root) {
        const result = []
        const pending = [root]
        while (pending.length > 0) {
            const item = pending.pop()
            result.push(item)
            const children = item.children || []
            for (let index = 0; index < children.length; ++index)
                pending.push(children[index])
        }
        return result
    }

    function test_resize_data() {
        const rows = []
        for (const dark of [false, true]) {
            for (const component of ["", "EAnimation", "EIcon", "EInput", "EForm", "ETable", "EFlex"])
                rows.push({tag: (dark ? "dark-" : "light-") + (component || "overview"), dark, component})
        }
        return rows
    }

    function navigationWindow() {
        const window = createTemporaryObject(windowComponent, null)
        verify(window !== null)
        tryCompare(findChild(window, "galleryLoader"), "status", Loader.Ready)
        window.openedGroup = "form"
        const navigation = findChild(window, "galleryNavigation")
        tryVerify(function() { return navigation.contentHeight > navigation.height })
        verify(waitForItemPolished(navigation))
        return window
    }

    function test_pages_data() {
        const rows = []
        for (const dark of [false, true]) {
            for (const pageWidth of [1040, 640]) {
                for (const group of exampleCatalog.groups) {
                    for (const entry of group.children) {
                        if (!entry.demo)
                            rows.push({tag: (dark ? "dark" : "light") + "-" + pageWidth + "-" + entry.index,
                                dark: dark, pageWidth: pageWidth, entry: entry})
                    }
                }
            }
        }
        compare(rows.length, exampleCatalog.groups.reduce(function(count, group) {
            return count + group.children.filter(function(entry) { return !entry.demo }).length
        }, 0) * 4)
        return rows
    }

    function loadExamples(page) {
        function sections() {
            return descendants(page).filter(function(item) {
                return item.loadedCard !== undefined && item.componentName !== undefined
                    && item.objectName !== "galleryApiLoader" && item.selected
            })
        }
        while (true) {
            verify(waitForItemPolished(page))
            const pending = sections().filter(function(section) { return section.loadedCard === null })
            if (!pending.length)
                break
            const section = pending[0]
            tryVerify(function() {
                page.contentY = Math.max(0, section.mapToItem(page.contentItem, 0, 0).y - 16)
                return section.loadedCard !== null
            })
        }
        page.contentY = 0
        verify(waitForItemPolished(page))
        for (const section of sections())
            tryVerify(function() { return section.loadedCard.implicitHeight > 0 })
    }

    function test_overviewCategoryRemoval() {
        const frame = createTemporaryObject(frameComponent, testCase, {width: 1040})
        const definition = Qt.createComponent("../../example/qml/component/Overview.qml")
        tryCompare(definition, "status", Component.Ready)
        const page = createTemporaryObject(definition, frame, {width: frame.width, height: frame.height})
        verify(page !== null)
        verify(waitForItemPolished(page))
        const cards = descendants(page).filter(function(item) { return item.objectName === "galleryCategoryCard" })
        compare(cards.length, exampleCatalog.groups.length)
        page.groups = page.groups.slice(0, 1)
        ETheme.setDark(true)
        verify(waitForItemPolished(page))
        page.groups = exampleCatalog.groups
        ETheme.setDark(false)
        verify(waitForItemPolished(page))
        page.destroy()
        frame.destroy()
        wait(0)
        definition.destroy()
    }

    function test_navigationFonts(data) {
        ETheme.setDark(data.dark)
        ELocale.language = data.language
        const window = navigationWindow()
        try {
            const entries = descendants(window.contentItem).filter(function(item) {
                return item.objectName === "galleryNavEntry"
            })
            verify(entries.length > 10)
            const group = exampleCatalog.groups.filter(function(item) { return item.index === "form" })[0]
            const heading = descendants(findChild(window, "galleryNavigation")).filter(function(item) {
                return item.text === group.text && item.renderType !== undefined
            })[0]
            verify(heading !== undefined)
            for (const entry of entries) {
                const label = entry.contentItem.children[0]
                compare(label.font.family, heading.font.family)
                compare(label.font.family, EDefaultConfig.primaryFontFamily)
                compare(label.font.pixelSize, heading.font.pixelSize)
                compare(label.font.weight, Font.Normal)
                compare(label.renderType, heading.renderType)
            }
        } finally {
            window.close()
        }
    }

    function verifyNavigationPixels(window, entry, dark) {
        const label = entry.contentItem.children[0]
        const image = grabImage(window.contentItem)
        if (!image.width)
            skip("This platform cannot capture pixels; run the same check on the isolated Windows desktop")
        const position = label.mapToItem(window.contentItem, 0, 0)
        const scale = window.Screen.devicePixelRatio
        verify(label.visible && label.opacity > 0 && label.text.length > 0)
        verify(label.width > 0 && label.height >= label.implicitHeight)
        let ink = 0
        const right = Math.min(image.width, Math.floor((position.x + label.width) * scale))
        const bottom = Math.min(image.height, Math.floor((position.y + label.height) * scale))
        for (let y = Math.max(0, Math.ceil(position.y * scale)); y < bottom; ++y) {
            for (let x = Math.max(0, Math.ceil(position.x * scale)); x < right; ++x) {
                const red = image.red(x, y)
                const green = image.green(x, y)
                const blue = image.blue(x, y)
                if (dark ? red > 180 && green > 180 && blue > 180 : red < 100 && green < 100 && blue < 100)
                    ++ink
            }
        }
        verify(ink > 5, "Navigation text must remain painted after hover: " + ink)
    }

    function test_resize(data) {
        ETheme.setDark(data.dark)
        ELocale.language = data.dark ? "en" : "zh"
        const entry = exampleCatalog.find(data.component)
        const definition = Qt.createComponent("../../example/qml/component/" + (entry ? entry.source : "Overview.qml"))
        tryCompare(definition, "status", Component.Ready)
        const frame = createTemporaryObject(frameComponent, testCase, {width: 1040})
        const properties = {width: frame.width, height: frame.height}
        if (entry)
            properties.selectedComponent = entry.index
        const page = createTemporaryObject(definition, frame, properties)
        verify(page !== null, definition.errorString())
        if (entry)
            loadExamples(page)
        const content = entry ? findChild(page, "galleryPageColumn") : findChild(page, "galleryOverviewHero").parent
        verify(content !== null)
        for (const pageWidth of [640, 1040, 1920, 2560, 1040, 640]) {
            frame.width = pageWidth
            page.width = pageWidth
            verify(waitForItemPolished(page))
            compare(content.x, entry ? 28 : 24)
            compare(content.width, pageWidth - (entry ? 56 : 48))
            if (entry) {
                const api = findChild(page, "galleryApiLoader")
                compare(api.x, content.x)
                compare(api.width, content.width)
                const panels = descendants(page).filter(function(item) {
                    return item.objectName === "galleryExamplePanel" && item.visible
                })
                verify(panels.length >= 5)
                for (const panel of panels)
                    compare(panel.width, content.width)
            } else {
                compare(findChild(page, "galleryOverviewHero").width, content.width)
                compare(findChild(page, "galleryCategoryGrid").width, content.width)
            }
        }
        page.destroy()
        frame.destroy()
        wait(0)
        definition.destroy()
        wait(0)
    }

    function test_navigationWheel() {
        const window = navigationWindow()
        try {
            const navigation = findChild(window, "galleryNavigation")
            navigation.contentY = 0
            mouseWheel(navigation, 80, 80, 0, -120)
            wait(300)
            compare(Math.round(navigation.contentY), 128)
            mouseWheel(navigation, 80, 80, 0, -60)
            tryCompare(navigation, "contentY", 192)
            mouseWheel(navigation, 80, 80, 0, 120)
            tryCompare(navigation, "contentY", 64)
            mouseWheel(navigation, 80, 80, 0, 1200)
            tryCompare(navigation, "contentY", 0)
            mouseWheel(navigation, 80, 80, 0, -12000)
            tryCompare(navigation, "contentY", navigation.contentHeight - navigation.height)
            navigation.contentY = 48
            mouseWheel(navigation, 80, 80, 120, 0)
            wait(100)
            compare(navigation.contentY, 48)
            navigation.contentY = 0
            mouseWheel(navigation, navigation.width - 8, 80, 0, -120)
            tryCompare(navigation, "contentY", 128)
            const search = findChild(window, "gallerySearch")
            search.text = "__no_matching_component__"
            tryVerify(function() { return navigation.contentHeight <= navigation.height })
            mouseWheel(navigation, 80, 80, 0, -120)
            tryCompare(navigation, "contentY", 0)
            search.text = ""
            tryVerify(function() { return navigation.contentHeight > navigation.height })
            navigation.contentY = 0
            const entry = descendants(navigation).filter(function(item) {
                if (item.objectName !== "galleryNavEntry")
                    return false
                const top = item.mapToItem(navigation, 0, 0).y
                return top >= 0 && top + item.height <= navigation.height
            })[0]
            verify(entry !== undefined)
            mouseClick(entry, entry.width / 2, entry.height / 2)
            tryCompare(window, "currentComponent", entry.entry.index)
        } finally {
            window.close()
        }
    }

    function test_pages(data) {
        ETheme.setDark(data.dark)
        ELocale.language = data.dark ? "en" : "zh"
        const frame = createTemporaryObject(frameComponent, testCase, {width: data.pageWidth})
        const definition = Qt.createComponent("../../example/qml/component/" + data.entry.source)
        tryCompare(definition, "status", Component.Ready)
        const page = createTemporaryObject(definition, frame, {
            width: data.pageWidth, height: frame.height, selectedComponent: data.entry.index
        })
        verify(page !== null, definition.errorString())
        tryVerify(function() { return page.exampleCount >= 5 })
        loadExamples(page)
        const previews = descendants(page).filter(function(item) {
            return item.objectName === "galleryPreviewContent" && item.visible
        })
        verify(previews.length >= 5)
        for (const preview of previews) {
            for (let index = 0; index < preview.children.length; ++index) {
                const child = preview.children[index]
                if (!child.visible || child.height <= 0)
                    continue
                verify(isFinite(child.width) && isFinite(child.height))
                verify(child.x >= -1 && child.x + child.width <= preview.width + 1,
                    data.entry.index + " preview overflows at width " + data.pageWidth)
            }
        }
        if (data.entry.index === "EPagination") {
            const example = findChild(page, "showcaseEPagination0")
            const pagination = findChild(example, "galleryPreviewContent").children[0]
            const left = findChild(pagination, "paginationLeft")
            verify(left !== null)
            verify(left.width <= pagination.availableWidth + 1)
        }
        page.contentY = 0
        verify(waitForItemPolished(frame))
        wait(100)
        let rendered = false
        verify(frame.grabToImage(function(result) {
            rendered = result.saveToFile(Qt.resolvedUrl("../../build/library-validation/gallery-images/"
                + data.tag + ".png").toString().replace("file:///", ""))
        }))
        tryVerify(function() { return rendered })
        page.destroy()
        frame.destroy()
        wait(0)
        definition.destroy()
        wait(0)
    }

    function test_navigationHover(data) {
        ETheme.setDark(data.dark)
        ELocale.language = data.language
        const window = createTemporaryObject(windowComponent, null)
        verify(window !== null)
        try {
            const loader = findChild(window, "galleryLoader")
            for (const component of ["EInput", "EButton", data.component]) {
                window.showComponent(exampleCatalog.find(component))
                tryCompare(loader, "status", Loader.Ready)
                wait(160)
            }
            const navigation = findChild(window, "galleryNavigation")
            const entry = descendants(navigation).filter(function(item) {
                return item.objectName === "galleryNavEntry" && item.entry.index === data.component
            })[0]
            verify(entry !== undefined)
            verify(waitForItemPolished(entry))
            navigation.contentY = Math.max(0, entry.mapToItem(navigation.contentItem, 0, 0).y - 120)
            const buttons = descendants(loader.item).filter(function(item) {
                return item.hovered !== undefined && item.text !== undefined && item.width > 20 && item.visible
            })
            verify(buttons.length > 0)
            for (let cycle = 0; cycle < 24; ++cycle) {
                navigation.contentY = Math.max(0, navigation.contentY + (cycle % 2 ? -0.5 : 0.5))
                const position = entry.mapToItem(navigation, 0, 0)
                if (position.y < 0 || position.y + entry.height > navigation.height)
                    navigation.contentY += position.y - Math.min(120, navigation.height - entry.height)
                mouseMove(entry, 60, entry.height / 2, 5)
                tryCompare(entry, "hovered", true)
                if (cycle % 4 === 0)
                    verifyNavigationPixels(window, entry, data.dark)
                let button = null
                for (const candidate of buttons) {
                    const target = candidate.mapToItem(window.contentItem, candidate.width / 2, candidate.height / 2)
                    if (target.x > 232 && target.x < window.width && target.y > 84 && target.y < window.height) {
                        button = candidate
                        break
                    }
                }
                verify(button !== null, "A preview control must be visible for the hover round trip")
                mouseMove(button, button.width / 2, button.height / 2, 5)
                tryCompare(button, "hovered", true)
                tryCompare(entry, "hovered", false)
            }
            verifyNavigationPixels(window, entry, data.dark)
        } finally {
            window.close()
            window.destroy()
            wait(0)
        }
    }

    height: 820
    width: 1040
    visible: true
    when: windowShown
    name: "LibraryGalleryMatrix"

    Component {
        id: frameComponent

        Rectangle {
            height: 820
            color: Gallery.GalleryStyle.canvas
        }
    }
    Component {
        id: windowComponent

        Gallery.App {
            height: 840
            asynchronousLoading: false
        }
    }
}
