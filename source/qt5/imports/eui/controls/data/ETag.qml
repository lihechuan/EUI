import EUI 1.0
import "../config/EControlConfig.js" as EControlConfig
import QtQuick 2.15
import QtQuick.Controls 2.15

Control {
    id: control

    property bool hit: false
    property string text: ""
    property bool round: false
    property bool checked: false
    property bool closable: false
    property bool checkable: false
    property string effect: "light"
    property int styleType: EStyle.Primary
    property int size: EControlConfig.sizeFor(control)
    property color color: EPaletteManager.BasePalette.transparent

    readonly property bool darkEffect: effect === "dark" || checkable && checked

    signal closed()
    signal clicked()
    signal changed(bool checked)

    topPadding: 0
    leftPadding: 8
    rightPadding: 8
    bottomPadding: 0
    implicitWidth: label.implicitWidth + leftPadding + rightPadding
    implicitHeight: size === EStyle.Small ? 20 : size === EStyle.Large ? 32 : 24
    background: ERectangle {
        border.color: inside.borderColor
        border.width: control.hit ? 2 : 1
        radius: control.round ? height / 2 : 4
        color: control.color.a > 0 ? control.color : inside.backgroundColor
    }
    contentItem: EText {
        id: label

        text: control.text
        elide: Text.ElideRight
        color: inside.textColor
        verticalAlignment: Text.AlignVCenter
        rightPadding: control.closable ? 20 : 0
        font.pixelSize: control.size === EStyle.Small ? 12 : 14
    }

    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor

        onClicked: {
            control.clicked()
            if (control.checkable) {
                control.checked = !control.checked
                control.changed(control.checked)
            }
        }
    }
    ETextButton {
        width: 16
        height: 16
        circle: true
        iconSize: 12
        minHeight: 16
        color: inside.textColor
        iconSource: EIcons.XMARK
        visible: control.closable
        anchors.right: control.right
        anchors.rightMargin: control.rightPadding
        anchors.verticalCenter: control.verticalCenter
        accessibleName: EControlConfig.textFor(control, "Close", ELocale.texts)

        onClicked: control.closed()
    }
    QtObject {
        id: inside

        readonly property color textColor: EPaletteManager.StatePalette.computeEffectiveTextColor(
            EPaletteManager.createStateObject(this, EStyle.Color)
                .setDark(ETheme.dark)
                .setPlain(!control.darkEffect)
                .setEnabled(control.enabled)
                .setComponentStyle(control.styleType)
        )
        readonly property color borderColor: EPaletteManager.StatePalette.computeBorderColor(
            EPaletteManager.createStateObject(this, EStyle.BorderColor)
                .setDark(ETheme.dark)
                .setBorder(true)
                .setPlain(!control.darkEffect)
                .setEnabled(control.enabled)
                .setComponentStyle(control.styleType)
        )
        readonly property color backgroundColor: control.effect === "plain"
            ? EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
            : EPaletteManager.StatePalette.computeBackgroundColor(
                EPaletteManager.createStateObject(this, EStyle.BackgroundColor)
                    .setDark(ETheme.dark)
                    .setPlain(!control.darkEffect)
                    .setEnabled(control.enabled)
                    .setComponentStyle(control.styleType)
            )
    }
}
