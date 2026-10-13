import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EMessageBox"

        ExampleCase {
            title: ELocale.texts.AlertMessage
            objectName: "showcaseEMessageBox0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开消息框"

                        onClicked: panel.alert("项目已经保存", "完成")
                    }
                    EMessageBox {
                        id: panel
                    }
                    EText {
                        id: result

                        text: "等待操作"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开消息框"

                    onClicked: panel.alert("项目已经保存", "完成")
                }
                EMessageBox {
                    id: panel
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessageBox"

        ExampleCase {
            objectName: "showcaseEMessageBox1"
            title: ELocale.texts.ConfirmationResult
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开消息框"

                        onClicked: panel.confirm("继续当前操作？", "确认")
                    }
                    EMessageBox {
                        id: panel

                        onCompleted: result.text = action
                    }
                    EText {
                        id: result

                        text: "等待操作"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开消息框"

                    onClicked: panel.confirm("继续当前操作？", "确认")
                }
                EMessageBox {
                    id: panel

                    onCompleted: result.text = action
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessageBox"

        ExampleCase {
            objectName: "showcaseEMessageBox2"
            title: ELocale.texts.PromptValidation
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开消息框"

                        onClicked: panel.prompt("输入项目名称", "新建项目", "EUI")
                    }
                    EMessageBox {
                        id: panel

                        inputPattern: /^.{3,20}$/
                        inputErrorMessage: "请输入 3 到 20 个字符"

                        onConfirmed: result.text = value
                    }
                    EText {
                        id: result

                        text: "等待操作"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开消息框"

                    onClicked: panel.prompt("输入项目名称", "新建项目", "EUI")
                }
                EMessageBox {
                    id: panel

                    inputPattern: /^.{3,20}$/
                    inputErrorMessage: "请输入 3 到 20 个字符"

                    onConfirmed: result.text = value
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMessageBox"

        ExampleCase {
            objectName: "showcaseEMessageBox3"
            title: ELocale.texts.StyledConfirmation
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "打开消息框"

                        onClicked: panel.confirm("准备发布项目？", "发布")
                    }
                    EMessageBox {
                        id: panel

                        roundButton: true
                        styleType: EStyle.Success
                        confirmStyleType: EStyle.Success
                    }
                    EText {
                        id: result

                        text: "等待操作"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "打开消息框"

                    onClicked: panel.confirm("准备发布项目？", "发布")
                }
                EMessageBox {
                    id: panel

                    roundButton: true
                    styleType: EStyle.Success
                    confirmStyleType: EStyle.Success
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
}
