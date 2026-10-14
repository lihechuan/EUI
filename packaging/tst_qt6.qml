import EUI 1.0
import QtQuick 2.15
import QtTest 1.2

TestCase {
    id: testCase

    function test_controls(data) {
        const item = createTemporaryQmlObject("import EUI 1.0; " + data.name + " {}", testCase)
        verify(item !== null)
        verify(item.width >= 0)
        verify(item.height >= 0)
    }

    function test_controls_data() {
        return ["EClip", "EUpload", "EMediaPlayer", "ECarousel", "EThemeSwitch", "EECharts",
            "ECalendar", "EDatePicker", "EDateTimePicker", "ETable", "EApplicationWindow",
            "EFilePicker", "EFolderPicker", "EToolBar", "ECodeView", "ECommandPalette", "EWizard"]
            .map(function(name) { return {tag: name, name: name} })
    }

    function test_uploadModes() {
        const upload = createTemporaryQmlObject("import EUI 1.0; EUpload {}", testCase)
        verify(upload !== null)
        compare(upload.multiple, true)
        compare(upload.directory, false)
        upload.directory = true
        upload.multiple = false
        compare(upload.directory, true)
        compare(upload.multiple, false)
        compare(upload.addFiles([]), false)
    }

    function test_mediaProperties() {
        const player = createTemporaryQmlObject("import EUI 1.0; EMediaPlayer {}", testCase)
        verify(player !== null)
        compare(player.autoPlay, false)
        player.muted = true
        player.volume = 0.25
        compare(player.muted, true)
        compare(player.volume, 0.25)
        player.play()
        player.pause()
        player.stop()
        player.seek(0)
        compare(player.position, 0)
    }

    function test_nativeLists() {
        const carousel = createTemporaryObject(carouselComponent, testCase, {model: ["one", "two"]})
        compare(carousel.count, 2)
        const select = createTemporaryObject(selectComponent, testCase,
            {model: ["one", "two"], value: ["two"]})
        compare(select.selectedValues.length, 1)
        compare(select.selectedValues[0], "two")
        select.value = {0: "one", length: 1}
        compare(select.selectedValues.length, 0)
        carousel.model = {0: "one", length: 1}
        compare(carousel.count, 0)
    }

    width: 640
    height: 480
    name: "Qt6Compatibility"

    Component {
        id: carouselComponent

        ECarousel {
            autoplay: false
        }
    }
    Component {
        id: selectComponent

        ESelect {
            multiple: true
        }
    }
}
