import EUI 1.0
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function test_itemReentrantCancellation_data() {
        return test_reentrantCancellation_data()
    }

    function test_reentrantCancellation_data() {
        return ["validate", "clear", "reset"].map(function(action) { return {tag: action, action: action} })
    }

    function test_cancellation_data() {
        return ["change", "validateItem", "clearItem", "resetItem", "destroyItem",
            "clearForm", "resetForm", "validateForm"].map(function(action) {
                return {tag: action, action: action}
            })
    }

    function test_modelPaths_data() {
        return [
            {tag: "literal", model: {"user.name": "Alice"}, path: "user.name"},
            {tag: "nested", model: {user: {name: "Alice"}}, path: "user.name"},
            {tag: "array", model: {users: [{name: "Alice"}]}, path: ["users", 0, "name"]},
            {tag: "precedence", model: {"user.name": "Alice", user: {name: "Nested"}}, path: "user.name"}
        ]
    }

    function test_invalidPaths() {
        const form = createTemporaryObject(formComponent, testCase)
        form.model = {}
        for (const path of [null, undefined, "", []]) {
            compare(form.modelValue(path), undefined)
            compare(form.setModelValue(path, "Bob"), false)
        }
        compare(Object.keys(form.model).length, 0)
        form.model = {user: 42}
        compare(form.setModelValue("user.name", "Bob"), false)
        compare(form.model.user, 42)
    }

    function test_cancelledSubmit() {
        const form = createTemporaryObject(formComponent, testCase)
        wait(0)
        form.replies = []
        const results = []
        form.submitted.connect(function(valid) { results.push(valid) })
        compare(form.submit(), undefined)
        form.input.text = "Bob"
        wait(0)
        compare(form.replies.length, 2)
        form.replies[0](true)
        form.replies[1](true)
        compare(results, [false])
        compare(form.validating, false)
        compare(form.item.validationState, EValidationState.None)
    }

    function test_asyncValidation() {
        const form = createTemporaryObject(formComponent, testCase)
        wait(0)
        form.replies = []
        const results = []
        compare(form.validate(function(valid) { results.push(valid) }), undefined)
        verify(form.validating)
        compare(form.replies.length, 1)
        form.replies[0](true)
        compare(results, [true])
        compare(form.validating, false)
        compare(form.valid, true)
        compare(form.firstInvalidItem, null)
        form.replies[0](false)
        compare(results, [true])
        compare(form.valid, true)
    }

    function test_completedFieldRemovalDuringValidation() {
        const form = createTemporaryObject(formComponent, testCase)
        const completedItem = createTemporaryQmlObject("import EUI 1.0\nEFormItem { prop: 'other'; "
            + "EInput { text: 'First' } }", form.item.parent)
        wait(0)
        form.replies = []
        const results = []
        compare(form.validate(function(valid) { results.push(valid) }), undefined)
        completedItem.destroy()
        wait(0)
        form.replies[0](true)
        compare(results, [false])
        compare(form.validating, false)
    }

    function test_completedFieldChangesDuringValidation() {
        const form = createTemporaryObject(formComponent, testCase)
        const completedItem = createTemporaryQmlObject("import EUI 1.0\nEFormItem { prop: 'other'; "
            + "EInput { text: 'First' } }", form.item.parent)
        wait(0)
        form.replies = []
        const results = []
        compare(form.validate(function(valid) { results.push(valid) }), undefined)
        completedItem.field.text = "Changed"
        form.replies[0](true)
        compare(results, [false])
        compare(form.validating, false)
    }

    function test_modelPaths(data) {
        const form = createTemporaryObject(formComponent, testCase)
        form.model = data.model
        compare(form.modelValue(data.path), "Alice")
        verify(form.setModelValue(data.path, "Bob"))
        compare(form.modelValue(data.path), "Bob")
        if (data.tag === "literal")
            compare(form.model.user, undefined)
        if (data.tag === "precedence")
            compare(form.model.user.name, "Nested")
        form.model = {}
        verify(form.setModelValue("users.0.name", "Created"))
        compare(form.model.users[0].name, "Created")
        verify(EDataUtils.isArray(form.model.users))
    }

    function test_reentrantCancellation(data) {
        const form = createTemporaryObject(formComponent, testCase)
        wait(0)
        form.replies = []
        const results = []
        form.validate(function(valid) {
            results.push(valid)
            form.validate(function(nestedValid) { results.push(nestedValid) })
        })
        if (data.action === "clear")
            form.clearValidate()
        else if (data.action === "reset")
            form.resetFields()
        else
            form.validate(function(valid) { results.push(valid) })
        const cancelled = data.action === "validate" ? [false, false] : [false]
        compare(results, cancelled)
        verify(form.validating)
        compare(form.replies.length, 2)
        form.replies[0](true)
        compare(results, cancelled)
        form.replies[1](true)
        compare(results, cancelled.concat([true]))
        compare(form.validating, false)
    }

    function test_itemReentrantCancellation(data) {
        const form = createTemporaryObject(formComponent, testCase)
        wait(0)
        form.replies = []
        const results = []
        form.item.validate(function(valid) {
            results.push(valid)
            form.item.validate(function(nestedValid) { results.push(nestedValid) })
        })
        if (data.action === "clear")
            form.item.clearValidate()
        else if (data.action === "reset")
            form.item.resetField()
        else
            form.item.validate(function(valid) { results.push(valid) })
        const cancelled = data.action === "validate" ? [false, false] : [false]
        compare(results, cancelled)
        verify(form.item.validating)
        compare(form.replies.length, 2)
        form.replies[0](true)
        compare(results, cancelled)
        form.replies[1](true)
        compare(results, cancelled.concat([true]))
        compare(form.item.validating, false)
    }

    function test_cancellation(data) {
        const form = createTemporaryObject(formComponent, testCase)
        wait(0)
        form.replies = []
        const results = []
        compare(form.validate(function(valid) { results.push(valid) }), undefined)
        compare(form.replies.length, 1)
        verify(form.validating)
        const stale = form.replies[0]
        if (data.action === "change")
            form.input.text = "Bob"
        else if (data.action === "validateItem")
            form.item.validate("change")
        else if (data.action === "clearItem")
            form.item.clearValidate()
        else if (data.action === "resetItem")
            form.item.resetField()
        else if (data.action === "destroyItem")
            form.item.destroy()
        else if (data.action === "clearForm")
            form.clearValidate()
        else if (data.action === "resetForm")
            form.resetFields()
        else
            form.validate(function(valid) { results.push(valid) })
        wait(0)
        compare(results, [false])
        stale(true)
        compare(results, [false])
        if (form.replies.length > 1)
            form.replies[1](true)
        compare(form.validating, false)
        compare(form.firstInvalidItem, null)
        compare(results, data.action === "validateForm" ? [false, true] : [false])
    }

    width: 640
    height: 480
    visible: true
    when: windowShown
    name: "FormRegressions"

    Component {
        id: formComponent

        EForm {
            id: form

            property var replies: []
            property alias item: fieldItem
            property alias input: fieldInput

            width: 400
            validateOnChange: true

            EFormItem {
                id: fieldItem

                prop: "user.name"
                rules: [{validator: function(rule, value, done) { form.replies.push(done) }}]

                EInput {
                    id: fieldInput

                    text: "Alice"
                }
            }
        }
    }
}
