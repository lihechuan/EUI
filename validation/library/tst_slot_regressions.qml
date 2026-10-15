import EUI 1.0
import "../../example/qml/component/PrimeSamples.js" as Samples
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function cleanup() {
        ETheme.setDark(false)
        ELocale.language = "zh"
    }

    function test_schedulerLanguage_data() {
        return ["month", "week", "day"].map(function(view) { return {tag: view, view: view} })
    }

    function luminance(color) {
        function linear(value) {
            return value <= 0.04045 ? value / 12.92 : Math.pow((value + 0.055) / 1.055, 2.4)
        }
        return 0.2126 * linear(color.r) + 0.7152 * linear(color.g) + 0.0722 * linear(color.b)
    }

    function test_intrinsicSlots_data() {
        const rows = []
        for (const name of ["ESelect", "ECheckBoxGroup", "EInputTag", "ESpinBox"]) {
            for (const dark of [false, true]) {
                for (const itemWidth of [180, 400])
                    rows.push({tag: name + "-" + dark + "-" + itemWidth,
                        typeName: name, dark: dark, itemWidth: itemWidth})
            }
        }
        return rows
    }

    function test_listClickForwarding() {
        const item = createTemporaryQmlObject("import EUI 1.0\nEList { width: 320; height: 200; "
            + "property var clickedRow: null; property int clickedIndex: -1; "
            + "items: ['Alpha', 'Beta']; onItemClicked: function(row, index) { clickedRow = row; clickedIndex = index } }", testCase)
        verify(item !== null)
        tryVerify(function() { return item.view !== null })
        item.view.itemClicked("Beta", 1)
        compare(item.clickedRow, "Beta")
        compare(item.clickedIndex, 1)
    }

    function test_loadingBlocksWheel() {
        const view = createTemporaryQmlObject("import QtQuick 2.15\nFlickable { width: 240; height: 160; contentHeight: 1000 }", testCase)
        const loading = createTemporaryQmlObject("import EUI 1.0\nELoading {}", view)
        verify(loading !== null)
        loading.target = view
        verify(waitForItemPolished(view))
        mouseWheel(view, 200, 140, 0, -120)
        wait(30)
        compare(view.contentY, 0)
        loading.lock = false
        mouseWheel(view, 200, 140, 0, -120)
        tryVerify(function() { return view.contentY > 0 })
    }

    function test_sampleContrast_data() {
        const rows = []
        for (const name of ["EDeferred", "EAnimateOnScroll", "ERipple", "EDropdown"]) {
            for (let index = 0; index < Samples.definitions[name].cases.length; ++index) {
                const code = Samples.definitions[name].cases[index].code
                if (code.indexOf("Scroll down") < 0 && code.indexOf("Click anywhere") < 0
                    && code.indexOf("Right-click anywhere") < 0)
                    continue
                for (const dark of [false, true])
                    rows.push({tag: name + index + "-" + dark, code: code, dark: dark})
            }
        }
        compare(rows.length, 32)
        return rows
    }

    function test_iftaLabelExternalTargetRestores() {
        const input = createTemporaryQmlObject("import EUI 1.0\nEInput { height: 36; topPadding: 6 }", testCase)
        const label = createTemporaryQmlObject("import EUI 1.0\nEIftaLabel { height: 60 }", testCase)
        label.target = input
        compare(input.height, 60)
        compare(input.topPadding, 18)
        label.fixed = false
        compare(input.height, 36)
        compare(input.topPadding, 6)
        label.fixed = true
        compare(input.height, 60)
        label.destroy()
        wait(0)
        compare(input.height, 36)
        compare(input.topPadding, 6)
        input.destroy()
        wait(0)
    }

    function test_schedulerLanguage(data) {
        const item = createTemporaryQmlObject("import EUI 1.0\nEScheduler {width: 840; height: 580}", testCase)
        item.date = new Date(2026, 9, 12)
        item.view = data.view
        for (const language of ["en", "zh", "en"]) {
            ELocale.language = language
            verify(waitForItemPolished(item))
            const weekday = findChild(item, "schedulerWeekday")
            const dateLabel = findChild(item, "schedulerDayLabel")
            verify(weekday !== null && dateLabel !== null)
            compare(weekday.text, language === "zh" ? "周一" : "Mon")
            if (data.view !== "month")
                verify(dateLabel.text.indexOf(language === "zh" ? "周一" : "Mon") === 0)
        }
        item.destroy()
        wait(0)
    }

    function test_sampleContrast(data) {
        ETheme.setDark(data.dark)
        const item = createTemporaryQmlObject(data.code, testCase)
        verify(item !== null)
        verify(waitForItemPolished(item))
        const pending = [item]
        let checked = 0
        while (pending.length) {
            const child = pending.pop()
            if (["Scroll down ↓", "Click anywhere", "Right-click anywhere"].indexOf(child.text) >= 0) {
                const text = luminance(child.color)
                const surface = luminance(child.parent.color)
                verify((Math.max(text, surface) + 0.05) / (Math.min(text, surface) + 0.05) >= 4.5)
                ++checked
            }
            const children = child.children || []
            for (let index = 0; index < children.length; ++index)
                pending.push(children[index])
        }
        compare(checked, 1)
        item.destroy()
        wait(0)
    }

    function test_tagOptionalAvatar() {
        const item = createTemporaryQmlObject("import EUI 1.0\nETag { text: 'EUI' }", testCase)
        const loader = findChild(item, "tagAvatarLoader")
        verify(loader !== null)
        compare(loader.active, false)
        compare(loader.item, null)
        item.iconSource = EIcons.CHECK
        compare(loader.item, null)
        item.avatarSource = "data:image/svg+xml;base64,PHN2ZyB4bWxucz0naHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmcnIHdpZHRoPScxNicgaGVpZ2h0PScxNic+PHJlY3Qgd2lkdGg9JzE2JyBoZWlnaHQ9JzE2JyBmaWxsPSdibHVlJy8+PC9zdmc+"
        tryVerify(function() { return loader.item !== null })
        tryCompare(loader.item, "loaded", true)
        compare(loader.item.width, item.implicitHeight - 4)
        item.avatarSource = ""
        compare(loader.item, null)
        item.destroy()
        wait(0)
    }

    function test_intrinsicSlots(data) {
        ETheme.setDark(data.dark)
        const fixtures = {
            ESelect: "multiple: true; value: [1]; model: [{label: 'One', value: 1}]; tagDelegate: Component { EText { text: parent.label } }",
            ECheckBoxGroup: "options: [{label: 'One', value: 1}]; optionDelegate: Component { EText { text: parent.label } }",
            EInputTag: "value: ['Alpha']; tagDelegate: Component { EText { text: parent.tag } }",
            ESpinBox: "prefixComponent: Component { EText { text: 'Prefix' } } suffixComponent: Component { EText { text: 'Suffix' } }"
        }
        const item = createTemporaryQmlObject("import EUI 1.0\nimport QtQuick 2.15\n"
            + data.typeName + " { width: " + data.itemWidth + "; " + fixtures[data.typeName] + " }", testCase)
        verify(item !== null)
        verify(waitForItemPolished(item))
        wait(20)
        verify(item.implicitHeight > 0)
        if (data.typeName === "ESpinBox") {
            for (const name of ["spinBoxPrefix", "spinBoxSuffix"]) {
                const loader = findChild(item, name)
                verify(loader !== null && loader.item !== null)
                verify(loader.width > 0 && loader.height > 0)
                compare(loader.height, loader.item.implicitHeight)
            }
        } else {
            verify(item.contentItem.children.length > 0)
        }
        item.width = 120
        verify(waitForItemPolished(item))
        item.enabled = false
        wait(0)
        item.destroy()
        wait(0)
    }

    width: 800
    height: 600
    visible: true
    when: windowShown
    name: "SlotRegressions"
}
