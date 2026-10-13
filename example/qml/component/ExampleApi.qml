import EUI 1.0
import EUI.Private 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15

Column {
    id: control

    property string componentName: ""

    readonly property var sections: {
        if (!componentName || typeof exampleCatalog === "undefined")
            return []
        exampleCatalog.groups
        const related = {
            ERow: ["ECol"],
            EContainer: ["EHeader", "EAside", "EMain", "EFooter", "EApplicationWindow"]
        }
        return [componentName].concat(related[componentName] || []).map(function(name) {
            return {name: name, api: exampleCatalog.documentation(name)}
        })
    }

    spacing: 24
    visible: sections.length > 0
    objectName: "galleryApiDocumentation"

    Repeater {
        model: control.sections

        Column {
            id: section

            readonly property var documentation: modelData.api
            readonly property string componentName: modelData.name

            spacing: 12
            width: control.width
            objectName: "galleryApiSection"

            EText {
                width: parent.width
                wrapMode: Text.WordWrap
                color: GalleryStyle.text
                font.weight: Font.DemiBold
                objectName: "galleryApiTitle"
                font.pixelSize: GalleryStyle.sectionSize
                text: section.componentName + " " + ELocale.texts.ApiAttributes
            }
            EText {
                width: parent.width
                wrapMode: Text.WordWrap
                color: GalleryStyle.muted
                text: ELocale.texts.ApiPropertiesNote
                lineHeight: GalleryStyle.bodyLineHeight
                font.pixelSize: GalleryStyle.captionSize
            }
            Flickable {
                id: tableViewport

                clip: true
                width: parent.width
                contentWidth: table.width
                contentHeight: table.height
                objectName: "galleryApiTableViewport"
                ScrollBar.horizontal: ScrollHandle { }
                boundsBehavior: Flickable.StopAtBounds
                flickableDirection: Flickable.HorizontalFlick
                height: table.height + (contentWidth > width ? ScrollBar.horizontal.height : 0)

                Column {
                    id: table

                    width: Math.max(820, tableViewport.width)

                    Rectangle {
                        height: 42
                        width: parent.width
                        color: GalleryStyle.hover
                        objectName: "galleryApiTableHeader"

                        Row {
                            width: parent.width
                            height: parent.height

                            Repeater {
                                model: [ELocale.texts.ApiName, ELocale.texts.ApiDescription,
                                    ELocale.texts.ApiType, ELocale.texts.ApiDefault]

                                EText {
                                    leftPadding: 16
                                    text: modelData
                                    rightPadding: 16
                                    height: parent.height
                                    color: GalleryStyle.text
                                    font.weight: Font.DemiBold
                                    verticalAlignment: Text.AlignVCenter
                                    font.pixelSize: GalleryStyle.captionSize
                                    width: table.width * [0.22, 0.38, 0.15, 0.25][index]
                                }
                            }
                        }
                    }
                    Repeater {
                        model: section.documentation.properties || []

                        Rectangle {
                            id: row

                            readonly property var entry: modelData

                            width: table.width
                            color: GalleryStyle.surface
                            objectName: "galleryApiRow"
                            height: Math.max(64, nameColumn.implicitHeight + 28,
                                descriptionLabel.implicitHeight + 28, typeLabel.implicitHeight + 28,
                                defaultColumn.implicitHeight + 28)

                            Column {
                                id: nameColumn

                                x: 16
                                y: 14
                                spacing: 5
                                width: table.width * 0.22 - 32

                                TextEdit {
                                    readOnly: true
                                    selectByMouse: true
                                    width: parent.width
                                    text: row.entry.name
                                    color: GalleryStyle.text
                                    wrapMode: TextEdit.WrapAnywhere
                                    font.family: GalleryStyle.codeFamily
                                    font.pixelSize: GalleryStyle.captionSize
                                }
                                EText {
                                    width: parent.width
                                    color: GalleryStyle.muted
                                    wrapMode: Text.WrapAnywhere
                                    font.pixelSize: GalleryStyle.smallSize
                                    text: row.entry.owner + (row.entry.readonly ? " · " + ELocale.texts.ApiReadOnly : "")
                                }
                            }
                            TextEdit {
                                id: descriptionLabel

                                y: 14
                                readOnly: true
                                selectByMouse: true
                                color: GalleryStyle.text
                                x: table.width * 0.22 + 16
                                text: row.entry.description
                                wrapMode: TextEdit.WordWrap
                                width: table.width * 0.38 - 32
                                font.pixelSize: GalleryStyle.captionSize
                            }
                            TextEdit {
                                id: typeLabel

                                y: 14
                                readOnly: true
                                selectByMouse: true
                                text: row.entry.type
                                x: table.width * 0.6 + 16
                                color: GalleryStyle.accent
                                width: table.width * 0.15 - 32
                                wrapMode: TextEdit.WrapAnywhere
                                font.family: GalleryStyle.codeFamily
                                font.pixelSize: GalleryStyle.captionSize
                            }
                            Column {
                                id: defaultColumn

                                y: 14
                                spacing: 6
                                x: table.width * 0.75 + 16
                                width: table.width * 0.25 - 32

                                TextEdit {
                                    readOnly: true
                                    selectByMouse: true
                                    width: parent.width
                                    color: GalleryStyle.text
                                    text: row.entry.defaultValue
                                    wrapMode: TextEdit.WrapAnywhere
                                    font.family: GalleryStyle.codeFamily
                                    font.pixelSize: GalleryStyle.smallSize
                                }
                                ETextButton {
                                    minHeight: 24
                                    leftPadding: 0
                                    rightPadding: 0
                                    text: ELocale.texts.ApiDefaultBinding
                                    font.pixelSize: GalleryStyle.smallSize
                                    objectName: "galleryApiDefaultBinding"
                                    visible: !row.entry.readonly && row.entry.binding.length > 0
                                        && row.entry.defaultValue.indexOf(row.entry.binding) < 0

                                    onClicked: bindingDialog.openBinding(section.componentName, row.entry)
                                }
                            }
                            Rectangle {
                                height: 1
                                width: parent.width
                                color: GalleryStyle.line
                                anchors.bottom: parent.bottom
                            }
                        }
                    }
                }
            }
        }
    }
    Dialog {
        id: bindingDialog

        property string binding: ""
        property string sourceFile: ""

        function openBinding(componentName, entry) {
            title = componentName + "." + entry.name
            binding = entry.binding
            sourceFile = entry.source
            open()
        }

        focus: true
        modal: true
        standardButtons: Dialog.Close
        anchors.centerIn: Overlay.overlay
        objectName: "galleryApiBindingDialog"
        width: Math.min(640, Overlay.overlay ? Overlay.overlay.width - 48 : 640)
        background: Rectangle {
            radius: 8
            border.width: 1
            color: GalleryStyle.surface
            border.color: GalleryStyle.line
        }
        contentItem: Column {
            spacing: 12

            TextEdit {
                readOnly: true
                selectByMouse: true
                width: parent.width
                color: GalleryStyle.text
                text: bindingDialog.binding
                wrapMode: TextEdit.WrapAnywhere
                font.family: GalleryStyle.codeFamily
                font.pixelSize: GalleryStyle.captionSize
            }
            EText {
                width: parent.width
                color: GalleryStyle.muted
                wrapMode: Text.WrapAnywhere
                text: bindingDialog.sourceFile
                font.pixelSize: GalleryStyle.smallSize
            }
        }
    }
}
