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
        const pages = [
            {name: "home", component: "", height: 1440},
            {name: "overview", component: ""},
            {name: "buttons", component: "EButton"},
            {name: "table", component: "ETable"},
            {name: "form", component: "EForm"},
            {name: "chart", component: "EChart"},
            {name: "code-view", component: "ECodeView"},
            {name: "command-palette", component: "ECommandPalette"},
            {name: "wizard", component: "EWizard"},
            {name: "task-board", component: "ETaskBoard"},
            {name: "compare", component: "ECompare"},
            {name: "image", component: "EImage"},
            {name: "echarts-mixed", component: "EECharts", chartType: "mixed"},
            {name: "echarts-sankey", component: "EECharts", chartType: "sankey"},
            {name: "echarts-sunburst", component: "EECharts", chartType: "sunburst"},
            {name: "echarts-heatmap", component: "EECharts", chartType: "heatmap"},
            {name: "icons", component: "EIcon"},
            {name: "source", component: "EButton", source: true},
            {name: "quick3d", component: "TokyoScene"}
        ]
        for (const language of ["zh", "en"]) {
            for (const dark of [false, true]) {
                for (const page of pages)
                    cases.push(Object.assign({tag: page.name + (dark ? "-dark" : "") + "-" + language,
                        language: language, dark: dark}, page))
            }
        }
        return cases
    }

    function test_capture(data) {
        ELocale.language = data.language
        ETheme.setDark(data.dark)
        const definition = Qt.createComponent("qrc:/example/qml/component/App.qml")
        compare(definition.status, Component.Ready, definition.errorString())
        const window = definition.createObject(null, {width: 1440, height: data.height || 1000})
        verify(window !== null)
        try {
            const loader = findChild(window, "galleryLoader")
            tryCompare(loader, "status", Loader.Ready, 20000)
            if (data.component) {
                window.showComponent(exampleCatalog.find(data.component))
                tryCompare(loader, "status", Loader.Ready, 20000)
                if (data.component !== "TokyoScene")
                    tryVerify(function() { return loader.item.previewsReady }, 15000)
            }
            verify(waitForRendering(window.contentItem))
            wait(500)
            const page = loader.item
            if (data.chartType) {
                const selector = findChild(page, "galleryChartType")
                verify(selector !== null)
                selector.currentIndex = exampleData.models.EChartsTypes.findIndex(function(type) {
                    return type.value === data.chartType
                })
                verify(selector.currentIndex >= 0)
                const chart = findChild(page, "galleryApacheChart")
                verify(chart !== null && chart.supported)
                tryCompare(chart, "ready", true, 20000)
                compare(chart.errorString, "")
                wait(1800)
            } else if (data.component === "TokyoScene") {
                verify(exampleCatalog.quick3DAvailable)
                const sceneLoader = findChild(page, "quick3dSceneLoader")
                verify(sceneLoader !== null)
                tryCompare(sceneLoader, "status", Loader.Ready, 30000)
                const scene = findChild(sceneLoader.item, "tokyoScene")
                verify(scene !== null)
                tryVerify(function() { return scene.fps > 0 }, 15000)
                wait(1200)
                scene.playing = false
            } else if (data.component === "ECommandPalette") {
                const scenario = findChild(page, "showcaseECommandPalette0")
                verify(scenario !== null)
                const button = descendants(scenario.sample).find(function(item) {
                    return item.text !== undefined && item.clicked !== undefined
                })
                verify(button !== undefined)
                mouseClick(button)
                wait(500)
            }
            if (data.source) {
                const expanders = descendants(page).filter(function(item) {
                    return item.objectName === "galleryCodeExpander" && item.visible && item.code.length > 0
                })
                verify(expanders.length > 0)
                expanders[0].expanded = true
                wait(400)
            }
            window.update()
            verify(waitForRendering(window.contentItem, 5000))
            const url = Qt.resolvedUrl("../../docs/screenshots/" + exampleUpdater.currentVersion
                + "/" + data.tag + ".png").toString()
            const file = decodeURIComponent(url.replace(Qt.platform.os === "windows" ? /^file:\/\/\// : /^file:\/\//, ""))
            const image = grabImage(window.contentItem)
            verify(image.width > 0 && image.height > 0)
            image.save(file)
            console.log("CAPTURED", data.tag, exampleUpdater.currentVersion)
        } finally {
            window.close()
            window.destroy()
            wait(0)
            definition.destroy()
        }
    }

    when: windowShown
    name: "DocumentationScreenshots"
}
