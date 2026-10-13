import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EAlert"

        ExampleCase {
            objectName: "showcaseEAlert0"
            title: ELocale.texts.SemanticFeedback
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EAlert {
                        title: "项目状态"
                        showIcon: true
                        styleType: EStyle.Primary
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                    EAlert {
                        title: "项目状态"
                        showIcon: true
                        styleType: EStyle.Success
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                    EAlert {
                        title: "项目状态"
                        showIcon: true
                        styleType: EStyle.Warning
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                    EAlert {
                        title: "项目状态"
                        showIcon: true
                        styleType: EStyle.Danger
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EAlert {
                    title: "项目状态"
                    showIcon: true
                    styleType: EStyle.Primary
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
                EAlert {
                    title: "项目状态"
                    showIcon: true
                    styleType: EStyle.Success
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
                EAlert {
                    title: "项目状态"
                    showIcon: true
                    styleType: EStyle.Warning
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
                EAlert {
                    title: "项目状态"
                    showIcon: true
                    styleType: EStyle.Danger
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAlert"

        ExampleCase {
            title: ELocale.texts.Effects
            objectName: "showcaseEAlert1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EAlert {
                        title: "项目状态"
                        showIcon: true
                        effect: "light"
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                    EAlert {
                        title: "项目状态"
                        effect: "dark"
                        showIcon: true
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                    EAlert {
                        title: "项目状态"
                        showIcon: true
                        effect: "plain"
                        description: "此示例展示不同语义颜色下的反馈信息。"
                        width: Math.min(360, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EAlert {
                    title: "项目状态"
                    showIcon: true
                    effect: "light"
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
                EAlert {
                    title: "项目状态"
                    effect: "dark"
                    showIcon: true
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
                EAlert {
                    title: "项目状态"
                    showIcon: true
                    effect: "plain"
                    description: "此示例展示不同语义颜色下的反馈信息。"
                    width: Math.min(360, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAlert"

        ExampleCase {
            objectName: "showcaseEAlert2"
            title: ELocale.texts.CenteredCloseAction
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAlert {
                    center: true
                    closable: true
                    showIcon: true
                    title: "更新已就绪"
                    closeText: "知道了"
                    description: "点击右侧文字关闭本条信息。"
                    width: Math.min(480, parent.width)
                }
            `)

            EAlert {
                center: true
                closable: true
                showIcon: true
                title: "更新已就绪"
                closeText: "知道了"
                description: "点击右侧文字关闭本条信息。"
                width: Math.min(480, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "EAlert"

        ExampleCase {
            objectName: "showcaseEAlert3"
            title: ELocale.texts.PersistentAnnouncement
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAlert {
                    title: "只读公告"
                    showIcon: true
                    closable: false
                    styleType: EStyle.Info
                    description: "重要信息可以持续展示。"
                    width: Math.min(480, parent.width)
                }
            `)

            EAlert {
                title: "只读公告"
                showIcon: true
                closable: false
                styleType: EStyle.Info
                description: "重要信息可以持续展示。"
                width: Math.min(480, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "EDrawer"

        ExampleCase {
            objectName: "showcaseEDrawer0"
            title: ELocale.texts.LeftDrawer
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开抽屉"

                        onClicked: panel.open()
                    }
                    EDrawer {
                        id: panel

                        title: "项目详情"
                        drawerSize: 360
                        edge: Qt.LeftEdge

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                text: "在抽屉中处理项目内容。"
                                width: parent.width
                                wrapMode: Text.WordWrap
                            }
                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                            EButton {
                                text: "完成"
                                styleType: EStyle.Primary

                                onClicked: panel.close()
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开抽屉"

                    onClicked: panel.open()
                }
                EDrawer {
                    id: panel

                    title: "项目详情"
                    drawerSize: 360
                    edge: Qt.LeftEdge

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            text: "在抽屉中处理项目内容。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                        EButton {
                            text: "完成"
                            styleType: EStyle.Primary

                            onClicked: panel.close()
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDrawer"

        ExampleCase {
            objectName: "showcaseEDrawer1"
            title: ELocale.texts.BottomDrawer
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开抽屉"

                        onClicked: panel.open()
                    }
                    EDrawer {
                        id: panel

                        title: "项目详情"
                        drawerSize: "45%"
                        edge: Qt.BottomEdge

                        Column {
                            spacing: 16
                            width: parent.width

                            EText {
                                text: "在抽屉中处理项目内容。"
                                width: parent.width
                                wrapMode: Text.WordWrap
                            }
                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                            EButton {
                                text: "完成"
                                styleType: EStyle.Primary

                                onClicked: panel.close()
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开抽屉"

                    onClicked: panel.open()
                }
                EDrawer {
                    id: panel

                    title: "项目详情"
                    drawerSize: "45%"
                    edge: Qt.BottomEdge

                    Column {
                        spacing: 16
                        width: parent.width

                        EText {
                            text: "在抽屉中处理项目内容。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                        EButton {
                            text: "完成"
                            styleType: EStyle.Primary

                            onClicked: panel.close()
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDrawer"

        ExampleCase {
            objectName: "showcaseEDrawer2"
            title: ELocale.texts.ResizableDrawerFooter
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开可调整抽屉"

                        onClicked: panel.open()
                    }
                    EDrawer {
                        id: panel

                        title: "项目资料"
                        resizable: true
                        minimumSize: 240
                        drawerSize: "40%"
                        footerComponent: Component {
                            EButton {
                                text: "完成"

                                onClicked: panel.close()
                            }
                        }

                        EText {
                            text: "拖动抽屉边缘调整尺寸。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开可调整抽屉"

                    onClicked: panel.open()
                }
                EDrawer {
                    id: panel

                    title: "项目资料"
                    resizable: true
                    minimumSize: 240
                    drawerSize: "40%"
                    footerComponent: Component {
                        EButton {
                            text: "完成"

                            onClicked: panel.close()
                        }
                    }

                    EText {
                        text: "拖动抽屉边缘调整尺寸。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDialog"

        ExampleCase {
            objectName: "showcaseEDialog0"
            title: ELocale.texts.DialogActions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开对话框"

                        onClicked: panel.open()
                    }
                    EDialog {
                        id: panel

                        center: true
                        title: "项目设置"
                        cancelText: "返回"
                        confirmText: "保存"
                        text: "调整项目设置后继续操作。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开对话框"

                    onClicked: panel.open()
                }
                EDialog {
                    id: panel

                    center: true
                    title: "项目设置"
                    cancelText: "返回"
                    confirmText: "保存"
                    text: "调整项目设置后继续操作。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDialog"

        ExampleCase {
            objectName: "showcaseEDialog1"
            title: ELocale.texts.ModelessDialog
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开对话框"

                        onClicked: panel.open()
                    }
                    EDialog {
                        id: panel

                        modal: false
                        title: "项目设置"
                        draggable: true
                        showFooter: false
                        text: "调整项目设置后继续操作。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开对话框"

                    onClicked: panel.open()
                }
                EDialog {
                    id: panel

                    modal: false
                    title: "项目设置"
                    draggable: true
                    showFooter: false
                    text: "调整项目设置后继续操作。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDialog"

        ExampleCase {
            objectName: "showcaseEDialog2"
            title: ELocale.texts.FullscreenDialog
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开全屏对话框"

                        onClicked: panel.open()
                    }
                    EDialog {
                        id: panel

                        title: "全屏工作区"
                        fullscreen: true
                        text: "适合展示较多内容的编辑面板。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开全屏对话框"

                    onClicked: panel.open()
                }
                EDialog {
                    id: panel

                    title: "全屏工作区"
                    fullscreen: true
                    text: "适合展示较多内容的编辑面板。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopover"

        ExampleCase {
            objectName: "showcaseEPopover0"
            title: ELocale.texts.PopupPlacement
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"
                    }
                    EPopover {
                        id: panel

                        title: "项目操作"
                        reference: target
                        text: "此操作将在当前项目中生效。"
                        placement: EPlacement.Right
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"
                }
                EPopover {
                    id: panel

                    title: "项目操作"
                    reference: target
                    text: "此操作将在当前项目中生效。"
                    placement: EPlacement.Right
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopover"

        ExampleCase {
            objectName: "showcaseEPopover1"
            title: ELocale.texts.HoverDelays
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"
                    }
                    EPopover {
                        id: panel

                        title: "项目操作"
                        hideAfter: 200
                        showAfter: 300
                        trigger: "hover"
                        reference: target
                        text: "此操作将在当前项目中生效。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"
                }
                EPopover {
                    id: panel

                    title: "项目操作"
                    hideAfter: 200
                    showAfter: 300
                    trigger: "hover"
                    reference: target
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopover"

        ExampleCase {
            objectName: "showcaseEPopover2"
            title: ELocale.texts.ArrowAndEffect
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"
                    }
                    EPopover {
                        id: panel

                        title: "项目操作"
                        effect: "dark"
                        showArrow: false
                        reference: target
                        text: "此操作将在当前项目中生效。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"
                }
                EPopover {
                    id: panel

                    title: "项目操作"
                    effect: "dark"
                    showArrow: false
                    reference: target
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopover"

        ExampleCase {
            objectName: "showcaseEPopover3"
            title: ELocale.texts.ManualPopup
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"

                        onClicked: panel.open()
                    }
                    EPopover {
                        id: panel

                        title: "项目操作"
                        reference: target
                        trigger: "manual"
                        text: "此操作将在当前项目中生效。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"

                    onClicked: panel.open()
                }
                EPopover {
                    id: panel

                    title: "项目操作"
                    reference: target
                    trigger: "manual"
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopconfirm"

        ExampleCase {
            objectName: "showcaseEPopconfirm0"
            title: ELocale.texts.PopupPlacement
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"
                    }
                    EPopconfirm {
                        id: panel

                        title: "项目操作"
                        reference: target
                        text: "此操作将在当前项目中生效。"
                        placement: EPlacement.Right
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"
                }
                EPopconfirm {
                    id: panel

                    title: "项目操作"
                    reference: target
                    text: "此操作将在当前项目中生效。"
                    placement: EPlacement.Right
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopconfirm"

        ExampleCase {
            title: ELocale.texts.HoverDelays
            objectName: "showcaseEPopconfirm1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"
                    }
                    EPopconfirm {
                        id: panel

                        title: "项目操作"
                        hideAfter: 200
                        showAfter: 300
                        trigger: "hover"
                        reference: target
                        text: "此操作将在当前项目中生效。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"
                }
                EPopconfirm {
                    id: panel

                    title: "项目操作"
                    hideAfter: 200
                    showAfter: 300
                    trigger: "hover"
                    reference: target
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopconfirm"

        ExampleCase {
            objectName: "showcaseEPopconfirm2"
            title: ELocale.texts.ArrowAndEffect
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"
                    }
                    EPopconfirm {
                        id: panel

                        title: "项目操作"
                        effect: "dark"
                        showArrow: false
                        reference: target
                        text: "此操作将在当前项目中生效。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"
                }
                EPopconfirm {
                    id: panel

                    title: "项目操作"
                    effect: "dark"
                    showArrow: false
                    reference: target
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EPopconfirm"

        ExampleCase {
            title: ELocale.texts.ManualPopup
            objectName: "showcaseEPopconfirm3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"

                        onClicked: panel.open()
                    }
                    EPopconfirm {
                        id: panel

                        title: "项目操作"
                        reference: target
                        trigger: "manual"
                        text: "此操作将在当前项目中生效。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"

                    onClicked: panel.open()
                }
                EPopconfirm {
                    id: panel

                    title: "项目操作"
                    reference: target
                    trigger: "manual"
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ENotification"

        ExampleCase {
            objectName: "showcaseENotification0"
            title: ELocale.texts.SemanticNotification
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "显示通知"

                            onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Success})
                        }
                        EButton {
                            text: "清空通知"

                            onClicked: messages.closeAll()
                        }
                    }
                    ENotification {
                        id: messages
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "显示通知"

                        onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Success})
                    }
                    EButton {
                        text: "清空通知"

                        onClicked: messages.closeAll()
                    }
                }
                ENotification {
                    id: messages
                }
            }
        }
    }
    ExampleSection {
        componentName: "ENotification"

        ExampleCase {
            objectName: "showcaseENotification1"
            title: ELocale.texts.PlacementAndDuration
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "显示通知"

                            onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Warning})
                        }
                        EButton {
                            text: "清空通知"

                            onClicked: messages.closeAll()
                        }
                    }
                    ENotification {
                        id: messages

                        duration: 1500
                        placement: EPlacement.BottomRight
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "显示通知"

                        onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Warning})
                    }
                    EButton {
                        text: "清空通知"

                        onClicked: messages.closeAll()
                    }
                }
                ENotification {
                    id: messages

                    duration: 1500
                    placement: EPlacement.BottomRight
                }
            }
        }
    }
    ExampleSection {
        componentName: "ENotification"

        ExampleCase {
            objectName: "showcaseENotification2"
            title: ELocale.texts.GroupedNotifications
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "显示通知"

                            onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Info})
                        }
                        EButton {
                            text: "清空通知"

                            onClicked: messages.closeAll()
                        }
                    }
                    ENotification {
                        id: messages

                        grouping: true
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "显示通知"

                        onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Info})
                    }
                    EButton {
                        text: "清空通知"

                        onClicked: messages.closeAll()
                    }
                }
                ENotification {
                    id: messages

                    grouping: true
                }
            }
        }
    }
    ExampleSection {
        componentName: "ENotification"

        ExampleCase {
            objectName: "showcaseENotification3"
            title: ELocale.texts.PersistentNotifications
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "显示通知"

                            onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Primary})
                        }
                        EButton {
                            text: "清空通知"

                            onClicked: messages.closeAll()
                        }
                    }
                    ENotification {
                        id: messages

                        duration: 0
                        showClose: true
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "显示通知"

                        onClicked: messages.open({message: "项目已经更新", title: "项目通知", styleType: EStyle.Primary})
                    }
                    EButton {
                        text: "清空通知"

                        onClicked: messages.closeAll()
                    }
                }
                ENotification {
                    id: messages

                    duration: 0
                    showClose: true
                }
            }
        }
    }
}
