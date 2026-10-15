import EUI 1.0
import "PublicTypes.js" as Inventory
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function cleanup() {
        ETheme.setDark(false)
        ELocale.language = "zh"
    }

    function test_animation_data() {
        return EAnimation.effects.map(function(effect) { return {tag: effect, effect: effect} })
    }

    function test_emptyModels_data() {
        return Object.keys(Inventory.models).map(function(name) { return {tag: name, typeName: name} })
    }

    function test_emptyModels(data) {
        const item = createTemporaryQmlObject("import EUI 1.0\n" + data.typeName + " {}", testCase)
        verify(item !== null)
        for (const name of Inventory.models[data.typeName]) {
            const original = item[name]
            item[name] = null
            wait(0)
            item[name] = []
            wait(0)
            item[name] = original
            wait(0)
        }
        item.destroy()
        wait(0)
    }

    function test_animation(data) {
        const target = createTemporaryQmlObject("import QtQuick 2.15\nItem { width: 100; height: 40; opacity: 0.6 }", testCase)
        verify(EAnimation.play(target, data.effect, {duration: 0}))
        verify(EAnimation.stop(target))
        compare(target.opacity, 0.6)
        verify(!EAnimation.play(target, "missing-effect"))
        verify(!EAnimation.play(null, data.effect))
        target.destroy()
        wait(0)
    }

    function test_components_data() {
        const rows = []
        for (const name of Inventory.types) {
            for (const state of [{dark: false, language: "zh", width: 640},
                {dark: true, language: "en", width: 320},
                {dark: false, language: "en", width: 320},
                {dark: true, language: "zh", width: 640}])
                rows.push({tag: name + "-" + (state.dark ? "dark" : "light") + "-" + state.width,
                    typeName: name, state: state})
        }
        return rows
    }

    function test_components(data) {
        ETheme.setDark(data.state.dark)
        ELocale.language = data.state.language
        const item = createTemporaryQmlObject("import EUI 1.0\n" + data.typeName + " {}",
            testCase, data.typeName)
        verify(item !== null)
        if (item.width !== undefined) {
            item.width = data.state.width
            item.height = 180
            if (item.children !== undefined && data.typeName !== "EApplicationWindow")
                verify(waitForItemPolished(item))
            verify(isFinite(item.width) && isFinite(item.height))
            if (item.implicitWidth !== undefined)
                verify(isFinite(item.implicitWidth) && isFinite(item.implicitHeight))
            for (const dimension of [0, 1, 320, 1280, data.state.width]) {
                item.width = dimension
                wait(0)
                verify(isFinite(item.width) && isFinite(item.height))
            }
        }
        if (item.enabled !== undefined) {
            item.enabled = false
            wait(0)
            compare(item.enabled, false)
            item.enabled = true
        }
        if (item.visible !== undefined) {
            item.visible = false
            wait(0)
            if (data.typeName !== "EApplicationWindow")
                item.visible = true
        }
        item.destroy()
        wait(0)
    }

    height: 900
    width: 1400
    visible: true
    when: windowShown
    name: "PublicComponents"
}
