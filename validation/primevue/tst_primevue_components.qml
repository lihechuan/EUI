import EUI 1.0
import "../../example/qml/component/PrimeSamples.js" as Samples
import QtQuick 2.15
import QtTest 1.2
import QtQuick.Window 2.15

TestCase {
    id: testCase

    function test_publicTypes(data) {
        const item = create(data.typeName)
        item.destroy()
    }

    function test_renderShowcase_data() {
        return [{tag: "light", dark: false}, {tag: "dark", dark: true}]
    }

    function test_listStyleStates_data() {
        return [{tag: "light", dark: false}, {tag: "dark", dark: true}]
    }

    function test_samples(data) {
        const item = Qt.createQmlObject(data.code, testCase)
        verify(item !== null)
        wait(0)
        item.destroy()
        wait(0)
    }

    function test_samples_data() {
        const result = []
        Object.keys(Samples.definitions).forEach(function(name) {
            Samples.definitions[name].cases.forEach(function(sample, index) {
                result.push({tag: name + index, code: sample.code})
            })
        })
        return result
    }

    function create(name, properties) {
        const component = Qt.createQmlObject("import EUI 1.0\n" + name + " {}", testCase)
        verify(component !== null, name)
        if (properties)
            Object.keys(properties).forEach(function(key) { component[key] = properties[key] })
        return component
    }

    function test_contextPosition() {
        const dropdown = create("EDropdown", {width: 360, height: 140, trigger: "contextmenu", model: [{label: "Copy", command: "copy"}]})
        mouseClick(dropdown, 120, 70, Qt.RightButton)
        tryCompare(dropdown.menu, "visible", true)
        compare(dropdown.menu.x, 120)
        compare(dropdown.menu.y, 70)
        dropdown.close()
        tryCompare(dropdown.menu, "visible", false)
        verify(!dropdown.openAt(NaN, 5))
        dropdown.destroy()
    }

    function test_publicTypes_data() {
        return ["ELabel","EFloatLabel","EIftaLabel","EInputGroup","EDeferred","EAnimateOnScroll","EFieldset","EInplace","ERipple","EFocusTrap","EKnob","ECompare","EMeterGroup","EListbox","EOrderList","EDock","EMegaMenu","EOrgChart","ETerminal","EDynamicDialog","EDialogService","ESidebar","EChip", "EScheduler", "ETaskBoard", "ETextEditor"].map(function(name) {
            return {tag: name, typeName: name}
        })
    }

    function test_knobAndCompare() {
        const knob = create("EKnob", {from: 10, to: 30, stepSize: 2})
        verify(knob.setValue(25))
        compare(knob.value, 26)
        verify(knob.setValue(100))
        compare(knob.value, 30)
        knob.readOnly = true
        verify(!knob.setValue(12))
        knob.destroy()
        const compareItem = create("ECompare")
        verify(compareItem.setValue(150))
        compare(compareItem.value, 100)
        verify(!compareItem.setValue(NaN))
        compareItem.enabled = false
        verify(!compareItem.setValue(50))
        compareItem.destroy()
    }

    function test_advancedRendering() {
        const frame = createTemporaryObject(renderWindow, null, {width: 880, height: 700})
        const calendar = createTemporaryObject(advancedPanel, frame.contentItem)
        frame.update()
        waitForRendering(calendar)
        wait(50)
        const image = grabImage(calendar)
        if (!image.width)
            skip("The platform plugin cannot capture offscreen images; screenshots run with Qt 6")
        image.save(Qt.resolvedUrl("../../build/primevue-completion/showcase-work.png").toString().replace("file:///", ""))
    }

    function test_renderShowcase(data) {
        ETheme.setDark(data.dark)
        const frame = createTemporaryObject(renderWindow, null, {width: 880, height: 640})
        const panel = createTemporaryObject(renderPanel, frame.contentItem)
        verify(panel !== null)
        frame.update()
        waitForRendering(panel)
        wait(50)
        const image = grabImage(panel)
        if (!image.width)
            skip("The platform plugin cannot capture offscreen images; screenshots run with Qt 6")
        verify(image.width > 0)
        image.save(Qt.resolvedUrl("../../build/primevue-completion/showcase-" + data.tag + ".png").toString().replace("file:///", ""))
        ETheme.setDark(false)
    }

    function test_floatLabels() {
        const floating = Qt.createQmlObject('import EUI 1.0; EFloatLabel {width: 240; text: "Account"; EInput {width: parent.width}}', testCase)
        verify(!floating.raised)
        floating.target.forceActiveFocus()
        verify(floating.raised)
        floating.target.text = "Value"
        verify(floating.filled)
        floating.destroy()
        const fixed = Qt.createQmlObject('import EUI 1.0; EIftaLabel {width: 240; text: "Account"; EInput {width: parent.width; text: "Value"}}', testCase)
        verify(fixed.raised)
        compare(fixed.target.height, fixed.height)
        verify(fixed.target.height - fixed.target.topPadding >= fixed.target.implicitHeight)
        fixed.destroy()
    }

    function test_terminal() {
        const terminal = create("ETerminal")
        verify(!terminal.execute(" "))
        verify(terminal.execute("echo hello"))
        compare(terminal.entries[0].output, "hello")
        terminal.commandHandler = function(command) { return command.toUpperCase() }
        verify(terminal.execute("demo"))
        compare(terminal.entries[1].output, "DEMO")
        terminal.commandHandler = function() { throw new Error("failure") }
        verify(terminal.execute("bad"))
        verify(terminal.entries[2].error)
        verify(!terminal.historyAt(NaN))
        verify(terminal.historyAt(-1))
        compare(terminal.input.text, "bad")
        terminal.clear()
        compare(terminal.entries.length, 0)
        verify(!terminal.busy)
        terminal.destroy()
    }

    function test_richText() {
        const rich = create("ETextEditor", {text: "Hello world"})
        rich.editor.select(0, 5)
        verify(rich.applyFormat("bold", true))
        verify(rich.text.indexOf("font-weight:600") >= 0 || rich.text.indexOf("font-weight:700") >= 0)
        verify(rich.canUndo)
        rich.undo()
        compare(rich.plainText, "Hello world")
        rich.editor.select(0, 5)
        const bold = findChild(rich, "textEditorFormat_bold")
        verify(bold !== null)
        mouseClick(bold)
        verify(rich.text.indexOf("font-weight:600") >= 0 || rich.text.indexOf("font-weight:700") >= 0)
        verify(!rich.applyFormat("link", "javascript:alert(1)"))
        rich.readOnly = true
        verify(!rich.applyFormat("italic", true))
        rich.destroy()
    }

    function test_dragOrderAndTransfer() {
        const order = create("EOrderList", {width: 400, height: 220, model: ["A", "B", "C"]})
        wait(30)
        mousePress(order, 80, 20, Qt.LeftButton)
        mouseMove(order, 80, 45, 20)
        mouseMove(order, 80, 70, 20)
        mouseMove(order, 80, 100, 20)
        mouseRelease(order, 80, 100, Qt.LeftButton)
        tryVerify(function() { return order.model[2] === "A" })
        order.destroy()
        const transfer = create("ETransfer", {width: 600, height: 240, filterable: false,
            draggable: true, data: [{key: 1, label: "A"}, {key: 2, label: "B"}], value: []})
        wait(30)
        mouseDrag(transfer, 70, 65, 380, 0, Qt.LeftButton)
        tryVerify(function() { return transfer.value.indexOf(1) >= 0 })
        transfer.destroy()
    }

    function test_existingEnhancements() {
        const input = create("EInput", {showPassword: true, showPasswordStrength: true, text: "Aa1!1234"})
        compare(input.passwordStrength, 1)
        input.text = "a"
        verify(input.passwordStrength < 0.5)
        input.destroy()
        const transfer = create("ETransfer", {reorderable: true, data: [
            {key: 1, label: "A"}, {key: 2, label: "B"}, {key: 3, label: "C"}], value: [1, 2, 3]})
        verify(transfer.reorder(true, 0, 2))
        compare(transfer.value, [2, 3, 1])
        verify(!transfer.reorder(true, -1, 0))
        transfer.value = [1, 2, 3]
        transfer.checkKey(2, true, true)
        transfer.checkKey(3, true, true)
        verify(transfer.moveSelection(true, -1))
        compare(transfer.value, [2, 3, 1])
        transfer.destroy()
    }

    function test_boardAndSchedulerDrag() {
        const board = create("ETaskBoard", {width: 840, height: 250, tasks: [
            {key: 1, title: "Drag task", status: "todo"}]})
        wait(30)
        mouseDrag(board, 60, 75, 270, 0, Qt.LeftButton)
        tryCompare(board, "tasks", [{key: 1, title: "Drag task", status: "doing"}])
        board.destroy()
        const calendar = create("EScheduler", {width: 700, height: 300, view: "week",
            date: new Date(2026, 9, 12), events: [{key: 1, title: "Review",
                start: new Date(2026, 9, 12, 9), end: new Date(2026, 9, 12, 10)}]})
        wait(30)
        mouseDrag(calendar, 40, 85, 100, 0, Qt.LeftButton)
        tryVerify(function() { return calendar.events[0].start.getDate() === 13 })
        compare(calendar.events[0].start.getHours(), 9)
        calendar.destroy()
    }

    function test_deferredViewport() {
        const viewport = Qt.createQmlObject('import QtQuick 2.15; Flickable {width: 300; height: 150; contentHeight: 600; clip: true}', testCase)
        const deferred = Qt.createQmlObject('import EUI 1.0; EDeferred {width: 200; height: 80; y: 350; once: false; EText {text: "Deferred"}}', viewport.contentItem)
        wait(0)
        verify(!deferred.inViewport)
        compare(deferred.item, null)
        viewport.contentY = 300
        tryCompare(deferred, "inViewport", true)
        tryVerify(function() { return deferred.item !== null })
        viewport.contentY = 0
        tryCompare(deferred, "item", null)
        deferred.once = true
        viewport.contentY = 300
        tryVerify(function() { return deferred.item !== null })
        viewport.contentY = 0
        wait(0)
        verify(deferred.item !== null)
        viewport.destroy()
    }

    function test_focusTrap() {
        const definition = Qt.createComponent("../../source/qt5/imports/eui/controls/basic/EFocusTrap.qml")
        compare(definition.status, Component.Ready, definition.errorString())
        const trap = createTemporaryObject(definition, testCase, {width: 400, height: 100})
        const first = Qt.createQmlObject('import EUI 1.0; EInput {width: 120}', trap)
        const second = Qt.createQmlObject('import EUI 1.0; EButton {x: 140; text: "Next"}', trap)
        first.forceActiveFocus()
        verify(trap.focusNext(false))
        verify(second.activeFocus)
        verify(trap.focusNext(false))
        verify(first.activeFocus)
        verify(trap.focusNext(true))
        verify(second.activeFocus)
        keyClick(Qt.Key_Tab)
        tryCompare(first, "activeFocus", true)
        keyClick(Qt.Key_Tab, Qt.ShiftModifier)
        tryCompare(second, "activeFocus", true)
    }

    function test_compareRendering() {
        const frame = createTemporaryObject(renderWindow, null, {width: 400, height: 180})
        const item = Qt.createQmlObject('import EUI 1.0; import QtQuick 2.15; ECompare {width: 400; height: 180; beforeComponent: Rectangle {color: "red"} afterComponent: Rectangle {color: "blue"}}', frame.contentItem)
        frame.update()
        waitForRendering(frame.contentItem)
        const image = grabImage(item)
        if (!image.width)
            skip("The platform plugin cannot capture offscreen images; pixel checks run with Qt 6")
        compare(image.red(40, 40), 255)
        compare(image.blue(360, 40), 255)
        mouseClick(item, 300, 80)
        compare(item.value, 75)
        item.vertical = true
        item.value = 50
        waitForRendering(item)
        const vertical = grabImage(item)
        compare(vertical.red(40, 20), 255)
        compare(vertical.blue(40, 160), 255)
        item.destroy()
    }

    function test_orgChartAndMeters() {
        const chart = create("EOrgChart", {model: [{key: "root", label: "Root", children: [
            {key: "a", label: "A"}, {key: "b", label: "B"}]}]})
        compare(chart.layout.nodes.length, 3)
        verify(chart.select("a"))
        compare(chart.selectedKeys, ["a"])
        verify(chart.toggle("root"))
        compare(chart.layout.nodes.length, 1)
        verify(chart.toggle("root"))
        chart.selectionMode = "checkbox"
        verify(chart.select("root"))
        verify(chart.selectedKeys.indexOf("b") >= 0)
        chart.model = [{key: "x", children: [{key: "x"}]}]
        compare(chart.errorString, "duplicate-key")
        chart.destroy()
        const meter = create("EMeterGroup", {values: [{label: "A", value: 80}, {label: "B", value: 50}]})
        compare(meter.total, 130)
        compare(meter.fractionAt(0), 0.8)
        compare(meter.fractionAt(1), 0.2)
        meter.maximum = 0
        compare(meter.fractionAt(0), 0)
        meter.destroy()
    }

    function test_rippleFieldsetAndInplace() {
        const ripple = create("ERipple", {width: 200, height: 80})
        let triggered = false
        ripple.triggered.connect(function() { triggered = true })
        mouseClick(ripple, 60, 30)
        verify(triggered)
        ripple.enabled = false
        verify(!ripple.pulse(20, 20))
        ripple.destroy()
        const fieldset = create("EFieldset", {collapsible: true, title: "Fields"})
        const toggle = findChild(fieldset, "fieldsetToggle")
        mouseClick(toggle)
        verify(fieldset.collapsed)
        fieldset.enabled = false
        verify(!fieldset.toggle())
        fieldset.destroy()
        const inplace = Qt.createQmlObject('import EUI 1.0; EInplace {width: 300; text: "Edit"; EInput {text: "Value"}}', testCase)
        verify(inplace.open())
        tryVerify(function() { return inplace.item !== null })
        inplace.item.forceActiveFocus()
        keyClick(Qt.Key_Escape)
        tryCompare(inplace, "active", false)
        inplace.destroy()
    }

    function test_listboxAndOrdering() {
        const list = create("EListbox", {model: [{label: "One", value: 1},
            {label: "Two", value: 2, disabled: true}, {label: "Three", value: 3}], multiple: true})
        verify(!list.navigate(NaN))
        verify(!list.navigate(-100))
        verify(list.select(0))
        compare(list.values, [1])
        verify(!list.select(1))
        verify(list.select(2))
        compare(list.values, [1, 3])
        list.filterText = "Three"
        compare(list.items.length, 1)
        verify(list.select(0))
        compare(list.values, [1])
        list.destroy()
        const order = create("EOrderList", {model: ["A", "B", "C", "D"], values: ["B", "C"]})
        verify(order.moveSelected("up"))
        compare(order.model, ["B", "C", "A", "D"])
        verify(order.moveSelected("bottom"))
        compare(order.model, ["A", "D", "B", "C"])
        verify(order.moveItem(3, 0))
        compare(order.model, ["C", "A", "D", "B"])
        verify(!order.moveItem(-1, 0))
        wait(0)
        mouseClick(order, 80, 20)
        compare(order.values, ["B"])
        order.destroy()
    }

    function test_imageAndLabel() {
        const item = Qt.createQmlObject(Samples.definitions.EImage.cases[0].code, testCase)
        tryCompare(item, "status", 1)
        verify(item.openPreview())
        tryCompare(item, "previewVisible", true)
        verify(item.flipPreview(false))
        verify(item.previewFlippedHorizontal)
        verify(item.flipPreview(true))
        verify(item.previewFlippedVertical)
        item.resetPreview()
        verify(!item.previewFlippedHorizontal)
        verify(!item.previewFlippedVertical)
        let saved = false
        item.downloaded.connect(function() { saved = true })
        verify(item.downloadPreview(Qt.resolvedUrl("../../build/primevue-completion/saved-preview.svg")))
        verify(saved)
        item.closePreview()
        item.destroy()
        wait(0)
        const field = create("EInput", {width: 200})
        const label = create("ELabel", {target: field, text: "Account"})
        verify(label.activate())
        verify(field.activeFocus)
        compare(field.Accessible.name, "Account")
        field.enabled = false
        verify(!label.activate())
        label.destroy()
        field.destroy()
    }

    function test_dynamicDialogLifecycle() {
        const service = create("EDialogService")
        const definition = Qt.createComponent("../../source/qt5/imports/eui/controls/indicator/EDynamicDialog.qml")
        compare(definition.status, Component.Ready, definition.errorString())
        let result = null
        const dialog = service.open(dialogContent, {message: "Injected"}, {
            title: "Dynamic", onClose: function(value, accepted) { result = {value: value, accepted: accepted} }
        })
        verify(dialog !== null)
        tryCompare(dialog, "opened", true)
        verify(dialog.instance !== null)
        compare(dialog.instance.message, "Injected")
        compare(dialog.instance.dialogRef, dialog)
        dialog.resolve({saved: true})
        tryVerify(function() { return result !== null })
        verify(result.accepted)
        verify(result.value.saved)
        tryCompare(service, "dialogs", [])
        const second = service.open(dialogContent, {message: "Cancel"})
        tryCompare(second, "opened", true)
        service.closeAll()
        tryCompare(service, "dialogs", [])
        service.destroy()
        definition.destroy()
    }

    function test_listStyleStates(data) {
        ETheme.setDark(data.dark)
        const frame = createTemporaryObject(renderWindow, null, {width: 440, height: 260})
        const order = Qt.createQmlObject('import EUI 1.0; EOrderList {width: 420; height: 240; model: ["One", "Two", "Three"]; values: ["One", "Two"]}', frame.contentItem)
        frame.update()
        waitForRendering(order)
        const image = grabImage(order)
        mouseClick(order, 28, 28)
        compare(order.values, ["Two"])
        verify(order.view.currentIndex === 0)
        keyClick(Qt.Key_Down)
        compare(order.view.currentIndex, 1)
        keyClick(Qt.Key_Space)
        compare(order.values, [])
        order.enabled = false
        mouseClick(order, 28, 28)
        compare(order.values, [])
        order.destroy()
        ETheme.setDark(false)
        if (!image.width)
            skip("The platform plugin cannot capture offscreen images; style pixels run with Qt 6")
        verify(Math.abs(image.red(200, 28) - image.red(200, 220)) < 90)
        verify(Math.abs(image.green(200, 28) - image.green(200, 220)) < 90)
        verify(Math.abs(image.blue(200, 28) - image.blue(200, 220)) < 90)
    }

    function test_schedulerAndBoard() {
        const scheduler = create("EScheduler", {date: new Date(2026, 0, 31), events: [
            {key: 1, title: "Review", start: new Date(2026, 0, 31, 9), end: new Date(2026, 0, 31, 10)}]})
        compare(scheduler.visibleDays.length, 42)
        verify(scheduler.navigate(1))
        compare(scheduler.date.getMonth(), 1)
        compare(scheduler.date.getDate(), 28)
        verify(scheduler.moveEvent(1, new Date(2026, 1, 1, 11)))
        compare(scheduler.events[0].end.getHours(), 12)
        verify(!scheduler.resizeEvent(1, new Date(2026, 1, 1, 10)))
        scheduler.readOnly = true
        verify(!scheduler.moveEvent(1, new Date()))
        scheduler.destroy()
        const board = create("ETaskBoard", {tasks: [{key: 1, title: "A", status: "todo"},
            {key: 2, title: "B", status: "doing"}]})
        verify(board.moveTask(1, "doing", 0))
        compare(board.tasksFor("doing")[0].key, 1)
        compare(board.tasks[0].status, "doing")
        verify(!board.moveTask(1, "unknown"))
        board.beforeMove = function() { return false }
        verify(!board.moveTask(1, "done"))
        board.tasks = [{key: 1, title: "A", status: "todo"}, {key: 1, title: "B", status: "todo"}]
        compare(board.errorString, "invalid-task-key-or-status")
        board.destroy()
    }

    width: 900
    height: 700
    visible: true
    when: windowShown
    name: "PrimeVueCompletion"

    Component {
        id: advancedPanel

        Rectangle {
            width: 880
            height: 700
            color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)

            EScheduler {
                x: 20
                y: 20
                width: 840
                height: 380
                date: new Date(2026, 9, 15)
                events: [{key: "review", title: "Review", start: new Date(2026, 9, 15, 9),
                    end: new Date(2026, 9, 15, 10)}, {key: "release", title: "Release", allDay: true,
                    start: new Date(2026, 9, 16), end: new Date(2026, 9, 18), color: "#168b77"}]
            }
            ETextEditor {
                x: 20
                y: 430
                width: 840
                height: 250
                text: "<h3>EUI rich text</h3><p>Select text to apply <b>formatting</b>, lists and alignment.</p>"
            }
        }
    }
    Component {
        id: renderWindow

        Window {
            visible: true
        }
    }
    Component {
        id: renderPanel

        Rectangle {
            width: 880
            height: 640
            color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)

            EKnob {
                x: 20
                y: 20
                value: 72
                suffix: "%"
            }
            EFloatLabel {
                y: 30
                x: 170
                width: 250
                text: "Account"

                EInput {
                    text: "EUI"
                    width: parent.width
                }
            }
            EIftaLabel {
                x: 170
                y: 100
                width: 250
                text: "Email"

                EInput {
                    width: parent.width
                    text: "hello@eui.dev"
                }
            }
            EMeterGroup {
                y: 40
                x: 470
                width: 360
                values: [{label: "Design", value: 40, color: "#2563eb"},
                    {label: "Development", value: 30, color: "#168b77"}]
            }
            EOrgChart {
                x: 20
                y: 180
                width: 400
                height: 190
                model: [{key: "team", label: "EUI", children: [{key: "design", label: "Design"},
                    {key: "dev", label: "Development"}]}]
            }
            ECompare {
                x: 460
                y: 160
                width: 400
                height: 180
                afterComponent: Rectangle {
                    color: "#168b77"

                    EText {
                        text: "After"
                        color: "white"
                        anchors.centerIn: parent
                    }
                }
                beforeComponent: Rectangle {
                    color: "#2563eb"

                    EText {
                        color: "white"
                        text: "Before"
                        anchors.centerIn: parent
                    }
                }
            }
            ETaskBoard {
                x: 20
                y: 390
                width: 840
                height: 230
                tasks: [{key: 1, title: "Build components", status: "todo"},
                    {key: 2, title: "Verify interactions", status: "doing"},
                    {key: 3, title: "Document API", status: "done"}]
            }
        }
    }
    Component {
        id: dialogContent

        Item {
            property string message: ""
            property var dialogRef: null

            implicitHeight: 120
        }
    }
}
