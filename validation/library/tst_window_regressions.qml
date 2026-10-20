import EUI 1.0
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function test_initialAppBarWidth() {
        const window = createTemporaryObject(initialBarWindowComponent, null)
        verify(window !== null)
        window.show()
        tryCompare(window.appBar, "width", 640)
        window.width = 800
        tryCompare(window.appBar, "width", 800)
        window.close()
    }

    function test_dynamicAppBarWidth() {
        const window = createTemporaryObject(windowComponent, null)
        verify(window !== null)
        window.show()
        const bar = createTemporaryObject(barComponent, window.contentItem)
        verify(bar !== null)
        window.appBar = bar
        tryCompare(bar, "width", 640)
        window.width = 800
        tryCompare(bar, "width", 800)
        window.close()
    }

    function test_appBarReplacementRestoresWidth() {
        const window = createTemporaryObject(windowComponent, null)
        verify(window !== null)
        window.show()
        const first = createTemporaryObject(barComponent, window.contentItem)
        const second = createTemporaryObject(barComponent, window.contentItem, {width: 237})
        verify(first !== null && second !== null)
        window.appBar = first
        tryCompare(first, "width", 640)
        window.appBar = second
        tryCompare(second, "width", 640)
        tryCompare(first, "width", first.preferredWidth)
        first.preferredWidth = 180
        tryCompare(first, "width", 180)
        window.width = 800
        tryCompare(second, "width", 800)
        compare(first.width, 180)
        window.appBar = null
        tryCompare(second, "width", 237)
        window.width = 900
        wait(0)
        compare(second.width, 237)
        window.close()
    }

    width: 640
    height: 480
    visible: true
    when: windowShown
    name: "WindowRegressions"

    Component {
        id: windowComponent

        EApplicationWindow {
            width: 640
            height: 480
        }
    }
    Component {
        id: barComponent

        EAppBar {
            property real preferredWidth: 160

            width: preferredWidth
        }
    }
    Component {
        id: initialBarWindowComponent

        EApplicationWindow {
            width: 640
            height: 480
            appBar: EAppBar {}
        }
    }
}
