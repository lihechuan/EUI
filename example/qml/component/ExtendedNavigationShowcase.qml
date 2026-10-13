import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EAffix"

        ExampleCase {
            objectName: "showcaseEAffix0"
            title: ELocale.texts.TopAffixOffset
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 20
                        width: parent.width

                        Item {
                            width: 1
                            height: 60
                        }
                        EAffix {
                            offset: 12
                            target: view

                            EButton {
                                text: "固定操作"
                                styleType: EStyle.Primary
                            }
                        }
                        EText {
                            height: 400
                            text: "滚动容器查看固定效果"
                        }
                    }
                }
            `)

            Flickable {
                id: view

                clip: true
                height: 200
                contentHeight: 600
                width: Math.min(480, parent.width)

                Column {
                    spacing: 20
                    width: parent.width

                    Item {
                        width: 1
                        height: 60
                    }
                    EAffix {
                        offset: 12
                        target: view

                        EButton {
                            text: "固定操作"
                            styleType: EStyle.Primary
                        }
                    }
                    EText {
                        height: 400
                        text: "滚动容器查看固定效果"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAffix"

        ExampleCase {
            objectName: "showcaseEAffix1"
            title: ELocale.texts.BottomAffix
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 20
                        width: parent.width

                        Item {
                            width: 1
                            height: 60
                        }
                        EAffix {
                            offset: 12
                            target: view
                            position: EPlacement.Bottom

                            EButton {
                                text: "固定操作"
                                styleType: EStyle.Primary
                            }
                        }
                        EText {
                            height: 400
                            text: "滚动容器查看固定效果"
                        }
                    }
                }
            `)

            Flickable {
                id: view

                clip: true
                height: 200
                contentHeight: 600
                width: Math.min(480, parent.width)

                Column {
                    spacing: 20
                    width: parent.width

                    Item {
                        width: 1
                        height: 60
                    }
                    EAffix {
                        offset: 12
                        target: view
                        position: EPlacement.Bottom

                        EButton {
                            text: "固定操作"
                            styleType: EStyle.Primary
                        }
                    }
                    EText {
                        height: 400
                        text: "滚动容器查看固定效果"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAffix"

        ExampleCase {
            objectName: "showcaseEAffix2"
            title: ELocale.texts.AffixedContent
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 20
                        width: parent.width

                        Item {
                            width: 1
                            height: 60
                        }
                        EAffix {
                            target: view

                            EButton {
                                text: "固定操作"
                                styleType: EStyle.Primary
                            }
                        }
                        EText {
                            height: 400
                            text: "滚动容器查看固定效果"
                        }
                    }
                }
            `)

            Flickable {
                id: view

                clip: true
                height: 200
                contentHeight: 600
                width: Math.min(480, parent.width)

                Column {
                    spacing: 20
                    width: parent.width

                    Item {
                        width: 1
                        height: 60
                    }
                    EAffix {
                        target: view

                        EButton {
                            text: "固定操作"
                            styleType: EStyle.Primary
                        }
                    }
                    EText {
                        height: 400
                        text: "滚动容器查看固定效果"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAffix"

        ExampleCase {
            objectName: "showcaseEAffix3"
            title: ELocale.texts.AffixStatus
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 20
                            width: parent.width

                            Item {
                                width: 1
                                height: 60
                            }
                            EAffix {
                                target: view

                                onChanged: result.text = fixed ? "已固定" : "跟随内容"

                                EButton {
                                    text: "固定操作"
                                    styleType: EStyle.Primary
                                }
                            }
                            EText {
                                height: 400
                                text: "滚动容器查看固定效果"
                            }
                        }
                    }
                    EText {
                        id: result

                        text: "跟随内容"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 20
                        width: parent.width

                        Item {
                            width: 1
                            height: 60
                        }
                        EAffix {
                            target: view

                            onChanged: result.text = fixed ? "已固定" : "跟随内容"

                            EButton {
                                text: "固定操作"
                                styleType: EStyle.Primary
                            }
                        }
                        EText {
                            height: 400
                            text: "滚动容器查看固定效果"
                        }
                    }
                }
                EText {
                    id: result

                    text: "跟随内容"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAnchor"

        ExampleCase {
            objectName: "showcaseEAnchor0"
            title: ELocale.texts.LinkedSections
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EAnchor {
                        marker: true
                        target: view
                        width: Math.min(480, parent.width)
                        model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                    }
                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                id: first

                                height: 200
                                text: "设计规范"
                            }
                            EText {
                                id: second

                                height: 200
                                text: "开发规范"
                            }
                            EText {
                                id: third

                                height: 200
                                text: "发布规范"
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EAnchor {
                    marker: true
                    target: view
                    width: Math.min(480, parent.width)
                    model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                }
                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            id: first

                            height: 200
                            text: "设计规范"
                        }
                        EText {
                            id: second

                            height: 200
                            text: "开发规范"
                        }
                        EText {
                            id: third

                            height: 200
                            text: "发布规范"
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAnchor"

        ExampleCase {
            objectName: "showcaseEAnchor1"
            title: ELocale.texts.HorizontalAnchor
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EAnchor {
                        target: view
                        direction: EOrientation.Horizontal
                        width: Math.min(480, parent.width)
                        model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                    }
                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                id: first

                                height: 200
                                text: "设计规范"
                            }
                            EText {
                                id: second

                                height: 200
                                text: "开发规范"
                            }
                            EText {
                                id: third

                                height: 200
                                text: "发布规范"
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EAnchor {
                    target: view
                    direction: EOrientation.Horizontal
                    width: Math.min(480, parent.width)
                    model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                }
                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            id: first

                            height: 200
                            text: "设计规范"
                        }
                        EText {
                            id: second

                            height: 200
                            text: "开发规范"
                        }
                        EText {
                            id: third

                            height: 200
                            text: "发布规范"
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAnchor"

        ExampleCase {
            objectName: "showcaseEAnchor2"
            title: ELocale.texts.AnimatedAnchor
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EAnchor {
                        offset: 12
                        target: view
                        duration: 350
                        width: Math.min(480, parent.width)
                        model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                    }
                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                id: first

                                height: 200
                                text: "设计规范"
                            }
                            EText {
                                id: second

                                height: 200
                                text: "开发规范"
                            }
                            EText {
                                id: third

                                height: 200
                                text: "发布规范"
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EAnchor {
                    offset: 12
                    target: view
                    duration: 350
                    width: Math.min(480, parent.width)
                    model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                }
                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            id: first

                            height: 200
                            text: "设计规范"
                        }
                        EText {
                            id: second

                            height: 200
                            text: "开发规范"
                        }
                        EText {
                            id: third

                            height: 200
                            text: "发布规范"
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAnchor"

        ExampleCase {
            objectName: "showcaseEAnchor3"
            title: ELocale.texts.DisabledAnchor
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EAnchor {
                        target: view
                        enabled: false
                        width: Math.min(480, parent.width)
                        model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                    }
                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                id: first

                                height: 200
                                text: "设计规范"
                            }
                            EText {
                                id: second

                                height: 200
                                text: "开发规范"
                            }
                            EText {
                                id: third

                                height: 200
                                text: "发布规范"
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EAnchor {
                    target: view
                    enabled: false
                    width: Math.min(480, parent.width)
                    model: [{title: "设计", target: first}, {title: "开发", target: second}, {title: "发布", target: third}]
                }
                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            id: first

                            height: 200
                            text: "设计规范"
                        }
                        EText {
                            id: second

                            height: 200
                            text: "开发规范"
                        }
                        EText {
                            id: third

                            height: 200
                            text: "发布规范"
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EBacktop"

        ExampleCase {
            objectName: "showcaseEBacktop0"
            title: ELocale.texts.VisibilityThreshold
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentY: 120
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                height: 48
                                text: "项目 1"
                            }
                            EText {
                                height: 48
                                text: "项目 2"
                            }
                            EText {
                                height: 48
                                text: "项目 3"
                            }
                            EText {
                                height: 48
                                text: "项目 4"
                            }
                            EText {
                                height: 48
                                text: "项目 5"
                            }
                            EText {
                                height: 48
                                text: "项目 6"
                            }
                            EText {
                                height: 48
                                text: "项目 7"
                            }
                            EText {
                                height: 48
                                text: "项目 8"
                            }
                            EText {
                                height: 48
                                text: "项目 9"
                            }
                            EText {
                                height: 48
                                text: "项目 10"
                            }
                            EText {
                                height: 48
                                text: "项目 11"
                            }
                            EText {
                                height: 48
                                text: "项目 12"
                            }
                        }
                    }
                    EBacktop {
                        target: view
                        visibilityHeight: 80
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentY: 120
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            height: 48
                            text: "项目 1"
                        }
                        EText {
                            height: 48
                            text: "项目 2"
                        }
                        EText {
                            height: 48
                            text: "项目 3"
                        }
                        EText {
                            height: 48
                            text: "项目 4"
                        }
                        EText {
                            height: 48
                            text: "项目 5"
                        }
                        EText {
                            height: 48
                            text: "项目 6"
                        }
                        EText {
                            height: 48
                            text: "项目 7"
                        }
                        EText {
                            height: 48
                            text: "项目 8"
                        }
                        EText {
                            height: 48
                            text: "项目 9"
                        }
                        EText {
                            height: 48
                            text: "项目 10"
                        }
                        EText {
                            height: 48
                            text: "项目 11"
                        }
                        EText {
                            height: 48
                            text: "项目 12"
                        }
                    }
                }
                EBacktop {
                    target: view
                    visibilityHeight: 80
                }
            }
        }
    }
    ExampleSection {
        componentName: "EBacktop"

        ExampleCase {
            objectName: "showcaseEBacktop1"
            title: ELocale.texts.AlwaysVisibleBacktop
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentY: 120
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                height: 48
                                text: "项目 1"
                            }
                            EText {
                                height: 48
                                text: "项目 2"
                            }
                            EText {
                                height: 48
                                text: "项目 3"
                            }
                            EText {
                                height: 48
                                text: "项目 4"
                            }
                            EText {
                                height: 48
                                text: "项目 5"
                            }
                            EText {
                                height: 48
                                text: "项目 6"
                            }
                            EText {
                                height: 48
                                text: "项目 7"
                            }
                            EText {
                                height: 48
                                text: "项目 8"
                            }
                            EText {
                                height: 48
                                text: "项目 9"
                            }
                            EText {
                                height: 48
                                text: "项目 10"
                            }
                            EText {
                                height: 48
                                text: "项目 11"
                            }
                            EText {
                                height: 48
                                text: "项目 12"
                            }
                        }
                    }
                    EBacktop {
                        target: view
                        visibilityHeight: 0
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentY: 120
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            height: 48
                            text: "项目 1"
                        }
                        EText {
                            height: 48
                            text: "项目 2"
                        }
                        EText {
                            height: 48
                            text: "项目 3"
                        }
                        EText {
                            height: 48
                            text: "项目 4"
                        }
                        EText {
                            height: 48
                            text: "项目 5"
                        }
                        EText {
                            height: 48
                            text: "项目 6"
                        }
                        EText {
                            height: 48
                            text: "项目 7"
                        }
                        EText {
                            height: 48
                            text: "项目 8"
                        }
                        EText {
                            height: 48
                            text: "项目 9"
                        }
                        EText {
                            height: 48
                            text: "项目 10"
                        }
                        EText {
                            height: 48
                            text: "项目 11"
                        }
                        EText {
                            height: 48
                            text: "项目 12"
                        }
                    }
                }
                EBacktop {
                    target: view
                    visibilityHeight: 0
                }
            }
        }
    }
    ExampleSection {
        componentName: "EBacktop"

        ExampleCase {
            objectName: "showcaseEBacktop2"
            title: ELocale.texts.BacktopIcon
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentY: 120
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                height: 48
                                text: "项目 1"
                            }
                            EText {
                                height: 48
                                text: "项目 2"
                            }
                            EText {
                                height: 48
                                text: "项目 3"
                            }
                            EText {
                                height: 48
                                text: "项目 4"
                            }
                            EText {
                                height: 48
                                text: "项目 5"
                            }
                            EText {
                                height: 48
                                text: "项目 6"
                            }
                            EText {
                                height: 48
                                text: "项目 7"
                            }
                            EText {
                                height: 48
                                text: "项目 8"
                            }
                            EText {
                                height: 48
                                text: "项目 9"
                            }
                            EText {
                                height: 48
                                text: "项目 10"
                            }
                            EText {
                                height: 48
                                text: "项目 11"
                            }
                            EText {
                                height: 48
                                text: "项目 12"
                            }
                        }
                    }
                    EBacktop {
                        target: view
                        visibilityHeight: 0
                        iconSource: EIcons.ARROW_UP_FROM_LINE
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentY: 120
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            height: 48
                            text: "项目 1"
                        }
                        EText {
                            height: 48
                            text: "项目 2"
                        }
                        EText {
                            height: 48
                            text: "项目 3"
                        }
                        EText {
                            height: 48
                            text: "项目 4"
                        }
                        EText {
                            height: 48
                            text: "项目 5"
                        }
                        EText {
                            height: 48
                            text: "项目 6"
                        }
                        EText {
                            height: 48
                            text: "项目 7"
                        }
                        EText {
                            height: 48
                            text: "项目 8"
                        }
                        EText {
                            height: 48
                            text: "项目 9"
                        }
                        EText {
                            height: 48
                            text: "项目 10"
                        }
                        EText {
                            height: 48
                            text: "项目 11"
                        }
                        EText {
                            height: 48
                            text: "项目 12"
                        }
                    }
                }
                EBacktop {
                    target: view
                    visibilityHeight: 0
                    iconSource: EIcons.ARROW_UP_FROM_LINE
                }
            }
        }
    }
    ExampleSection {
        componentName: "EBacktop"

        ExampleCase {
            objectName: "showcaseEBacktop3"
            title: ELocale.texts.DisabledBacktop
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flickable {
                        id: view

                        clip: true
                        height: 200
                        contentY: 120
                        contentHeight: 600
                        width: Math.min(480, parent.width)

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                height: 48
                                text: "项目 1"
                            }
                            EText {
                                height: 48
                                text: "项目 2"
                            }
                            EText {
                                height: 48
                                text: "项目 3"
                            }
                            EText {
                                height: 48
                                text: "项目 4"
                            }
                            EText {
                                height: 48
                                text: "项目 5"
                            }
                            EText {
                                height: 48
                                text: "项目 6"
                            }
                            EText {
                                height: 48
                                text: "项目 7"
                            }
                            EText {
                                height: 48
                                text: "项目 8"
                            }
                            EText {
                                height: 48
                                text: "项目 9"
                            }
                            EText {
                                height: 48
                                text: "项目 10"
                            }
                            EText {
                                height: 48
                                text: "项目 11"
                            }
                            EText {
                                height: 48
                                text: "项目 12"
                            }
                        }
                    }
                    EBacktop {
                        target: view
                        enabled: false
                        visibilityHeight: 0
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flickable {
                    id: view

                    clip: true
                    height: 200
                    contentY: 120
                    contentHeight: 600
                    width: Math.min(480, parent.width)

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            height: 48
                            text: "项目 1"
                        }
                        EText {
                            height: 48
                            text: "项目 2"
                        }
                        EText {
                            height: 48
                            text: "项目 3"
                        }
                        EText {
                            height: 48
                            text: "项目 4"
                        }
                        EText {
                            height: 48
                            text: "项目 5"
                        }
                        EText {
                            height: 48
                            text: "项目 6"
                        }
                        EText {
                            height: 48
                            text: "项目 7"
                        }
                        EText {
                            height: 48
                            text: "项目 8"
                        }
                        EText {
                            height: 48
                            text: "项目 9"
                        }
                        EText {
                            height: 48
                            text: "项目 10"
                        }
                        EText {
                            height: 48
                            text: "项目 11"
                        }
                        EText {
                            height: 48
                            text: "项目 12"
                        }
                    }
                }
                EBacktop {
                    target: view
                    enabled: false
                    visibilityHeight: 0
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPageHeader"

        ExampleCase {
            objectName: "showcaseEPageHeader0"
            title: ELocale.texts.HeaderWithoutBack
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EPageHeader {
                    title: "工作台"
                    content: "项目详情"
                    showBack: false
                    width: parent.width
                }
            `)

            EPageHeader {
                title: "工作台"
                content: "项目详情"
                showBack: false
                width: parent.width
            }
        }
    }
    ExampleSection {
        componentName: "EPageHeader"

        ExampleCase {
            title: ELocale.texts.HeaderIcon
            objectName: "showcaseEPageHeader1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EPageHeader {
                    title: "工作台"
                    content: "项目详情"
                    width: parent.width
                    iconSource: EIcons.HOUSE
                }
            `)

            EPageHeader {
                title: "工作台"
                content: "项目详情"
                width: parent.width
                iconSource: EIcons.HOUSE
            }
        }
    }
    ExampleSection {
        componentName: "EPageHeader"

        ExampleCase {
            objectName: "showcaseEPageHeader2"
            title: ELocale.texts.HeaderActions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EPageHeader {
                    title: "工作台"
                    content: "项目详情"
                    width: parent.width

                    EButton {
                        text: "编辑"
                        size: EStyle.Small
                        styleType: EStyle.Primary
                    }
                }
            `)

            EPageHeader {
                title: "工作台"
                content: "项目详情"
                width: parent.width

                EButton {
                    text: "编辑"
                    size: EStyle.Small
                    styleType: EStyle.Primary
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPageHeader"

        ExampleCase {
            title: ELocale.texts.HeaderBody
            objectName: "showcaseEPageHeader3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EPageHeader {
                    title: "工作台"
                    content: "项目详情"
                    width: parent.width
                    bodyComponent: Component {
                        EText {
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: "在页面标题下展示当前项目的说明与状态。"
                        }
                    }
                }
            `)

            EPageHeader {
                title: "工作台"
                content: "项目详情"
                width: parent.width
                bodyComponent: Component {
                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: "在页面标题下展示当前项目的说明与状态。"
                    }
                }
            }
        }
    }
}
