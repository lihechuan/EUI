import EUI 1.0
import EUI.Private 1.0
import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: control

    property string code: ""
    property bool expanded: false
    property int maximumSourceHeight: 360

    readonly property int sourceLineCount: code.split(/\r\n|\r|\n/).length

    function sourceParagraph(contents) {
        return '<p style="margin:0; line-height:22px; -qt-line-height-type:fixed">' + contents + '</p>'
    }

    function highlightedCode() {
        return sourceParagraph(exampleData.highlightCode(code, {
            plain: GalleryStyle.text,
            type: GalleryStyle.dark ? "#71CED1" : "#087F8C",
            property: GalleryStyle.dark ? "#89B4FA" : "#315EB5",
            keyword: GalleryStyle.dark ? "#C4A7E7" : "#8255AD",
            literal: GalleryStyle.dark ? "#C4A7E7" : "#8255AD",
            string: GalleryStyle.dark ? "#A6D3A0" : "#27775B",
            number: GalleryStyle.dark ? "#E6B673" : "#A15C21",
            member: GalleryStyle.dark ? "#89B4FA" : "#315EB5",
            comment: GalleryStyle.muted,
            punctuation: GalleryStyle.muted
        }))
    }

    width: parent ? parent.width : 0
    objectName: "galleryCodeExpander"
    implicitHeight: toolbar.height + (expanded ? sourcePane.height : 0)

    ERectangle {
        id: toolbar

        height: 48
        width: parent.width
        color: "transparent"

        ERectangle {
            height: 1
            width: parent.width
            color: GalleryStyle.line
        }
        ETextButton {
            id: sourceToggle

            iconSize: 14
            minHeight: 32
            leftIcon: true
            font.bold: false
            anchors.leftMargin: 16
            anchors.left: parent.left
            objectName: "gallerySourceToggle"
            font.pixelSize: GalleryStyle.captionSize
            anchors.verticalCenter: parent.verticalCenter
            iconSource: control.expanded ? EIcons.CHEVRON_UP : EIcons.CODE
            text: control.expanded ? ELocale.texts.HideSource : ELocale.texts.ShowSource

            onClicked: control.expanded = !control.expanded
        }
        ERectangle {
            radius: 5
            width: 44
            height: 22
            anchors.leftMargin: 10
            color: GalleryStyle.accentSoft
            anchors.left: sourceToggle.right
            anchors.verticalCenter: parent.verticalCenter
            visible: control.expanded && toolbar.width >= 400

            EText {
                text: "QML"
                font.pixelSize: 11
                anchors.centerIn: parent
                color: GalleryStyle.accent
                font.weight: Font.DemiBold
            }
        }
        ETextButton {
            iconSize: 14
            minHeight: 32
            leftIcon: true
            font.bold: false
            anchors.rightMargin: 16
            iconSource: EIcons.COPY
            anchors.right: parent.right
            text: ELocale.texts.CopySource
            objectName: "gallerySourceCopy"
            font.pixelSize: GalleryStyle.captionSize
            anchors.verticalCenter: parent.verticalCenter

            onClicked: exampleClipboard.setText(control.code)
        }
    }
    ERectangle {
        id: sourcePane

        clip: true
        radius: 10
        width: parent.width
        visible: control.expanded
        anchors.top: toolbar.bottom
        objectName: "gallerySourcePane"
        color: GalleryStyle.dark ? "#1D2129" : "#F6F8FC"
        height: control.expanded ? Math.min(control.maximumSourceHeight, sourceText.contentHeight + 40) : 0

        ERectangle {
            height: 10
            width: parent.width
            color: sourcePane.color
        }
        ERectangle {
            height: 1
            width: parent.width
            color: GalleryStyle.line
        }
        Item {
            id: gutter

            clip: true
            height: parent.height
            width: Math.max(52, String(control.sourceLineCount).length * 9 + 24)

            Text {
                id: lineNumbers

                x: 8
                font.pixelSize: 14
                width: parent.width - 22
                textFormat: Text.RichText
                renderType: Text.QtRendering
                font.family: GalleryStyle.codeFamily
                horizontalAlignment: Text.AlignRight
                objectName: "gallerySourceLineNumbers"
                color: GalleryStyle.dark ? "#777F8F" : "#8992A3"
                y: sourceText.y + codeViewport.y - codeViewport.contentY
                text: control.expanded ? control.sourceParagraph(control.code.split(/\r\n|\r|\n/)
                    .map(function(line, index) { return index + 1 }).join("<br/>")) : ""
            }
            ERectangle {
                y: 12
                width: 1
                height: parent.height - 24
                anchors.right: parent.right
                color: GalleryStyle.dark ? "#343B49" : "#E1E6EF"
            }
        }
        Flickable {
            id: codeViewport

            clip: true
            anchors.margins: 1
            anchors.fill: parent
            anchors.leftMargin: gutter.width
            objectName: "gallerySourceViewport"
            ScrollBar.vertical: ScrollHandle { }
            ScrollBar.horizontal: ScrollHandle { }
            boundsBehavior: Flickable.StopAtBounds
            contentHeight: sourceText.contentHeight + 40
            contentWidth: Math.max(width, sourceText.contentWidth + 40)

            ECopyableText {
                id: sourceText

                x: 20
                y: 20
                font.pixelSize: 14
                selectByMouse: true
                wrapMode: TextEdit.NoWrap
                renderType: Text.QtRendering
                textColor: GalleryStyle.text
                textFormat: TextEdit.RichText
                objectName: "gallerySourceText"
                font.family: GalleryStyle.codeFamily
                width: Math.max(codeViewport.width - 40, contentWidth)
                text: control.expanded ? control.highlightedCode() : ""
            }
        }
    }
}
