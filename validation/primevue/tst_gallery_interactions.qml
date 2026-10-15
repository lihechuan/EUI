import EUI 1.0
import "../../example/qml/component/PrimeSamples.js" as Samples
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function test_boardDragFeedback_data() {
        return [{tag: "default-card", index: 0}, {tag: "custom-card", index: 3}]
    }

    function test_customInputGroupSpacing_data() {
        return [0, 4, 16, 32, 48].map(function(spacing) { return {tag: String(spacing), spacing} })
    }

    function test_fieldsets_data() {
        return Samples.definitions.EFieldset.cases.map(function(sample, index) {
            return {tag: String(index), code: sample.code}
        })
    }

    function test_inputGroupSpacing_data() {
        return Samples.definitions.EInputGroup.cases.slice(0, 5).map(function(sample, index) {
            return {tag: String(index), index, code: sample.code}
        })
    }

    function descendants(root) {
        const items = [root]
        for (let index = 0; index < items.length; ++index) {
            const children = items[index].children || []
            for (let child = 0; child < children.length; ++child)
                items.push(children[child])
        }
        return items
    }

    function test_dockBounds_data() {
        const rows = []
        for (const placement of ["top", "bottom", "left", "right"]) {
            for (const magnification of [1, 1.35, 2])
                rows.push({tag: placement + "-" + magnification, placement, magnification})
        }
        return rows
    }

    function test_dialogCallbacks_data() {
        const result = []
        for (const name of ["EDialogService", "EDynamicDialog", "EDialog"])
            Samples.definitions[name].cases.forEach(function(sample, index) {
                result.push({tag: name + index, code: sample.code})
            })
        return result
    }

    function test_roundedMeterPixels_data() {
        return [{tag: "horizontal-light", vertical: false, dark: false},
            {tag: "horizontal-dark", vertical: false, dark: true},
            {tag: "vertical-light", vertical: true, dark: false},
            {tag: "vertical-dark", vertical: true, dark: true}]
    }

    function test_boardRejectedDrag_data() {
        return [{tag: "outside", kind: "outside"}, {tag: "limit", kind: "limit"},
            {tag: "callback", kind: "callback"}, {tag: "disabled-column", kind: "disabled-column"},
            {tag: "read-only", kind: "read-only"}, {tag: "disabled-task", kind: "disabled-task"}]
    }

    function test_commandFeedback_data() {
        return [
            {tag: "dock", code: Samples.definitions.EDock.cases[4].code, kind: "dock"},
            {tag: "menu", code: Samples.definitions.EMegaMenu.cases[3].code, kind: "menu"},
            {tag: "scheduler", code: Samples.definitions.EScheduler.cases[3].code, kind: "scheduler"}]
    }

    function findObject(root, propertyName) {
        const pending = [root]
        const visited = []
        while (pending.length) {
            const item = pending.pop()
            if (!item || visited.indexOf(item) >= 0)
                continue
            visited.push(item)
            if (item[propertyName] !== undefined)
                return item
            const children = item.data || item.children || []
            for (let index = 0; index < children.length; ++index)
                pending.push(children[index])
        }
        return null
    }

    function test_commandFeedback(data) {
        const sample = createTemporaryQmlObject(data.code, host)
        if (data.kind === "dock")
            verify(sample.trigger(0))
        else if (data.kind === "menu")
            verify(sample.trigger({label: "Code", command: "code"}))
        else
            sample.eventClicked(sample.events[0])
        const feedback = findChild(sample, "sampleFeedback")
        verify(feedback !== null)
        tryCompare(feedback, "count", 1)
        if (data.kind === "scheduler") {
            sample.slotClicked(new Date(2026, 9, 15))
            tryCompare(feedback, "count", 2)
        }
        feedback.closeAll()
        tryCompare(feedback, "count", 0)
        sample.destroy()
        wait(0)
    }

    function test_fieldsets(data) {
        const fieldset = createTemporaryQmlObject(data.code, host)
        verify(fieldset !== null)
        wait(0)
        const toggle = findChild(fieldset, "fieldsetToggle")
        verify(toggle !== null && toggle.visible, "The legend must remain visible when collapsed")
        compare(toggle.parent, fieldset)
        const originalHeight = fieldset.height
        if (fieldset.collapsible) {
            verify(fieldset.toggle())
            wait(0)
            verify(fieldset.toggle())
            wait(0)
            compare(fieldset.height, originalHeight)
        }
        fieldset.width = 480
        wait(0)
        verify(isFinite(fieldset.height) && fieldset.height > 0)
        fieldset.destroy()
        wait(0)
    }

    function test_boardSameColumnAndCancel() {
        const sample = createTemporaryQmlObject(Samples.definitions.ETaskBoard.cases[0].code, host)
        verify(waitForItemPolished(sample))
        const preview = findChild(sample, "taskBoardDragPreview")
        mousePress(sample, 70, 170)
        mouseMove(sample, 70, 70, 30)
        verify(preview.visible)
        mouseRelease(sample, 70, 70)
        tryCompare(preview, "visible", false)
        compare(sample.tasksFor("todo")[0].key, 3)
        const original = JSON.stringify(sample.tasks)
        mousePress(sample, 70, 90)
        mouseMove(sample, 110, 120, 30)
        verify(preview.visible)
        sample.readOnly = true
        tryCompare(preview, "visible", false)
        mouseRelease(sample, 350, 120)
        compare(JSON.stringify(sample.tasks), original)
        sample.destroy()
        wait(0)
    }

    function test_customInputGroupSpacing(data) {
        const sample = createTemporaryQmlObject(Samples.definitions.EInputGroup.cases[5].code, host)
        const group = findChild(sample, "customSpacingInputGroup")
        const editor = findChild(sample, "inputGroupSpacingControl")
        verify(group !== null && editor !== null)
        editor.value = data.spacing
        verify(waitForItemPolished(sample))
        compare(group.spacing, data.spacing)
        const fields = group.children[0].children
        for (const width of [320, 480]) {
            sample.width = width
            verify(waitForItemPolished(sample))
            fuzzyCompare(fields[1].x - fields[0].x - fields[0].width, data.spacing, 0.5)
            verify(fields[0].width > 0 && fields[1].width > 0)
            verify(fields[1].x + fields[1].width <= width + 0.5)
        }
        sample.destroy()
        wait(0)
    }

    function test_dialogCallbacks(data) {
        const sample = createTemporaryQmlObject(data.code, host)
        const trigger = sample.children[0]
        trigger.clicked()
        const service = findObject(sample, "dialogs")
        const dialog = service ? service.dialogs[0] : findObject(sample, "openComponent")
        verify(dialog !== null)
        tryCompare(dialog, "opened", true)
        verify(dialog.instance !== null)
        const action = dialog.instance.children[1]
        action.clicked()
        const feedback = findChild(sample, "sampleFeedback")
        verify(feedback !== null, "The example must show the result through an EMessage instance")
        tryCompare(feedback, "count", 1)
        feedback.closeAll()
        tryCompare(feedback, "count", 0)
        if (service)
            tryCompare(service, "dialogs", [])
        else
            tryCompare(dialog, "visible", false)
        sample.destroy()
        wait(0)
    }

    function test_circularMeterPixels() {
        const sample = createTemporaryQmlObject('import EUI 1.0; EMeterGroup {width: 360; diameter: 180; circular: true; '
            + 'values: [{value: 40, color: "#ff0000"}, {value: 25, color: "#0000ff"}]}', host)
        const ring = findChild(sample, "meterGroupRing")
        verify(ring !== null)
        verify(waitForItemPolished(sample))
        wait(100)
        const image = grabImage(ring)
        if (!image.width)
            skip("This platform cannot capture pixels; run the same check on the isolated Windows desktop")
        verify(image.red(173, 90) > 240 && image.blue(173, 90) < 15)
        verify(image.blue(90, 173) > 240 && image.red(90, 173) < 15)
        sample.values = [{value: 150, color: "#ff0000"}]
        wait(100)
        const updated = grabImage(ring)
        verify(updated.red(7, 90) > 240 && updated.blue(7, 90) < 15)
        sample.destroy()
        wait(0)
    }

    function test_boardDragFeedback(data) {
        const sample = createTemporaryQmlObject(Samples.definitions.ETaskBoard.cases[data.index].code, host)
        verify(waitForItemPolished(sample))
        const preview = findChild(sample, "taskBoardDragPreview")
        verify(preview !== null, "Dragging needs a floating card preview")
        verify(!preview.visible)
        mousePress(sample, 70, 90)
        mouseMove(sample, 95, 90, 30)
        tryCompare(preview, "visible", true)
        const firstX = preview.x
        mouseMove(sample, 350, 120, 30)
        verify(preview.x > firstX + 200)
        const columns = descendants(sample).filter(function(item) { return item.objectName === "taskBoardColumn" })
        compare(columns.length, 3)
        verify(columns[1].dropHovered)
        mouseRelease(sample, 350, 120)
        tryCompare(preview, "visible", false)
        compare(sample.tasks.filter(function(task) { return task.key === 1 })[0].status, "doing")
        sample.destroy()
        wait(0)
    }

    function test_circularMeters() {
        const sample = createTemporaryQmlObject('import EUI 1.0; EMeterGroup {width: 360; diameter: 180; circular: true; '
            + 'values: [{label: "A", value: 40, color: "#ff0000"}, {label: "B", value: 25, color: "#0000ff"}]}', host)
        const ring = findChild(sample, "meterGroupRing")
        verify(ring !== null && ring.visible)
        verify(waitForItemPolished(sample))
        compare(ring.width, 180)
        compare(ring.height, 180)
        compare(sample.total, 65)
        sample.values = [{value: 80}, {value: 50}, {value: -1}, {value: Infinity}, null]
        compare(sample.total, 130)
        compare(sample.fractionAt(1), 0.2)
        sample.labelsOnTop = true
        sample.vertical = true
        verify(waitForItemPolished(sample))
        verify(ring.y > 0)
        sample.width = 100
        tryCompare(ring, "width", 100)
        sample.maximum = 0
        compare(sample.fractionAt(0), 0)
        sample.values = null
        compare(sample.total, 0)
        sample.destroy()
        wait(0)
    }

    function test_inputGroupSpacing(data) {
        const sample = createTemporaryQmlObject(data.code, host)
        verify(waitForItemPolished(sample))
        const fields = []
        const children = sample.children[0].children
        for (let index = 0; index < children.length; ++index) {
            if (children[index].implicitHeight > 0)
                fields.push(children[index])
        }
        compare(fields.length, 2)
        verify(fields[1].x - fields[0].x - fields[0].width >= 8,
            "Grouped fields must leave enough space for both borders and focus states")
        if (data.index === 1)
            compare(fields[0].width, 120)
        for (const field of fields) {
            mouseClick(field, 20, field.height / 2)
            verify(fields[1].x >= fields[0].x + fields[0].width + 8)
        }
        for (const width of [320, 480]) {
            sample.width = width
            verify(waitForItemPolished(sample))
            verify(fields[1].x >= fields[0].x + fields[0].width + 8)
            verify(fields[1].x + fields[1].width <= width + 0.5)
        }
        sample.destroy()
        wait(0)
    }

    function test_dockBounds(data) {
        const sample = createTemporaryQmlObject(Samples.definitions.EDock.cases[0].code, host)
        sample.placement = data.placement
        sample.magnification = data.magnification
        verify(waitForItemPolished(sample))
        const actions = descendants(sample).filter(function(item) {
            return typeof item.clicked === "function" && item.iconSize === sample.iconSize
        })
        compare(actions.length, 3)
        for (const action of actions) {
            action.hoverEnabled = true
            mouseMove(host, host.width - 2, host.height - 2)
            mouseMove(action, action.width / 2, action.height / 2, 30)
            tryCompare(action, "hovered", true)
            wait(150)
            const top = action.mapToItem(sample, 0, 0)
            const bottom = action.mapToItem(sample, action.width, action.height)
            verify(top.x >= 7.5 && top.y >= 7.5 && bottom.x <= sample.width - 7.5
                && bottom.y <= sample.height - 7.5, "Magnified items must stay inside the dock border")
        }
        sample.destroy()
        wait(0)
    }

    function test_roundedMeterPixels(data) {
        ETheme.setDark(data.dark)
        const frame = createTemporaryQmlObject('import QtQuick 2.15; Rectangle {width: 260; height: 200; color: "#102030"}', host)
        const sample = createTemporaryQmlObject('import EUI 1.0; EMeterGroup {width: 240; barSize: 24; '
            + 'values: [{value: 40, color: "#67c23a"}, {value: 35, color: "#f56c6c"}, '
            + '{value: 25, color: "#409eff"}]}', frame)
        sample.x = 8
        sample.y = 8
        sample.vertical = data.vertical
        if (data.vertical)
            sample.height = 180
        verify(waitForItemPolished(sample))
        wait(150)
        const image = grabImage(frame)
        if (!image.width)
            skip("This platform cannot capture pixels; run the same check on the isolated Windows desktop")
        verify(image.red(8, 8) === 16 && image.green(8, 8) === 32 && image.blue(8, 8) === 48,
            "The first segment must follow the rounded outer corner")
        verify(image.red(data.vertical ? 31 : 247, data.vertical ? 187 : 31) === 16,
            "The last segment must follow the rounded outer corner")
        verify(image.green(data.vertical ? 20 : 8, data.vertical ? 8 : 20) > 180)
        verify(image.green(data.vertical ? 20 : 28, data.vertical ? 28 : 20) > 180)
        sample.values = [{value: 0, color: "#909399"}, {value: 1, color: "#e6a23c"},
            {value: 98, color: "#f56c6c"}, {value: 1, color: "#409eff"}]
        wait(150)
        const narrow = grabImage(frame)
        compare(narrow.red(8, 8), 16)
        compare(narrow.red(data.vertical ? 31 : 247, data.vertical ? 187 : 31), 16)
        verify(narrow.red(data.vertical ? 20 : 128, data.vertical ? 98 : 20) > 230)
        frame.destroy()
        wait(0)
        ETheme.setDark(false)
    }

    function test_boardRejectedDrag(data) {
        const sample = createTemporaryQmlObject(Samples.definitions.ETaskBoard.cases[0].code, host)
        if (data.kind === "limit")
            sample.columns = [{key: "todo", title: "Todo"}, {key: "doing", title: "Doing", limit: 1}]
        else if (data.kind === "callback")
            sample.beforeMove = function() { return false }
        else if (data.kind === "disabled-column")
            sample.columns = [{key: "todo", title: "Todo"}, {key: "doing", title: "Doing", disabled: true}]
        else if (data.kind === "read-only")
            sample.readOnly = true
        else if (data.kind === "disabled-task")
            sample.tasks = [{key: 1, title: "Locked", status: "todo", disabled: true}]
        if (data.kind === "limit" || data.kind === "disabled-column")
            sample.tasks = sample.tasks.filter(function(task) { return task.status !== "done" })
        wait(0)
        verify(waitForItemPolished(sample))
        const original = sample.tasks
        const preview = findChild(sample, "taskBoardDragPreview")
        verify(preview !== null)
        const source = descendants(sample).filter(function(item) {
            return item.taskData && item.taskData.key === 1 && typeof item.clicked === "function"
        })[0]
        verify(source !== undefined)
        mousePress(source, source.width / 2, source.height / 2)
        mouseMove(sample, 350, 120, 30)
        if (data.kind === "read-only" || data.kind === "disabled-task")
            verify(!preview.visible)
        else
            verify(preview.visible)
        if (data.kind === "limit" || data.kind === "disabled-column") {
            const destination = descendants(sample).filter(function(item) {
                return item.objectName === "taskBoardColumn" && item.entry.key === "doing"
            })[0]
            verify(!destination.dropHovered)
        }
        mouseRelease(sample, 350, data.kind === "outside" ? -20 : 120)
        tryCompare(preview, "visible", false)
        compare(JSON.stringify(sample.tasks), JSON.stringify(original))
        sample.destroy()
        wait(0)
    }

    width: 800
    height: 600
    visible: true
    when: windowShown
    name: "GalleryInteractions"

    Item {
        id: host

        anchors.fill: parent
    }
}
