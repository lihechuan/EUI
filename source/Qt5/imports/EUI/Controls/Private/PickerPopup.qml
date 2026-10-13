import EUI 1.0
import EUI.Private 1.0
import "../Config/EControlConfig.js" as EControlConfig
import QtQuick 2.15
import QtQuick.Controls 2.15

Popup {
    id: control

    margins: 8
    padding: 12
    background: FieldSurface {}
    y: parent ? parent.height + 6 : 0
    horizontalPadding: Math.max(1, padding)
    z: EControlConfig.valueFor(control, "zIndex", 0)
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent
}
