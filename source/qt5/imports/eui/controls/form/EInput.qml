import EUI 1.0
import EUI.Private 1.0
import "../indicator" as IndicatorControls
import "../config/EControlConfig.js" as EControlConfig
import QtQuick 2.15
import QtQuick.Controls 2.15

TextField {
    id: control

    property int prefixIcon
    property int suffixIcon
    property var parser: null
    property var formatter: null
    property bool clearable: false
    property bool showPassword: false
    property bool validateEvent: true
    property bool showWordLimit: false
    property bool passwordVisible: false
    property string validationMessage: ""
    property int styleType: EStyle.Primary
    property Component appendComponent: null
    property Component prefixComponent: null
    property Component suffixComponent: null
    property Component prependComponent: null
    property int clearIcon: EIcons.CIRCLE_XMARK
    property string accessibleName: placeholderText
    property int size: EControlConfig.sizeFor(control)
    property int validationState: EValidationState.None
    property color textColor: EPaletteManager.StatePalette.defaultTextColor(
        EPaletteManager.createStateObject(this, EStyle.TextColor)
            .setDark(ETheme.dark)
            .setEnabled(control.enabled)
    )

    readonly property int wordCount: text.length
    readonly property bool hasValidationError: validationState === EValidationState.Error

    signal cleared()
    signal formatFailed(string message)

    function formatInput(value) {
        try {
            const parsed = typeof parser === "function" ? parser(String(value)) : String(value)
            return typeof formatter === "function" ? String(formatter(parsed)) : String(parsed)
        } catch (error) {
            formatFailed(String(error))
            return String(value)
        }
    }

    hoverEnabled: true
    selectByMouse: true
    renderType: Text.QtRendering
    Accessible.name: control.accessibleName
    Accessible.role: Accessible.EditableText
    selectionColor: EPaletteManager.BasePalette.blue
    Accessible.description: control.validationMessage
    selectedTextColor: EPaletteManager.BasePalette.white
    implicitHeight: EDefaultConfig.controlHeight(control.size)
    font.pixelSize: EDefaultConfig.controlFontPixelSize(control.size)
    echoMode: control.showPassword && !control.passwordVisible
        ? TextInput.Password : TextInput.Normal
    placeholderTextColor: enabled ? EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
        : EPaletteManager.ThemePalette.textDisabled(ETheme.dark)
    background: FieldSurface {
        hovered: control.hovered
        focused: control.activeFocus
        styleType: control.styleType
        validationState: control.validationState
    }
    leftPadding: EDefaultConfig.horizontalPadding(control.size)
        + prependLoader.width + (prefixLoader.active ? prefixLoader.width + 6 : 0)
        + (prefixIconItem.visible ? prefixIconItem.width + 6 : 0)
    color: EPaletteManager.StatePalette.computeEffectiveTextColor(
        EPaletteManager.createStateObject(this, EStyle.Color)
            .setDark(ETheme.dark)
            .setEnabled(control.enabled)
            .setPlain(true)
            .setColor(textColor)
    )
    rightPadding: EDefaultConfig.horizontalPadding(control.size)
        + appendLoader.width + (suffixLoader.active ? suffixLoader.width + 6 : 0)
        + (suffixIconItem.visible ? suffixIconItem.width + 6 : 0)
        + (wordLimit.visible ? wordLimit.implicitWidth + 6 : 0)

    onEditingFinished: if (typeof formatter === "function" || typeof parser === "function")
        text = formatInput(text)

    IndicatorControls.EToolTip {
        delay: 300
        triggerKeys: []
        trigger: "manual"
        reference: control
        text: control.validationMessage
        objectName: "inputValidationTooltip"
        visible: control.enabled && control.validationState !== EValidationState.None
            && control.validationState !== EValidationState.Success
            && control.validationMessage.length > 0
            && (control.hovered || control.activeFocus)
    }
    EAdvancedRectangle {
        borderWidth: 1
        height: control.height
        width: prependLoader.width
        visible: prependLoader.active
        color: EPaletteManager.ThemePalette.surfaceHover(ETheme.dark)
        borderColor: control.background ? control.background.border.color : "transparent"
        radius: control.background ? [control.background.radius, 0, 0, control.background.radius] : 0
    }
    EAdvancedRectangle {
        borderWidth: 1
        height: control.height
        width: appendLoader.width
        anchors.right: control.right
        visible: appendLoader.active
        color: EPaletteManager.ThemePalette.surfaceHover(ETheme.dark)
        borderColor: control.background ? control.background.border.color : "transparent"
        radius: control.background ? [0, control.background.radius, control.background.radius, 0] : 0
    }
    Loader {
        id: prependLoader

        readonly property var input: control

        height: control.height
        anchors.left: control.left
        objectName: "inputPrepend"
        active: control.prependComponent !== null
        sourceComponent: control.prependComponent
        width: active && item ? item.implicitWidth + 16 : 0
    }
    Loader {
        id: appendLoader

        readonly property var input: control

        height: control.height
        objectName: "inputAppend"
        anchors.right: control.right
        active: control.appendComponent !== null
        sourceComponent: control.appendComponent
        width: active && item ? item.implicitWidth + 16 : 0
    }
    Loader {
        id: prefixLoader

        readonly property var input: control

        objectName: "inputPrefix"
        y: (control.height - height) / 2
        active: control.prefixComponent !== null
        sourceComponent: control.prefixComponent
        width: active && item ? item.implicitWidth : 0
        height: active && item ? item.implicitHeight : 0
        x: prependLoader.width + EDefaultConfig.horizontalPadding(control.size)
    }
    Loader {
        id: suffixLoader

        readonly property var input: control

        objectName: "inputSuffix"
        y: (control.height - height) / 2
        active: control.suffixComponent !== null
        sourceComponent: control.suffixComponent
        width: active && item ? item.implicitWidth : 0
        height: active && item ? item.implicitHeight : 0
        x: control.width - appendLoader.width - EDefaultConfig.horizontalPadding(control.size) - width
    }
    EText {
        id: wordLimit

        font.pixelSize: 11
        anchors.right: control.right
        anchors.verticalCenter: control.verticalCenter
        color: EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
        visible: control.showWordLimit && control.length > 0 && !control.activeFocus
        text: control.wordCount + (control.maximumLength < 32767 ? "/" + control.maximumLength : "")
        anchors.rightMargin: EDefaultConfig.horizontalPadding(control.size) + appendLoader.width
            + (suffixLoader.active ? suffixLoader.width + 6 : 0)
            + (suffixIconItem.visible ? suffixIconItem.width + 6 : 0)
    }
    EIcon {
        id: prefixIconItem

        height: width
        iconSize: width
        color: control.textColor
        anchors.left: control.left
        iconSource: control.prefixIcon
        visible: control.prefixIcon > 0
        width: EDefaultConfig.iconSize(control.size)
        anchors.verticalCenter: control.verticalCenter
        anchors.leftMargin: EDefaultConfig.horizontalPadding(control.size) + prependLoader.width
            + (prefixLoader.active ? prefixLoader.width + 6 : 0)
    }
    EIcon {
        id: suffixIconItem

        readonly property bool actionable: canClear || control.showPassword
        readonly property bool canClear: control.clearable && control.text.length > 0

        height: width
        iconSize: width
        color: control.textColor
        anchors.right: control.right
        width: EDefaultConfig.iconSize(control.size)
        visible: actionable || control.suffixIcon > 0
        anchors.verticalCenter: control.verticalCenter
        anchors.rightMargin: EDefaultConfig.horizontalPadding(control.size) + appendLoader.width
            + (suffixLoader.active ? suffixLoader.width + 6 : 0)
        iconSource: control.showPassword
            ? (control.passwordVisible ? EIcons.EYE : EIcons.EYE_SLASH)
            : (canClear ? control.clearIcon : control.suffixIcon)

        MouseArea {
            anchors.fill: parent
            cursorShape: Qt.PointingHandCursor
            enabled: suffixIconItem.actionable

            onClicked: {
                if (control.showPassword) {
                    control.passwordVisible = !control.passwordVisible
                } else if (suffixIconItem.canClear) {
                    control.clear()
                    control.cleared()
                    control.forceActiveFocus()
                }
            }
        }
    }
}
