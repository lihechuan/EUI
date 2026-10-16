import EUI 1.0
import QtQuick 2.15
import QtQml 2.15
import QtQuick.Dialogs 1.3
import QtQuick.Window 2.15
import Qt.labs.calendar 1.0
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import QtGraphicalEffects 1.15

Rectangle {
    width: 640
    height: 420
    color: ETheme.dark ? "#171c24" : "#ffffff"

    Flickable {
        id: viewport

        clip: true
        anchors.fill: parent
        contentHeight: host.height + 48
        contentWidth: Math.max(width, host.childrenRect.width + 48)

        Item {
            id: host

            x: 24
            y: 24
            objectName: "previewHost"
            width: Math.max(0, viewport.width - 48)
            height: Math.max(viewport.height - 48, childrenRect.y + childrenRect.height)
        }
    }
}
