import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EIcon"

        ExampleCase {
            objectName: "showcaseEIcon0"
            title: ELocale.texts.AccentColors
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EIcon {
                        iconSize: 32
                        color: "#409EFF"
                        iconSource: EIcons.PALETTE
                    }
                    EIcon {
                        iconSize: 32
                        color: "#21B6A8"
                        iconSource: EIcons.PALETTE
                    }
                    EIcon {
                        iconSize: 32
                        color: "#A78BFA"
                        iconSource: EIcons.PALETTE
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EIcon {
                    iconSize: 32
                    color: "#409EFF"
                    iconSource: EIcons.PALETTE
                }
                EIcon {
                    iconSize: 32
                    color: "#21B6A8"
                    iconSource: EIcons.PALETTE
                }
                EIcon {
                    iconSize: 32
                    color: "#A78BFA"
                    iconSource: EIcons.PALETTE
                }
            }
        }
    }
    ExampleSection {
        componentName: "EIcon"

        ExampleCase {
            objectName: "showcaseEIcon1"
            title: ELocale.texts.LabeledIconAction
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EButton {
                    text: "编辑项目"
                    leftIcon: true
                    iconSource: EIcons.PENCIL
                    styleType: EStyle.Primary
                }
            `)

            EButton {
                text: "编辑项目"
                leftIcon: true
                iconSource: EIcons.PENCIL
                styleType: EStyle.Primary
            }
        }
    }
    ExampleSection {
        componentName: "EIcon"

        ExampleCase {
            objectName: "showcaseEIcon2"
            title: ELocale.texts.IconScale
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EIcon {
                        iconSize: 16
                        iconSource: EIcons.PALETTE
                    }
                    EIcon {
                        iconSize: 24
                        iconSource: EIcons.PALETTE
                    }
                    EIcon {
                        iconSize: 32
                        iconSource: EIcons.PALETTE
                    }
                    EIcon {
                        iconSize: 48
                        iconSource: EIcons.PALETTE
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EIcon {
                    iconSize: 16
                    iconSource: EIcons.PALETTE
                }
                EIcon {
                    iconSize: 24
                    iconSource: EIcons.PALETTE
                }
                EIcon {
                    iconSize: 32
                    iconSource: EIcons.PALETTE
                }
                EIcon {
                    iconSize: 48
                    iconSource: EIcons.PALETTE
                }
            }
        }
    }
    ExampleSection {
        componentName: "EText"

        ExampleCase {
            objectName: "showcaseEText0"
            title: ELocale.texts.TruncationAndLineLimits
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EText {
                        truncated: true
                        width: Math.min(480, parent.width)
                        text: "一段超过容器宽度的长文本，用于验证单行省略与完整内容的可读性。"
                    }
                    EText {
                        lineClamp: 2
                        wrapMode: Text.WordWrap
                        width: Math.min(480, parent.width)
                        text: "多行文字可以限制展示行数。较长的说明会自动换行，并在达到指定行数后省略其余内容。调整窗口宽度可以查看排版变化。"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EText {
                    truncated: true
                    width: Math.min(480, parent.width)
                    text: "一段超过容器宽度的长文本，用于验证单行省略与完整内容的可读性。"
                }
                EText {
                    lineClamp: 2
                    wrapMode: Text.WordWrap
                    width: Math.min(480, parent.width)
                    text: "多行文字可以限制展示行数。较长的说明会自动换行，并在达到指定行数后省略其余内容。调整窗口宽度可以查看排版变化。"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EText"

        ExampleCase {
            objectName: "showcaseEText1"
            title: ELocale.texts.TextSizes
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EText {
                        size: EStyle.Small
                        text: "Qt Quick · 清晰阅读"
                    }
                    EText {
                        size: EStyle.Medium
                        text: "Qt Quick · 清晰阅读"
                    }
                    EText {
                        size: EStyle.Large
                        text: "Qt Quick · 清晰阅读"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EText {
                    size: EStyle.Small
                    text: "Qt Quick · 清晰阅读"
                }
                EText {
                    size: EStyle.Medium
                    text: "Qt Quick · 清晰阅读"
                }
                EText {
                    size: EStyle.Large
                    text: "Qt Quick · 清晰阅读"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EText"

        ExampleCase {
            objectName: "showcaseEText2"
            title: ELocale.texts.SemanticText
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EText {
                        text: "状态信息"
                        type: "primary"
                    }
                    EText {
                        text: "状态信息"
                        type: "success"
                    }
                    EText {
                        text: "状态信息"
                        type: "warning"
                    }
                    EText {
                        text: "状态信息"
                        type: "danger"
                    }
                    EText {
                        text: "状态信息"
                        type: "info"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EText {
                    text: "状态信息"
                    type: "primary"
                }
                EText {
                    text: "状态信息"
                    type: "success"
                }
                EText {
                    text: "状态信息"
                    type: "warning"
                }
                EText {
                    text: "状态信息"
                    type: "danger"
                }
                EText {
                    text: "状态信息"
                    type: "info"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EText"

        ExampleCase {
            objectName: "showcaseEText3"
            title: ELocale.texts.RichText
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EText {
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    width: Math.min(480, parent.width)
                    text: "<b>标题</b> · <i>说明</i><br/>Qt Quick &amp; QML"
                }
            `)

            EText {
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                width: Math.min(480, parent.width)
                text: "<b>标题</b> · <i>说明</i><br/>Qt Quick &amp; QML"
            }
        }
    }
    ExampleSection {
        componentName: "EButton"

        ExampleCase {
            objectName: "showcaseEButton0"
            title: ELocale.texts.SemanticActions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        plain: true
                        text: "Action"
                        styleType: EStyle.Primary
                    }
                    EButton {
                        plain: true
                        text: "Action"
                        styleType: EStyle.Success
                    }
                    EButton {
                        plain: true
                        text: "Action"
                        styleType: EStyle.Warning
                    }
                    EButton {
                        plain: true
                        text: "Action"
                        styleType: EStyle.Danger
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EButton {
                    plain: true
                    text: "Action"
                    styleType: EStyle.Primary
                }
                EButton {
                    plain: true
                    text: "Action"
                    styleType: EStyle.Success
                }
                EButton {
                    plain: true
                    text: "Action"
                    styleType: EStyle.Warning
                }
                EButton {
                    plain: true
                    text: "Action"
                    styleType: EStyle.Danger
                }
            }
        }
    }
    ExampleSection {
        componentName: "EButton"

        ExampleCase {
            objectName: "showcaseEButton1"
            title: ELocale.texts.ActionLimitsAndReset
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: sample

                        property int count: 0

                        enabled: count < 3
                        styleType: EStyle.Primary
                        text: "提交 (" + count + "/3)"

                        onClicked: ++count
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.count = 0
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: sample

                    property int count: 0

                    enabled: count < 3
                    styleType: EStyle.Primary
                    text: "提交 (" + count + "/3)"

                    onClicked: ++count
                }
                EButton {
                    text: "重置"

                    onClicked: sample.count = 0
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERectangle"

        ExampleCase {
            objectName: "showcaseERectangle0"
            title: ELocale.texts.CornerShapes
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERectangle {
                        radius: 0
                        height: 72
                        width: 100
                        color: "#409EFF"
                    }
                    ERectangle {
                        height: 72
                        radius: 12
                        width: 100
                        color: "#409EFF"
                    }
                    ERectangle {
                        height: 72
                        radius: 36
                        width: 100
                        color: "#409EFF"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERectangle {
                    radius: 0
                    height: 72
                    width: 100
                    color: "#409EFF"
                }
                ERectangle {
                    height: 72
                    radius: 12
                    width: 100
                    color: "#409EFF"
                }
                ERectangle {
                    height: 72
                    radius: 36
                    width: 100
                    color: "#409EFF"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERectangle"

        ExampleCase {
            objectName: "showcaseERectangle1"
            title: ELocale.texts.OutlineWeights
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERectangle {
                        height: 72
                        radius: 12
                        width: 100
                        border.width: 1
                        color: "transparent"
                        border.color: "#21B6A8"
                    }
                    ERectangle {
                        height: 72
                        radius: 12
                        width: 100
                        border.width: 2
                        color: "transparent"
                        border.color: "#21B6A8"
                    }
                    ERectangle {
                        height: 72
                        radius: 12
                        width: 100
                        border.width: 4
                        color: "transparent"
                        border.color: "#21B6A8"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERectangle {
                    height: 72
                    radius: 12
                    width: 100
                    border.width: 1
                    color: "transparent"
                    border.color: "#21B6A8"
                }
                ERectangle {
                    height: 72
                    radius: 12
                    width: 100
                    border.width: 2
                    color: "transparent"
                    border.color: "#21B6A8"
                }
                ERectangle {
                    height: 72
                    radius: 12
                    width: 100
                    border.width: 4
                    color: "transparent"
                    border.color: "#21B6A8"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERectangle"

        ExampleCase {
            objectName: "showcaseERectangle2"
            title: ELocale.texts.GradientSurface
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERectangle {
                    radius: 16
                    height: 100
                    width: Math.min(420, parent.width)
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#226DCA"
                        }
                        GradientStop {
                            position: 1
                            color: "#21B6A8"
                        }
                    }
                }
            `)

            ERectangle {
                radius: 16
                height: 100
                width: Math.min(420, parent.width)
                gradient: Gradient {
                    GradientStop {
                        position: 0
                        color: "#226DCA"
                    }
                    GradientStop {
                        position: 1
                        color: "#21B6A8"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERectangle"

        ExampleCase {
            objectName: "showcaseERectangle3"
            title: ELocale.texts.ContentSurface
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERectangle {
                    height: 96
                    radius: 16
                    width: 240
                    color: "#173958"

                    EText {
                        color: "#E7F0FF"
                        text: "EUI / WORKSPACE"
                        anchors.centerIn: parent
                    }
                }
            `)

            ERectangle {
                height: 96
                radius: 16
                width: 240
                color: "#173958"

                EText {
                    color: "#E7F0FF"
                    text: "EUI / WORKSPACE"
                    anchors.centerIn: parent
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextArea"

        ExampleCase {
            objectName: "showcaseETextArea0"
            title: ELocale.texts.ThreeSizes2
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextArea {
                        rows: 3
                        text: "第一行\\n第二行"
                        size: EStyle.Small
                        width: Math.min(220, parent.width)
                    }
                    ETextArea {
                        rows: 3
                        text: "第一行\\n第二行"
                        size: EStyle.Medium
                        width: Math.min(220, parent.width)
                    }
                    ETextArea {
                        rows: 3
                        text: "第一行\\n第二行"
                        size: EStyle.Large
                        width: Math.min(220, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextArea {
                    rows: 3
                    text: "第一行\n第二行"
                    size: EStyle.Small
                    width: Math.min(220, parent.width)
                }
                ETextArea {
                    rows: 3
                    text: "第一行\n第二行"
                    size: EStyle.Medium
                    width: Math.min(220, parent.width)
                }
                ETextArea {
                    rows: 3
                    text: "第一行\n第二行"
                    size: EStyle.Large
                    width: Math.min(220, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextArea"

        ExampleCase {
            objectName: "showcaseETextArea1"
            title: ELocale.texts.ReadOnlyAndDisabled
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextArea {
                        rows: 3
                        readOnly: true
                        text: "第一行\\n第二行"
                        enabled: readOnly
                        width: Math.min(220, parent.width)
                    }
                    ETextArea {
                        rows: 3
                        readOnly: false
                        text: "第一行\\n第二行"
                        enabled: readOnly
                        width: Math.min(220, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextArea {
                    rows: 3
                    readOnly: true
                    text: "第一行\n第二行"
                    enabled: readOnly
                    width: Math.min(220, parent.width)
                }
                ETextArea {
                    rows: 3
                    readOnly: false
                    text: "第一行\n第二行"
                    enabled: readOnly
                    width: Math.min(220, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextArea"

        ExampleCase {
            objectName: "showcaseETextArea2"
            title: ELocale.texts.ValidationStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextArea {
                        rows: 3
                        text: "第一行\\n第二行"
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Success
                    }
                    ETextArea {
                        rows: 3
                        text: "第一行\\n第二行"
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Warning
                    }
                    ETextArea {
                        rows: 3
                        text: "第一行\\n第二行"
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Error
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextArea {
                    rows: 3
                    text: "第一行\n第二行"
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Success
                }
                ETextArea {
                    rows: 3
                    text: "第一行\n第二行"
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Warning
                }
                ETextArea {
                    rows: 3
                    text: "第一行\n第二行"
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Error
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextArea"

        ExampleCase {
            objectName: "showcaseETextArea3"
            title: ELocale.texts.WordCountAndAutosize
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ETextArea {
                    maximumLength: 80
                    showWordLimit: true
                    placeholderText: "最多 80 个字符"
                    width: Math.min(480, parent.width)
                    autosize: ({minRows: 2, maxRows: 5})
                }
            `)

            ETextArea {
                maximumLength: 80
                showWordLimit: true
                placeholderText: "最多 80 个字符"
                width: Math.min(480, parent.width)
                autosize: ({minRows: 2, maxRows: 5})
            }
        }
    }
    ExampleSection {
        componentName: "ETextButton"

        ExampleCase {
            title: ELocale.texts.Sizes
            objectName: "showcaseETextButton0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextButton {
                        text: "查看详情"
                        size: EStyle.Small
                        styleType: EStyle.Primary
                    }
                    ETextButton {
                        text: "查看详情"
                        size: EStyle.Medium
                        styleType: EStyle.Primary
                    }
                    ETextButton {
                        text: "查看详情"
                        size: EStyle.Large
                        styleType: EStyle.Primary
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextButton {
                    text: "查看详情"
                    size: EStyle.Small
                    styleType: EStyle.Primary
                }
                ETextButton {
                    text: "查看详情"
                    size: EStyle.Medium
                    styleType: EStyle.Primary
                }
                ETextButton {
                    text: "查看详情"
                    size: EStyle.Large
                    styleType: EStyle.Primary
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextButton"

        ExampleCase {
            title: ELocale.texts.IconActions
            objectName: "showcaseETextButton1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextButton {
                        text: "操作"
                        leftIcon: true
                        iconSource: EIcons.PENCIL
                    }
                    ETextButton {
                        text: "操作"
                        leftIcon: true
                        iconSource: EIcons.CHECK
                    }
                    ETextButton {
                        text: "操作"
                        leftIcon: true
                        iconSource: EIcons.DELETE
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextButton {
                    text: "操作"
                    leftIcon: true
                    iconSource: EIcons.PENCIL
                }
                ETextButton {
                    text: "操作"
                    leftIcon: true
                    iconSource: EIcons.CHECK
                }
                ETextButton {
                    text: "操作"
                    leftIcon: true
                    iconSource: EIcons.DELETE
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextButton"

        ExampleCase {
            objectName: "showcaseETextButton2"
            title: ELocale.texts.DisabledAndHighlighted
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextButton {
                        text: "项目操作"
                        enabled: true
                        styleType: EStyle.Primary
                        isOpenBackgroundColor: true
                    }
                    ETextButton {
                        text: "项目操作"
                        enabled: false
                        styleType: EStyle.Primary
                        isOpenBackgroundColor: true
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextButton {
                    text: "项目操作"
                    enabled: true
                    styleType: EStyle.Primary
                    isOpenBackgroundColor: true
                }
                ETextButton {
                    text: "项目操作"
                    enabled: false
                    styleType: EStyle.Primary
                    isOpenBackgroundColor: true
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETextButton"

        ExampleCase {
            objectName: "showcaseETextButton3"
            title: ELocale.texts.RoundedActions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETextButton {
                        text: "状态"
                        round: true
                        styleType: EStyle.Success
                        isOpenBackgroundColor: true
                    }
                    ETextButton {
                        text: "状态"
                        round: true
                        styleType: EStyle.Warning
                        isOpenBackgroundColor: true
                    }
                    ETextButton {
                        text: "状态"
                        round: true
                        styleType: EStyle.Danger
                        isOpenBackgroundColor: true
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETextButton {
                    text: "状态"
                    round: true
                    styleType: EStyle.Success
                    isOpenBackgroundColor: true
                }
                ETextButton {
                    text: "状态"
                    round: true
                    styleType: EStyle.Warning
                    isOpenBackgroundColor: true
                }
                ETextButton {
                    text: "状态"
                    round: true
                    styleType: EStyle.Danger
                    isOpenBackgroundColor: true
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERotatingIcon"

        ExampleCase {
            title: ELocale.texts.RotationSpeeds
            objectName: "showcaseERotatingIcon0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERotatingIcon {
                        iconSize: 28
                        duration: 600
                        running: true
                        iconSource: EIcons.ARROW_ROTATE_RIGHT
                    }
                    ERotatingIcon {
                        iconSize: 28
                        running: true
                        duration: 1400
                        iconSource: EIcons.ARROW_ROTATE_RIGHT
                    }
                    ERotatingIcon {
                        iconSize: 28
                        running: true
                        duration: 2800
                        iconSource: EIcons.ARROW_ROTATE_RIGHT
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERotatingIcon {
                    iconSize: 28
                    duration: 600
                    running: true
                    iconSource: EIcons.ARROW_ROTATE_RIGHT
                }
                ERotatingIcon {
                    iconSize: 28
                    running: true
                    duration: 1400
                    iconSource: EIcons.ARROW_ROTATE_RIGHT
                }
                ERotatingIcon {
                    iconSize: 28
                    running: true
                    duration: 2800
                    iconSource: EIcons.ARROW_ROTATE_RIGHT
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERotatingIcon"

        ExampleCase {
            title: ELocale.texts.PauseAndResume
            objectName: "showcaseERotatingIcon1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        id: active

                        text: "运行动画"
                        checked: true
                    }
                    ERotatingIcon {
                        iconSize: 32
                        running: active.checked
                        iconSource: EIcons.ARROW_ROTATE_RIGHT
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESwitch {
                    id: active

                    text: "运行动画"
                    checked: true
                }
                ERotatingIcon {
                    iconSize: 32
                    running: active.checked
                    iconSource: EIcons.ARROW_ROTATE_RIGHT
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERotatingIcon"

        ExampleCase {
            title: ELocale.texts.RotationOrigin
            objectName: "showcaseERotatingIcon2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERotatingIcon {
                    width: 80
                    height: 80
                    originX: 0
                    iconSize: 24
                    originY: height / 2
                    iconSource: EIcons.PALETTE
                }
            `)

            ERotatingIcon {
                width: 80
                height: 80
                originX: 0
                iconSize: 24
                originY: height / 2
                iconSource: EIcons.PALETTE
            }
        }
    }
    ExampleSection {
        componentName: "ERotatingIcon"

        ExampleCase {
            objectName: "showcaseERotatingIcon3"
            title: ELocale.texts.StaticStatusIcon
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERotatingIcon {
                    iconSize: 36
                    running: false
                    color: "#21B6A8"
                    iconSource: EIcons.CHECK
                }
            `)

            ERotatingIcon {
                iconSize: 36
                running: false
                color: "#21B6A8"
                iconSource: EIcons.CHECK
            }
        }
    }
    ExampleSection {
        componentName: "ECopyableText"

        ExampleCase {
            objectName: "showcaseECopyableText0"
            title: ELocale.texts.SelectableParagraphs
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECopyableText {
                    wrapMode: Text.WordWrap
                    width: Math.min(480, parent.width)
                    text: "选中这段长文本并使用 Ctrl+C 或 Command+C 复制。窗口变窄时，内容会自动换行。"
                }
            `)

            ECopyableText {
                wrapMode: Text.WordWrap
                width: Math.min(480, parent.width)
                text: "选中这段长文本并使用 Ctrl+C 或 Command+C 复制。窗口变窄时，内容会自动换行。"
            }
        }
    }
    ExampleSection {
        componentName: "ECopyableText"

        ExampleCase {
            title: ELocale.texts.CodeSelection
            objectName: "showcaseECopyableText1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECopyableText {
                    width: Math.min(480, parent.width)
                    text: "import EUI 1.0\\nEButton { text: \\"Hello\\" }"
                    font.family: Qt.platform.os === "windows" ? "Consolas" : "Menlo"
                }
            `)

            ECopyableText {
                width: Math.min(480, parent.width)
                text: "import EUI 1.0\nEButton { text: \"Hello\" }"
                font.family: Qt.platform.os === "windows" ? "Consolas" : "Menlo"
            }
        }
    }
    ExampleSection {
        componentName: "ECopyableText"

        ExampleCase {
            objectName: "showcaseECopyableText2"
            title: ELocale.texts.CopyableIdentifier
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECopyableText {
                    font.pixelSize: 18
                    textColor: "#409EFF"
                    text: "PROJECT-2026-001"
                }
            `)

            ECopyableText {
                font.pixelSize: 18
                textColor: "#409EFF"
                text: "PROJECT-2026-001"
            }
        }
    }
    ExampleSection {
        componentName: "ECopyableText"

        ExampleCase {
            objectName: "showcaseECopyableText3"
            title: ELocale.texts.ContactSelection
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: "拖动选择文字，保留标准快捷键复制。"
                    }
                    ECopyableText {
                        font.underline: true
                        text: "support@example.com"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EText {
                    width: parent.width
                    wrapMode: Text.WordWrap
                    text: "拖动选择文字，保留标准快捷键复制。"
                }
                ECopyableText {
                    font.underline: true
                    text: "support@example.com"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAdvancedRectangle"

        ExampleCase {
            title: ELocale.texts.IndependentCorners
            objectName: "showcaseEAdvancedRectangle0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EAdvancedRectangle {
                        height: 72
                        width: 100
                        color: "#409EFF"
                        radius: [0, 24, 0, 24]
                    }
                    EAdvancedRectangle {
                        height: 72
                        width: 100
                        color: "#409EFF"
                        radius: [24, 0, 24, 0]
                    }
                    EAdvancedRectangle {
                        height: 72
                        width: 100
                        color: "#409EFF"
                        radius: [24, 24, 4, 4]
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EAdvancedRectangle {
                    height: 72
                    width: 100
                    color: "#409EFF"
                    radius: [0, 24, 0, 24]
                }
                EAdvancedRectangle {
                    height: 72
                    width: 100
                    color: "#409EFF"
                    radius: [24, 0, 24, 0]
                }
                EAdvancedRectangle {
                    height: 72
                    width: 100
                    color: "#409EFF"
                    radius: [24, 24, 4, 4]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAdvancedRectangle"

        ExampleCase {
            title: ELocale.texts.BorderPatterns
            objectName: "showcaseEAdvancedRectangle1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EAdvancedRectangle {
                        height: 72
                        width: 100
                        borderWidth: 2
                        color: "transparent"
                        borderColor: "#21B6A8"
                        radius: [12, 12, 12, 12]
                        borderStyle: Qt.SolidLine
                    }
                    EAdvancedRectangle {
                        height: 72
                        width: 100
                        borderWidth: 2
                        color: "transparent"
                        borderColor: "#21B6A8"
                        borderStyle: Qt.DashLine
                        radius: [12, 12, 12, 12]
                    }
                    EAdvancedRectangle {
                        height: 72
                        width: 100
                        borderWidth: 2
                        color: "transparent"
                        borderColor: "#21B6A8"
                        borderStyle: Qt.DotLine
                        radius: [12, 12, 12, 12]
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EAdvancedRectangle {
                    height: 72
                    width: 100
                    borderWidth: 2
                    color: "transparent"
                    borderColor: "#21B6A8"
                    radius: [12, 12, 12, 12]
                    borderStyle: Qt.SolidLine
                }
                EAdvancedRectangle {
                    height: 72
                    width: 100
                    borderWidth: 2
                    color: "transparent"
                    borderColor: "#21B6A8"
                    borderStyle: Qt.DashLine
                    radius: [12, 12, 12, 12]
                }
                EAdvancedRectangle {
                    height: 72
                    width: 100
                    borderWidth: 2
                    color: "transparent"
                    borderColor: "#21B6A8"
                    borderStyle: Qt.DotLine
                    radius: [12, 12, 12, 12]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAdvancedRectangle"

        ExampleCase {
            title: ELocale.texts.CustomDashPattern
            objectName: "showcaseEAdvancedRectangle2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAdvancedRectangle {
                    height: 80
                    width: 280
                    borderWidth: 3
                    color: "transparent"
                    borderColor: "#A78BFA"
                    radius: [16, 16, 16, 16]
                    dashPattern: [6, 2, 1, 2]
                    borderStyle: Qt.CustomDashLine
                }
            `)

            EAdvancedRectangle {
                height: 80
                width: 280
                borderWidth: 3
                color: "transparent"
                borderColor: "#A78BFA"
                radius: [16, 16, 16, 16]
                dashPattern: [6, 2, 1, 2]
                borderStyle: Qt.CustomDashLine
            }
        }
    }
    ExampleSection {
        componentName: "EAdvancedRectangle"

        ExampleCase {
            title: ELocale.texts.AsymmetricSurface
            objectName: "showcaseEAdvancedRectangle3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAdvancedRectangle {
                    height: 96
                    width: 240
                    color: "#173958"
                    radius: [28, 6, 28, 6]

                    EText {
                        color: "#E7F0FF"
                        text: "EUI / PANEL"
                        anchors.centerIn: parent
                    }
                }
            `)

            EAdvancedRectangle {
                height: 96
                width: 240
                color: "#173958"
                radius: [28, 6, 28, 6]

                EText {
                    color: "#E7F0FF"
                    text: "EUI / PANEL"
                    anchors.centerIn: parent
                }
            }
        }
    }
}
