import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EDivider"

        ExampleCase {
            objectName: "showcaseEDivider0"
            title: ELocale.texts.DividerLabels
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EDivider {
                        text: "分组"
                        contentPosition: EPlacement.Left
                        width: Math.min(240, parent.width)
                    }
                    EDivider {
                        text: "分组"
                        contentPosition: EPlacement.Center
                        width: Math.min(240, parent.width)
                    }
                    EDivider {
                        text: "分组"
                        contentPosition: EPlacement.Right
                        width: Math.min(240, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EDivider {
                    text: "分组"
                    contentPosition: EPlacement.Left
                    width: Math.min(240, parent.width)
                }
                EDivider {
                    text: "分组"
                    contentPosition: EPlacement.Center
                    width: Math.min(240, parent.width)
                }
                EDivider {
                    text: "分组"
                    contentPosition: EPlacement.Right
                    width: Math.min(240, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDivider"

        ExampleCase {
            objectName: "showcaseEDivider1"
            title: ELocale.texts.DividerPatterns
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EDivider {
                        lineWidth: 2
                        color: "#409EFF"
                        borderStyle: "solid"
                        width: Math.min(240, parent.width)
                    }
                    EDivider {
                        lineWidth: 2
                        color: "#409EFF"
                        borderStyle: "dashed"
                        width: Math.min(240, parent.width)
                    }
                    EDivider {
                        lineWidth: 2
                        color: "#409EFF"
                        borderStyle: "dotted"
                        width: Math.min(240, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EDivider {
                    lineWidth: 2
                    color: "#409EFF"
                    borderStyle: "solid"
                    width: Math.min(240, parent.width)
                }
                EDivider {
                    lineWidth: 2
                    color: "#409EFF"
                    borderStyle: "dashed"
                    width: Math.min(240, parent.width)
                }
                EDivider {
                    lineWidth: 2
                    color: "#409EFF"
                    borderStyle: "dotted"
                    width: Math.min(240, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDivider"

        ExampleCase {
            objectName: "showcaseEDivider2"
            title: ELocale.texts.VerticalDivider2
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EText {
                        text: "设计"
                    }
                    EDivider {
                        verticalHeight: 24
                        direction: EOrientation.Vertical
                    }
                    EText {
                        text: "开发"
                    }
                    EDivider {
                        verticalHeight: 24
                        direction: EOrientation.Vertical
                    }
                    EText {
                        text: "发布"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EText {
                    text: "设计"
                }
                EDivider {
                    verticalHeight: 24
                    direction: EOrientation.Vertical
                }
                EText {
                    text: "开发"
                }
                EDivider {
                    verticalHeight: 24
                    direction: EOrientation.Vertical
                }
                EText {
                    text: "发布"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDivider"

        ExampleCase {
            objectName: "showcaseEDivider3"
            title: ELocale.texts.DividerContent
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EDivider {
                    width: parent.width
                    contentComponent: Component {
                        Row {
                            spacing: 8

                            EIcon {
                                iconSize: 18
                                iconSource: EIcons.PALETTE
                            }
                            EText {
                                text: "高级设置"
                            }
                        }
                    }
                }
            `)

            EDivider {
                width: parent.width
                contentComponent: Component {
                    Row {
                        spacing: 8

                        EIcon {
                            iconSize: 18
                            iconSource: EIcons.PALETTE
                        }
                        EText {
                            text: "高级设置"
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EWatermark"

        ExampleCase {
            objectName: "showcaseEWatermark0"
            title: ELocale.texts.MultilineWatermark
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Item {
                    height: 180
                    width: Math.min(480, parent.width)

                    ECard {
                        header: "项目文档"
                        anchors.fill: parent

                        EText {
                            text: "水印覆盖文档内容。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EWatermark {
                        anchors.fill: parent
                        content: ["EUI / INTERNAL", "2026"]
                    }
                }
            `)

            Item {
                height: 180
                width: Math.min(480, parent.width)

                ECard {
                    header: "项目文档"
                    anchors.fill: parent

                    EText {
                        text: "水印覆盖文档内容。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EWatermark {
                    anchors.fill: parent
                    content: ["EUI / INTERNAL", "2026"]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EWatermark"

        ExampleCase {
            objectName: "showcaseEWatermark1"
            title: ELocale.texts.WatermarkSpacing
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Item {
                    height: 180
                    width: Math.min(480, parent.width)

                    ECard {
                        header: "项目文档"
                        anchors.fill: parent

                        EText {
                            text: "水印覆盖文档内容。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EWatermark {
                        rotate: -35
                        content: "EUI"
                        tileHeight: 64
                        tileWidth: 100
                        anchors.fill: parent
                        gap: Qt.point(32, 24)
                    }
                }
            `)

            Item {
                height: 180
                width: Math.min(480, parent.width)

                ECard {
                    header: "项目文档"
                    anchors.fill: parent

                    EText {
                        text: "水印覆盖文档内容。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EWatermark {
                    rotate: -35
                    content: "EUI"
                    tileHeight: 64
                    tileWidth: 100
                    anchors.fill: parent
                    gap: Qt.point(32, 24)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EWatermark"

        ExampleCase {
            objectName: "showcaseEWatermark2"
            title: ELocale.texts.ImageWatermark
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Item {
                    height: 180
                    width: Math.min(480, parent.width)

                    ECard {
                        header: "项目文档"
                        anchors.fill: parent

                        EText {
                            text: "水印覆盖文档内容。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EWatermark {
                        tileWidth: 72
                        tileHeight: 72
                        anchors.fill: parent
                        image: "qrc:/example/eui-mark.svg"
                    }
                }
            `)

            Item {
                height: 180
                width: Math.min(480, parent.width)

                ECard {
                    header: "项目文档"
                    anchors.fill: parent

                    EText {
                        text: "水印覆盖文档内容。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EWatermark {
                    tileWidth: 72
                    tileHeight: 72
                    anchors.fill: parent
                    image: "qrc:/example/eui-mark.svg"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EWatermark"

        ExampleCase {
            objectName: "showcaseEWatermark3"
            title: ELocale.texts.WatermarkOffset
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Item {
                    height: 180
                    width: Math.min(480, parent.width)

                    ECard {
                        header: "项目文档"
                        anchors.fill: parent

                        EText {
                            text: "水印覆盖文档内容。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EWatermark {
                        rotate: 0
                        color: "#409EFF"
                        font.pixelSize: 18
                        anchors.fill: parent
                        content: "CONFIDENTIAL"
                        offset: Qt.point(30, 10)
                    }
                }
            `)

            Item {
                height: 180
                width: Math.min(480, parent.width)

                ECard {
                    header: "项目文档"
                    anchors.fill: parent

                    EText {
                        text: "水印覆盖文档内容。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EWatermark {
                    rotate: 0
                    color: "#409EFF"
                    font.pixelSize: 18
                    anchors.fill: parent
                    content: "CONFIDENTIAL"
                    offset: Qt.point(30, 10)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMenu"

        ExampleCase {
            objectName: "showcaseEMenu0"
            title: ELocale.texts.HorizontalMenu
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EMenu {
                    width: parent.width
                    currentIndex: "home"
                    direction: EOrientation.Horizontal
                    model: [{index: "home", text: "概览", iconSource: EIcons.HOUSE}, {index: "work", text: "工作区", children: [{index: "design", text: "设计"}, {index: "dev", text: "开发"}]}, {index: "locked", text: "归档", disabled: true}]
                }
            `)

            EMenu {
                width: parent.width
                currentIndex: "home"
                direction: EOrientation.Horizontal
                model: [{index: "home", text: "概览", iconSource: EIcons.HOUSE}, {index: "work", text: "工作区", children: [{index: "design", text: "设计"}, {index: "dev", text: "开发"}]}, {index: "locked", text: "归档", disabled: true}]
            }
        }
    }
    ExampleSection {
        componentName: "EMenu"

        ExampleCase {
            objectName: "showcaseEMenu1"
            title: ELocale.texts.CollapsibleMenu
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        id: collapsed

                        text: "折叠菜单"
                    }
                    EMenu {
                        uniqueOpened: true
                        collapse: collapsed.checked
                        width: Math.min(480, parent.width)
                        model: [{index: "home", text: "概览", iconSource: EIcons.HOUSE}, {index: "work", text: "工作区", children: [{index: "design", text: "设计"}, {index: "dev", text: "开发"}]}, {index: "locked", text: "归档", disabled: true}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESwitch {
                    id: collapsed

                    text: "折叠菜单"
                }
                EMenu {
                    uniqueOpened: true
                    collapse: collapsed.checked
                    width: Math.min(480, parent.width)
                    model: [{index: "home", text: "概览", iconSource: EIcons.HOUSE}, {index: "work", text: "工作区", children: [{index: "design", text: "设计"}, {index: "dev", text: "开发"}]}, {index: "locked", text: "归档", disabled: true}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMenu"

        ExampleCase {
            objectName: "showcaseEMenu2"
            title: ELocale.texts.MenuInitialState
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EMenu {
                    currentIndex: "design"
                    openedIndexes: ["work"]
                    activeTextColor: "#21B6A8"
                    width: Math.min(480, parent.width)
                    model: [{index: "home", text: "概览", iconSource: EIcons.HOUSE}, {index: "work", text: "工作区", children: [{index: "design", text: "设计"}, {index: "dev", text: "开发"}]}, {index: "locked", text: "归档", disabled: true}]
                }
            `)

            EMenu {
                currentIndex: "design"
                openedIndexes: ["work"]
                activeTextColor: "#21B6A8"
                width: Math.min(480, parent.width)
                model: [{index: "home", text: "概览", iconSource: EIcons.HOUSE}, {index: "work", text: "工作区", children: [{index: "design", text: "设计"}, {index: "dev", text: "开发"}]}, {index: "locked", text: "归档", disabled: true}]
            }
        }
    }
}
