import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "ELoading"

        ExampleCase {
            objectName: "showcaseELoading0"
            title: ELocale.texts.SpinnerStrokes
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ELoading {
                        width: 40
                        height: 40
                        strokeWidth: 2
                        sweepAngle: 100
                        color: "#21B6A8"
                    }
                    ELoading {
                        width: 40
                        height: 40
                        strokeWidth: 4
                        sweepAngle: 100
                        color: "#21B6A8"
                    }
                    ELoading {
                        width: 40
                        height: 40
                        strokeWidth: 6
                        sweepAngle: 100
                        color: "#21B6A8"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ELoading {
                    width: 40
                    height: 40
                    strokeWidth: 2
                    sweepAngle: 100
                    color: "#21B6A8"
                }
                ELoading {
                    width: 40
                    height: 40
                    strokeWidth: 4
                    sweepAngle: 100
                    color: "#21B6A8"
                }
                ELoading {
                    width: 40
                    height: 40
                    strokeWidth: 6
                    sweepAngle: 100
                    color: "#21B6A8"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ELoading"

        ExampleCase {
            objectName: "showcaseELoading1"
            title: ELocale.texts.TargetLoadingOverlay
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        id: active

                        text: "加载中"
                    }
                    ERectangle {
                        id: target

                        radius: 12
                        height: 140
                        color: "#173958"
                        width: Math.min(480, parent.width)

                        EText {
                            text: "项目内容"
                            color: "#E7F0FF"
                            anchors.centerIn: parent
                        }
                    }
                    ELoading {
                        text: "正在处理…"
                        material: true
                        target: target
                        loading: active.checked
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESwitch {
                    id: active

                    text: "加载中"
                }
                ERectangle {
                    id: target

                    radius: 12
                    height: 140
                    color: "#173958"
                    width: Math.min(480, parent.width)

                    EText {
                        text: "项目内容"
                        color: "#E7F0FF"
                        anchors.centerIn: parent
                    }
                }
                ELoading {
                    text: "正在处理…"
                    material: true
                    target: target
                    loading: active.checked
                }
            }
        }
    }
    ExampleSection {
        componentName: "ELoading"

        ExampleCase {
            objectName: "showcaseELoading2"
            title: ELocale.texts.MaterialSpinner
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Item {
                    height: 120
                    width: Math.min(480, parent.width)

                    ELoading {
                        width: 40
                        height: 40
                        text: "正在加载"
                        material: true
                        anchors.centerIn: parent
                        styleType: EStyle.Success
                    }
                }
            `)

            Item {
                height: 120
                width: Math.min(480, parent.width)

                ELoading {
                    width: 40
                    height: 40
                    text: "正在加载"
                    material: true
                    anchors.centerIn: parent
                    styleType: EStyle.Success
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessage"

        ExampleCase {
            objectName: "showcaseEMessage0"
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
                    EMessage {
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
                EMessage {
                    id: messages
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessage"

        ExampleCase {
            objectName: "showcaseEMessage1"
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
                    EMessage {
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
                EMessage {
                    id: messages

                    duration: 1500
                    placement: EPlacement.BottomRight
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessage"

        ExampleCase {
            objectName: "showcaseEMessage2"
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
                    EMessage {
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
                EMessage {
                    id: messages

                    grouping: true
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessage"

        ExampleCase {
            objectName: "showcaseEMessage3"
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
                    EMessage {
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
                EMessage {
                    id: messages

                    duration: 0
                    showClose: true
                }
            }
        }
    }
    ExampleSection {
        componentName: "EToolTip"

        ExampleCase {
            objectName: "showcaseEToolTip0"
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
                    EToolTip {
                        id: panel

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
                EToolTip {
                    id: panel

                    reference: target
                    text: "此操作将在当前项目中生效。"
                    placement: EPlacement.Right
                }
            }
        }
    }
    ExampleSection {
        componentName: "EToolTip"

        ExampleCase {
            objectName: "showcaseEToolTip1"
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
                    EToolTip {
                        id: panel

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
                EToolTip {
                    id: panel

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
        componentName: "EToolTip"

        ExampleCase {
            objectName: "showcaseEToolTip2"
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
                    EToolTip {
                        id: panel

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
                EToolTip {
                    id: panel

                    effect: "dark"
                    showArrow: false
                    reference: target
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EToolTip"

        ExampleCase {
            objectName: "showcaseEToolTip3"
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
                    EToolTip {
                        id: panel

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
                EToolTip {
                    id: panel

                    reference: target
                    trigger: "manual"
                    text: "此操作将在当前项目中生效。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECircleProgress"

        ExampleCase {
            title: ELocale.texts.ProgressStatus
            objectName: "showcaseECircleProgress0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECircleProgress {
                        value: 65
                        width: 100
                        height: 100
                        status: "success"
                    }
                    ECircleProgress {
                        value: 65
                        width: 100
                        height: 100
                        status: "warning"
                    }
                    ECircleProgress {
                        value: 65
                        width: 100
                        height: 100
                        status: "exception"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECircleProgress {
                    value: 65
                    width: 100
                    height: 100
                    status: "success"
                }
                ECircleProgress {
                    value: 65
                    width: 100
                    height: 100
                    status: "warning"
                }
                ECircleProgress {
                    value: 65
                    width: 100
                    height: 100
                    status: "exception"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECircleProgress"

        ExampleCase {
            objectName: "showcaseECircleProgress1"
            title: ELocale.texts.ProgressFormatting
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECircleProgress {
                    value: 65
                    width: 100
                    height: 100
                    strokeWidth: 8
                    color: "#21B6A8"
                    format: function(value) { return Math.round(value) + " / 100" }
                }
            `)

            ECircleProgress {
                value: 65
                width: 100
                height: 100
                strokeWidth: 8
                color: "#21B6A8"
                format: function(value) { return Math.round(value) + " / 100" }
            }
        }
    }
    ExampleSection {
        componentName: "ECircleProgress"

        ExampleCase {
            objectName: "showcaseECircleProgress2"
            title: ELocale.texts.IndeterminateProgress
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECircleProgress {
                    value: 65
                    width: 100
                    height: 100
                    showText: false
                    indeterminate: true
                }
            `)

            ECircleProgress {
                value: 65
                width: 100
                height: 100
                showText: false
                indeterminate: true
            }
        }
    }
    ExampleSection {
        componentName: "EProgress"

        ExampleCase {
            objectName: "showcaseEProgress0"
            title: ELocale.texts.ProgressStatus
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EProgress {
                        value: 65
                        status: "success"
                        width: Math.min(260, parent.width)
                    }
                    EProgress {
                        value: 65
                        status: "warning"
                        width: Math.min(260, parent.width)
                    }
                    EProgress {
                        value: 65
                        status: "exception"
                        width: Math.min(260, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EProgress {
                    value: 65
                    status: "success"
                    width: Math.min(260, parent.width)
                }
                EProgress {
                    value: 65
                    status: "warning"
                    width: Math.min(260, parent.width)
                }
                EProgress {
                    value: 65
                    status: "exception"
                    width: Math.min(260, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EProgress"

        ExampleCase {
            objectName: "showcaseEProgress1"
            title: ELocale.texts.ProgressFormatting
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EProgress {
                    value: 65
                    strokeWidth: 8
                    color: "#21B6A8"
                    width: Math.min(260, parent.width)
                    format: function(value) { return Math.round(value) + " / 100" }
                }
            `)

            EProgress {
                value: 65
                strokeWidth: 8
                color: "#21B6A8"
                width: Math.min(260, parent.width)
                format: function(value) { return Math.round(value) + " / 100" }
            }
        }
    }
    ExampleSection {
        componentName: "EProgress"

        ExampleCase {
            objectName: "showcaseEProgress2"
            title: ELocale.texts.IndeterminateProgress
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EProgress {
                    value: 65
                    showText: false
                    indeterminate: true
                    width: Math.min(260, parent.width)
                }
            `)

            EProgress {
                value: 65
                showText: false
                indeterminate: true
                width: Math.min(260, parent.width)
            }
        }
    }
}
