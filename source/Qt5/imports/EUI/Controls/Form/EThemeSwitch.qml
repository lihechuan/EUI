import EUI 1.0
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

ESwitch {
    id: control

    property int duration: 300
    property int transitionDuration: 400
    property color darkIconColor: EPaletteManager.BasePalette.white
    property color darkThumbColor: EPaletteManager.BasePalette.black
    property color lightThumbColor: EPaletteManager.BasePalette.white
    property color darkTrackColor: EPaletteManager.BasePalette.darkGray
    property color lightBorderColor: EPaletteManager.BasePalette.gainsboro
    property color lightTrackColor: EPaletteManager.BasePalette.ghostWhite
    property color darkBorderColor: EPaletteManager.BasePalette.charcoalGray
    property color lightIconColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)

    readonly property bool dark: ETheme.dark
    readonly property Item captureRoot: inside.findCaptureRoot()
    readonly property Item transitionRoot: Window.window ? Window.window.contentItem : null

    signal themeChangeRequested()

    spacing: 0
    implicitWidth: 40
    hoverEnabled: true
    implicitHeight: 20
    background: Item { }
    width: implicitWidth
    contentItem: Item { }
    height: implicitHeight
    indicator: ERectangle {
        id: track

        border.width: 1
        radius: height / 2
        anchors.fill: parent
        opacity: control.enabled ? 1 : 0.55
        color: inside.switchDark ? control.darkTrackColor : control.lightTrackColor
        border.color: inside.switchDark ? control.darkBorderColor : control.lightBorderColor

        Behavior on color {
            ColorAnimation {
                easing.type: Easing.Bezier
                duration: Math.max(0, control.duration)
                easing.bezierCurve: [0.645, 0.045, 0.355, 1, 1, 1]
            }
        }
        Behavior on border.color {
            ColorAnimation {
                easing.type: Easing.Bezier
                duration: Math.max(0, control.duration)
                easing.bezierCurve: [0.645, 0.045, 0.355, 1, 1, 1]
            }
        }
        Behavior on opacity {
            NumberAnimation {
                easing.type: Easing.Bezier
                duration: Math.max(0, control.duration)
                easing.bezierCurve: [0.645, 0.045, 0.355, 1, 1, 1]
            }
        }
        ERectangle {
            id: thumb

            clip: true
            height: width
            border.width: 1
            radius: width / 2
            y: (parent.height - height) / 2
            x: 2 + inside.switchProgress * (parent.width - width - 4)
            width: Math.max(0, Math.min(parent.height - 4, parent.width - 4))
            color: inside.switchDark ? control.darkThumbColor : control.lightThumbColor
            border.color: inside.switchDark ? control.darkBorderColor : control.lightBorderColor

            Behavior on color {
                ColorAnimation {
                    easing.type: Easing.Bezier
                    duration: Math.max(0, control.duration)
                    easing.bezierCurve: [0.645, 0.045, 0.355, 1, 1, 1]
                }
            }
            Behavior on border.color {
                ColorAnimation {
                    easing.type: Easing.Bezier
                    duration: Math.max(0, control.duration)
                    easing.bezierCurve: [0.645, 0.045, 0.355, 1, 1, 1]
                }
            }
            EIcon {
                anchors.fill: parent
                iconSource: EIcons.SUN
                color: control.lightIconColor
                opacity: 1 - inside.switchProgress
                rotation: inside.switchProgress * 90
                iconSize: Math.max(0, thumb.width - 6)
                scale: 1 - inside.switchProgress * 0.35
            }
            EIcon {
                anchors.fill: parent
                iconSource: EIcons.MOON
                color: control.darkIconColor
                opacity: inside.switchProgress
                iconSize: Math.max(0, thumb.width - 6)
                rotation: -90 + inside.switchProgress * 90
                scale: 0.65 + inside.switchProgress * 0.35
            }
        }
        HoverHandler {
            cursorShape: Qt.PointingHandCursor
            acceptedDevices: PointerDevice.Mouse
        }
    }

    onClicked: inside.requestThemeChange()
    Component.onCompleted: {
        checked = ETheme.dark
        inside.switchDark = checked
        inside.requestedDark = checked
        inside.switchProgress = checked ? 1 : 0
    }

    QtObject {
        id: inside

        property real progress: 0
        property bool switchDark: false
        property real switchProgress: 0
        property bool targetDark: false
        property int animationDuration: 0
        property bool requestedDark: false
        property bool snapshotVisible: false
        property bool switchTriggered: false
        property point origin: Qt.point(0, 0)
        property real switchTriggerProgress: 0
        property bool transitionRunning: false
        property point switchOrigin: Qt.point(0, 0)

        function continueThemeTransition() {
            if (!transitionRunning && requestedDark !== ETheme.dark)
                startThemeTransition()
        }

        function findCaptureRoot() {
            let item = control
            while (item.parent && item.parent !== control.transitionRoot)
                item = item.parent
            return item
        }

        function triggerSwitchTransition() {
            if (switchTriggered)
                return

            switchTriggered = true
            control.checked = targetDark
            switchDark = targetDark
            switchProgress = targetDark ? 1 : 0
        }

        function requestThemeChange() {
            if (transitionRunning) {
                requestedDark = !requestedDark
                control.checked = switchDark
                return
            }

            requestedDark = control.checked
            startThemeTransition()
        }

        function finishThemeTransition() {
            triggerSwitchTransition()
            snapshotVisible = false
            transitionRunning = false
            if (requestedDark !== ETheme.dark)
                Qt.callLater(continueThemeTransition)
        }

        function newSnapshotCaptured() {
            if (!transitionRunning)
                return
            if (switchTriggerProgress <= 0)
                triggerSwitchTransition()
            if (targetDark)
                themeDarkAnimation.restart()
            else
                themeLightAnimation.restart()
        }

        function oldSnapshotCaptured() {
            if (!transitionRunning || !transitionLayer.item || transitionLayer.item.newSnapshot.sourceItem)
                return
            control.themeChangeRequested()
            ETheme.setDark(targetDark)
            transitionLayer.item.newSnapshot.sourceItem = control.captureRoot
            transitionLayer.item.newSnapshot.scheduleUpdate()
        }

        function beginSnapshotCapture() {
            if (!transitionRunning)
                return
            if (!transitionLayer.item) {
                Qt.callLater(beginSnapshotCapture)
                return
            }

            if (control.GraphicsInfo.api === GraphicsInfo.Software) {
                control.themeChangeRequested()
                ETheme.setDark(targetDark)
                newSnapshotCaptured()
                return
            }
            snapshotVisible = true
            transitionLayer.item.oldSnapshot.scheduleUpdate()
        }

        function calculateSwitchTriggerProgress() {
            if (!targetDark)
                return 0

            const maximumX = Math.max(origin.x, control.transitionRoot.width - origin.x)
            const maximumY = Math.max(origin.y, control.transitionRoot.height - origin.y)
            const maximumRadius = Math.sqrt(maximumX * maximumX + maximumY * maximumY)
            const switchRadius = Math.max(control.width, control.height) / 2
            return maximumRadius > 0 ? 1 - switchRadius / maximumRadius : 0
        }

        function startThemeTransition() {
            const nextDark = requestedDark

            if (!control.transitionRoot) {
                ETheme.setDark(nextDark)
                control.themeChangeRequested()
                return
            }

            switchOrigin = control.mapToItem(control.transitionRoot, 0, 0)
            origin = Qt.point(switchOrigin.x + control.width / 2,
                switchOrigin.y + control.height / 2)
            targetDark = nextDark
            switchTriggered = false
            switchTriggerProgress = calculateSwitchTriggerProgress()
            transitionRunning = true
            animationDuration = control.duration
            progress = 0
            snapshotVisible = false
            control.checked = switchDark
            Qt.callLater(beginSnapshotCapture)
        }

        onProgressChanged: {
            if (transitionRunning && progress >= switchTriggerProgress)
                triggerSwitchTransition()
        }

        Behavior on switchProgress {
            NumberAnimation {
                easing.type: Easing.Bezier
                duration: Math.max(0, inside.animationDuration)
                easing.bezierCurve: [0.645, 0.045, 0.355, 1, 1, 1]
            }
        }
    }
    Loader {
        id: transitionLayer

        z: 1000
        anchors.fill: parent
        parent: control.transitionRoot
        active: inside.transitionRunning
        sourceComponent: transitionLayerComponent
    }
    Component {
        id: transitionLayerComponent

        Item {
            property alias newSnapshot: newThemeSnapshot
            property alias oldSnapshot: oldThemeSnapshot

            ShaderEffectSource {
                id: oldThemeSnapshot

                live: false
                smooth: true
                visible: false
                recursive: false
                hideSource: false
                sourceItem: control.captureRoot

                onScheduledUpdateCompleted: inside.oldSnapshotCaptured()
            }
            ShaderEffectSource {
                id: newThemeSnapshot

                live: false
                smooth: true
                visible: false
                recursive: false
                sourceItem: null
                hideSource: false

                onScheduledUpdateCompleted: inside.newSnapshotCaptured()
            }
            ShaderEffect {
                property point origin: inside.origin
                property real progress: inside.progress
                property variant newSource: newThemeSnapshot
                property variant oldSource: oldThemeSnapshot
                property point switchOrigin: inside.switchOrigin
                property real darkTarget: inside.targetDark ? 1 : 0
                property vector2d viewportSize: Qt.vector2d(width, height)
                property vector2d switchSize: Qt.vector2d(control.width, control.height)
                property color newBackgroundColor: EPaletteManager.ThemePalette.fetchThemeColor(
                    inside.targetDark)
                property color oldBackgroundColor: EPaletteManager.ThemePalette.fetchThemeColor(
                    !inside.targetDark)
                property real maximumRadius: Math.sqrt(
                    Math.pow(Math.max(inside.origin.x, width - inside.origin.x), 2)
                        + Math.pow(Math.max(inside.origin.y, height - inside.origin.y), 2))

                anchors.fill: parent
                visible: inside.snapshotVisible
                fragmentShader: (GraphicsInfo.profile === GraphicsInfo.OpenGLCoreProfile
                    ? "#version 150\n#define varying in\n#define texture2D texture\nout vec4 fragmentColor;\n#define gl_FragColor fragmentColor\n"
                    : "") + "
                    varying highp vec2 qt_TexCoord0;
                    uniform sampler2D oldSource;
                    uniform sampler2D newSource;
                    uniform lowp float qt_Opacity;
                    uniform lowp vec4 newBackgroundColor;
                    uniform lowp vec4 oldBackgroundColor;
                    uniform highp float darkTarget;
                    uniform highp float progress;
                    uniform highp float maximumRadius;
                    uniform highp vec2 origin;
                    uniform highp vec2 switchOrigin;
                    uniform highp vec2 switchSize;
                    uniform highp vec2 viewportSize;

                    void main() {
                        highp vec2 pixel = qt_TexCoord0 * viewportSize;
                        highp float radius = mix(maximumRadius * progress,
                            maximumRadius * (1.0 - progress), darkTarget);
                        highp float outside = smoothstep(radius - 1.5, radius + 1.5,
                            distance(pixel, origin));
                        highp float newMask = mix(1.0 - outside, outside, darkTarget);
                        lowp vec4 oldColor = texture2D(oldSource, qt_TexCoord0);
                        lowp vec4 newColor = texture2D(newSource, qt_TexCoord0);
                        oldColor += oldBackgroundColor * (1.0 - oldColor.a);
                        newColor += newBackgroundColor * (1.0 - newColor.a);
                        highp vec2 switchCenter = switchOrigin + switchSize * 0.5;
                        highp float switchRadius = switchSize.y * 0.5;
                        highp vec2 switchDistance = abs(pixel - switchCenter)
                            - (switchSize * 0.5 - vec2(switchRadius));
                        highp float switchEdge = length(max(switchDistance, vec2(0.0)))
                            + min(max(switchDistance.x, switchDistance.y), 0.0)
                            - switchRadius;
                        highp float insideSwitch = 1.0
                            - smoothstep(-1.0, 1.0, switchEdge);
                        gl_FragColor = mix(oldColor, newColor, newMask)
                            * (1.0 - insideSwitch) * qt_Opacity;
                    }
                "
            }
        }
    }
    NumberAnimation {
        id: themeDarkAnimation

        to: 1
        from: 0
        target: inside
        property: "progress"
        easing.type: Easing.Bezier
        easing.bezierCurve: [0.42, 0, 1, 1, 1, 1]
        duration: Math.max(0, control.transitionDuration)

        onFinished: inside.finishThemeTransition()
    }
    NumberAnimation {
        id: themeLightAnimation

        to: 1
        from: 0
        target: inside
        property: "progress"
        easing.type: Easing.Bezier
        easing.bezierCurve: [0, 0, 0.58, 1, 1, 1]
        duration: Math.max(0, control.transitionDuration)

        onFinished: inside.finishThemeTransition()
    }
    Connections {
        function onDarkChanged() {
            if (!inside.transitionRunning && control.checked !== ETheme.dark)
                control.checked = ETheme.dark

            if (!inside.transitionRunning) {
                inside.requestedDark = ETheme.dark
                inside.animationDuration = control.duration
                inside.switchDark = ETheme.dark
                inside.switchProgress = ETheme.dark ? 1 : 0
            }
        }

        target: ETheme
    }
}
