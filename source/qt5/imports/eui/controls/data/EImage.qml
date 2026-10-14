import EUI 1.0
import "../config/EControlConfig.js" as EControlConfig
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Control {
    id: control

    property url source: ""
    property bool lazy: false
    property real maxScale: 7
    property real minScale: 0.2
    property real zoomRate: 1.2
    property bool infinite: true
    property int initialIndex: 0
    property bool preview: false
    property var previewSrcList: []
    property bool showProgress: true
    property bool hideOnClickModal: true
    property bool closeOnPressEscape: true
    property Component errorDelegate: null
    property Component viewerDelegate: null
    property Component toolbarDelegate: null
    property Component placeholderDelegate: null
    property int fillMode: Image.PreserveAspectFit
    property string errorText: EControlConfig.textFor(control, "FailedToLoad", ELocale.texts)

    readonly property int status: image.status
    readonly property int previewIndex: internal.previewIndex
    readonly property real previewScale: internal.previewScale
    readonly property bool previewVisible: previewPopup.visible
    readonly property real previewRotation: internal.previewRotation
    readonly property var previewSources: Array.isArray(previewSrcList) && previewSrcList.length > 0
        ? previewSrcList : source.toString().length > 0 ? [source] : []

    signal loaded()
    signal loadError()
    signal previewShown()
    signal previewClosed()
    signal previewRotated(real angle)
    signal previewSwitched(int index)

    function closePreview() {
        previewPopup.close()
    }

    function loadNow() {
        internal.loadStarted = true
    }

    function showPreview(index) {
        return openPreview(index)
    }

    function resetPreview() {
        internal.previewScale = 1
        internal.previewRotation = 0
        viewerImage.x = 0
        viewerImage.y = 0
    }

    function rotatePreview(angle) {
        if (!previewVisible || !isFinite(angle))
            return false
        internal.previewRotation = (internal.previewRotation + Number(angle)) % 360
        previewRotated(previewRotation)
        return true
    }

    function zoomPreview(factor) {
        if (!previewVisible || !isFinite(factor) || factor <= 0)
            return false
        const lower = Math.max(0.01, Math.min(minScale, maxScale))
        const upper = Math.max(lower, maxScale)
        internal.previewScale = Math.max(lower, Math.min(upper, previewScale * factor))
        return true
    }

    function openPreview(index) {
        if (!enabled
            || (!preview && previewSources.length <= 1)
            || status !== Image.Ready
            || previewSources.length === 0)
            return false
        const requested = index === undefined ? initialIndex : Number(index)
        internal.previewIndex = isFinite(requested)
            ? Math.max(0, Math.min(previewSources.length - 1, Math.floor(requested)))
            : 0
        resetPreview()
        previewPopup.open()
        return true
    }

    function setActiveItem(index) {
        if (!previewVisible || previewSources.length === 0 || !isFinite(index))
            return false
        const count = previewSources.length
        const value = Math.floor(index)
        const next = infinite ? (value % count + count) % count : Math.max(0, Math.min(count - 1, value))
        if (next === previewIndex)
            return false
        internal.previewIndex = next
        resetPreview()
        previewSwitched(next)
        return true
    }

    function checkVisibility() {
        if (!lazy || internal.loadStarted)
            return
        if (!visible || !Window.window)
            return
        let item = control
        while (item.parent) {
            const parent = item.parent
            if (parent.clip) {
                const position = control.mapToItem(parent, 0, 0)
                if (position.x + width <= 0 || position.y + height <= 0
                        || position.x >= parent.width || position.y >= parent.height)
                    return
            }
            item = parent
        }
        internal.loadStarted = true
    }

    clip: true
    implicitWidth: 200
    implicitHeight: 160
    background: ERectangle {
        color: EPaletteManager.ThemePalette.surfaceHover(ETheme.dark)
    }
    contentItem: Item {
        Image {
            id: image

            asynchronous: true
            anchors.fill: parent
            fillMode: control.fillMode
            source: !control.lazy || internal.loadStarted ? control.source : ""

            onStatusChanged: {
                if (status === Image.Error)
                    control.loadError()
                else if (status === Image.Ready)
                    control.loaded()
            }
        }
        ELoading {
            width: 24
            height: 24
            material: true
            anchors.centerIn: parent
            visible: image.status === Image.Loading && !control.placeholderDelegate
        }
        EText {
            font: control.font
            anchors.centerIn: parent
            color: EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
            text: image.status === Image.Error ? control.errorText
                : EControlConfig.textFor(control, "Image", ELocale.texts)
            visible: image.status === Image.Error && !control.errorDelegate
                || image.status === Image.Null && !control.placeholderDelegate
        }
        Loader {
            anchors.fill: parent
            active: image.status !== Image.Ready && sourceComponent !== null
            sourceComponent: image.status === Image.Error ? control.errorDelegate
                : control.placeholderDelegate
        }
        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            enabled: control.preview || control.previewSources.length > 1

            onClicked: control.openPreview()
        }
    }

    onLazyChanged: Qt.callLater(checkVisibility)
    Component.onCompleted: Qt.callLater(checkVisibility)
    onSourceChanged: {
        internal.loadStarted = !lazy
        if (previewVisible)
            closePreview()
        Qt.callLater(checkVisibility)
    }

    QtObject {
        id: internal

        property int previewIndex: 0
        property real previewScale: 1
        property bool loadStarted: false
        property real previewRotation: 0
    }
    Timer {
        repeat: true
        interval: 150
        running: control.lazy && !internal.loadStarted && control.visible

        onTriggered: control.checkVisibility()
    }
    Popup {
        id: previewPopup

        focus: true
        modal: true
        padding: 48
        parent: Overlay.overlay
        objectName: "imagePreview"
        width: parent ? parent.width : 0
        height: parent ? parent.height : 0
        background: ERectangle {
            color: "#E61A1A1A"

            MouseArea {
                anchors.fill: parent
                enabled: control.hideOnClickModal

                onClicked: control.closePreview()
            }
        }
        closePolicy: (control.closeOnPressEscape ? Popup.CloseOnEscape : Popup.NoAutoClose)
            | (control.hideOnClickModal ? Popup.CloseOnPressOutside : Popup.NoAutoClose)
        contentItem: Item {
            id: viewerArea

            clip: true
            focus: true

            Keys.onLeftPressed: control.setActiveItem(control.previewIndex - 1)
            Keys.onRightPressed: control.setActiveItem(control.previewIndex + 1)
            Keys.onPressed: {
                if (event.key === Qt.Key_Plus || event.key === Qt.Key_Equal) {
                    control.zoomPreview(Math.max(1.01, control.zoomRate))
                    event.accepted = true
                } else if (event.key === Qt.Key_Minus) {
                    control.zoomPreview(1 / Math.max(1.01, control.zoomRate))
                    event.accepted = true
                } else if (event.key === Qt.Key_R) {
                    control.rotatePreview(90)
                    event.accepted = true
                }
            }

            Image {
                id: viewerImage

                asynchronous: true
                width: viewerArea.width
                height: viewerArea.height
                scale: control.previewScale
                visible: !control.viewerDelegate
                fillMode: Image.PreserveAspectFit
                rotation: control.previewRotation
                source: control.previewVisible && control.previewSources.length > 0
                    ? control.previewSources[control.previewIndex] : ""
            }
            Loader {
                property int currentIndex: control.previewIndex
                property url source: control.previewSources.length > 0
                    ? control.previewSources[control.previewIndex]
                    : ""

                anchors.fill: parent
                active: sourceComponent !== null
                sourceComponent: control.viewerDelegate
            }
            EText {
                color: "white"
                text: control.errorText
                anchors.centerIn: parent
                visible: viewerImage.status === Image.Error && !control.viewerDelegate
            }
            MouseArea {
                anchors.fill: parent
                drag.target: viewerImage
                enabled: !control.viewerDelegate

                onWheel: {
                    control.zoomPreview(wheel.angleDelta.y > 0 ? Math.max(1.01, control.zoomRate)
                        : 1 / Math.max(1.01, control.zoomRate))
                    wheel.accepted = true
                }
            }
        }

        onClosed: control.previewClosed()
        onOpened: {
            viewerArea.forceActiveFocus()
            control.previewShown()
        }

        EButton {
            width: 32
            height: 32
            anchors.topMargin: 12
            anchors.rightMargin: 12
            anchors.top: parent.top
            anchors.right: parent.right
            objectName: "imagePreviewClose"
            text: EControlConfig.textFor(control, "CloseGlyph", ELocale.texts)
            Accessible.name: EControlConfig.textFor(control, "ClosePreview", ELocale.texts)

            onClicked: control.closePreview()
        }
        EButton {
            width: 32
            height: 32
            anchors.left: parent.left
            objectName: "imagePreviewPrevious"
            visible: control.previewSources.length > 1
            anchors.verticalCenter: parent.verticalCenter
            enabled: control.infinite || control.previewIndex > 0
            text: EControlConfig.textFor(control, "PreviousGlyph", ELocale.texts)

            onClicked: control.setActiveItem(control.previewIndex - 1)
        }
        EButton {
            width: 32
            height: 32
            anchors.right: parent.right
            objectName: "imagePreviewNext"
            visible: control.previewSources.length > 1
            anchors.verticalCenter: parent.verticalCenter
            text: EControlConfig.textFor(control, "NextGlyph", ELocale.texts)
            enabled: control.infinite || control.previewIndex < control.previewSources.length - 1

            onClicked: control.setActiveItem(control.previewIndex + 1)
        }
        Row {
            spacing: 8
            anchors.bottomMargin: 8
            anchors.bottom: parent.bottom
            visible: !control.toolbarDelegate
            anchors.horizontalCenter: parent.horizontalCenter

            EButton {
                text: "−"
                objectName: "imagePreviewZoomOut"

                onClicked: control.zoomPreview(1 / Math.max(1.01, control.zoomRate))
            }
            EButton {
                text: "+"
                objectName: "imagePreviewZoomIn"

                onClicked: control.zoomPreview(Math.max(1.01, control.zoomRate))
            }
            EButton {
                text: "↶"
                objectName: "imagePreviewRotate"

                onClicked: control.rotatePreview(-90)
            }
            EButton {
                text: "1:1"
                objectName: "imagePreviewReset"

                onClicked: control.resetPreview()
            }
            EText {
                height: 32
                color: "white"
                visible: control.showProgress
                verticalAlignment: Text.AlignVCenter
                text: (control.previewIndex + 1) + " / " + control.previewSources.length
            }
        }
        Loader {
            anchors.bottom: parent.bottom
            active: sourceComponent !== null
            sourceComponent: control.toolbarDelegate
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
