import EUI 1.0
import "../../example/qml/component" as Gallery
import QtQuick 2.15
import QtTest 1.2

TestCase {
    function cleanup() {
        ETheme.setDark(false)
        ELocale.language = "zh"
    }

    function test_textRendering_data() {
        return [{tag: "light-zh", dark: false, language: "zh"},
            {tag: "dark-zh", dark: true, language: "zh"},
            {tag: "light-en", dark: false, language: "en"},
            {tag: "dark-en", dark: true, language: "en"}]
    }

    function test_textRendering(data) {
        ETheme.setDark(data.dark)
        ELocale.language = data.language
        const window = createTemporaryObject(windowComponent, null)
        verify(window !== null)
        try {
            tryCompare(findChild(window, "galleryLoader"), "status", Loader.Ready)
            const navigation = findChild(window, "galleryNavigation")
            verify(waitForItemPolished(navigation))
            mouseMove(window.contentItem, window.width - 20, 60)
            wait(200)
            const actual = grabImage(navigation)
            if (!actual.width)
                skip("This platform cannot capture pixels; run on the isolated Windows desktop")
            navigation.layer.enabled = false
            window.update()
            wait(50)
            const reference = grabImage(navigation)
            verify(actual.equals(reference), "Navigation text must match rendering directly into the window")
        } finally {
            window.close()
        }
    }

    height: 820
    width: 1040
    visible: true
    when: windowShown
    name: "GalleryNavigation"

    Component {
        id: windowComponent

        Gallery.App {
            asynchronousLoading: false
        }
    }
}
