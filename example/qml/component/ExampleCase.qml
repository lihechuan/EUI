import EUI 1.0
import QtQuick 2.15

Item {
    id: control
    default property alias content: previewContent.data

    property string code: ""
    property string title: ""
    property string description: ""

    readonly property bool hasSource: code.length > 0

    implicitHeight: panel.height
    width: parent ? parent.width : 0

    ERectangle {
        id: panel

        radius: 12
        border.width: 1
        width: parent.width
        color: GalleryStyle.surface
        border.color: GalleryStyle.line
        objectName: "galleryExamplePanel"
        height: panelColumn.implicitHeight + 2

        Column {
            id: panelColumn

            x: 1
            y: 1
            width: parent.width - 2

            Item {
                width: parent.width
                height: Math.max(52, heading.implicitHeight + 36)
                visible: control.title.length > 0 || control.description.length > 0

                EIcon {
                    x: 24
                    y: 19
                    iconSize: 16
                    iconSource: EIcons.CODE
                    color: GalleryStyle.accent
                }
                Column {
                    id: heading

                    x: 50
                    y: 18
                    spacing: 6
                    width: parent.width - 74

                    EText {
                        text: control.title
                        width: parent.width
                        wrapMode: Text.WordWrap
                        color: GalleryStyle.text
                        visible: text.length > 0
                        font.weight: Font.DemiBold
                        font.pixelSize: GalleryStyle.headingSize
                    }
                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        visible: text.length > 0
                        color: GalleryStyle.muted
                        text: control.description
                        font.pixelSize: GalleryStyle.bodySize
                        lineHeight: GalleryStyle.bodyLineHeight
                    }
                }
            }
            ERectangle {
                width: parent.width
                color: GalleryStyle.preview
                objectName: "galleryPreview"
                height: Math.max(96, previewContent.implicitHeight + 48)

                ERectangle {
                    height: 1
                    width: parent.width
                    color: GalleryStyle.line
                }
                Column {
                    id: previewContent

                    y: 24
                    spacing: 16
                    anchors.leftMargin: 24
                    anchors.rightMargin: 24
                    anchors.left: parent.left
                    anchors.right: parent.right
                    objectName: "galleryPreviewContent"
                }
            }
            CodeExpander {
                code: control.code
                width: parent.width
                visible: control.code.length > 0
            }
        }
    }
}
