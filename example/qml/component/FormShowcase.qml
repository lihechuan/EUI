import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "ESpinBox"

        ExampleCase {
            objectName: "showcaseESpinBox0"
            title: ELocale.texts.ThreeSizes2
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        numberValue: 12.5
                        size: EStyle.Small
                        width: Math.min(220, parent.width)
                    }
                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        numberValue: 12.5
                        size: EStyle.Medium
                        width: Math.min(220, parent.width)
                    }
                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        numberValue: 12.5
                        size: EStyle.Large
                        width: Math.min(220, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    numberValue: 12.5
                    size: EStyle.Small
                    width: Math.min(220, parent.width)
                }
                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    numberValue: 12.5
                    size: EStyle.Medium
                    width: Math.min(220, parent.width)
                }
                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    numberValue: 12.5
                    size: EStyle.Large
                    width: Math.min(220, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESpinBox"

        ExampleCase {
            objectName: "showcaseESpinBox1"
            title: ELocale.texts.ReadOnlyAndDisabled
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ESpinBox {
                        precision: 1
                        editable: true
                        readOnly: true
                        numberStep: 0.5
                        enabled: readOnly
                        numberValue: 12.5
                        width: Math.min(220, parent.width)
                    }
                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        readOnly: false
                        enabled: readOnly
                        numberValue: 12.5
                        width: Math.min(220, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ESpinBox {
                    precision: 1
                    editable: true
                    readOnly: true
                    numberStep: 0.5
                    enabled: readOnly
                    numberValue: 12.5
                    width: Math.min(220, parent.width)
                }
                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    readOnly: false
                    enabled: readOnly
                    numberValue: 12.5
                    width: Math.min(220, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESpinBox"

        ExampleCase {
            objectName: "showcaseESpinBox2"
            title: ELocale.texts.ValidationStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        numberValue: 12.5
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Success
                    }
                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        numberValue: 12.5
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Warning
                    }
                    ESpinBox {
                        precision: 1
                        editable: true
                        numberStep: 0.5
                        numberValue: 12.5
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Error
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    numberValue: 12.5
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Success
                }
                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    numberValue: 12.5
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Warning
                }
                ESpinBox {
                    precision: 1
                    editable: true
                    numberStep: 0.5
                    numberValue: 12.5
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Error
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESpinBox"

        ExampleCase {
            objectName: "showcaseESpinBox3"
            title: ELocale.texts.CurrencyAndRightControls
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESpinBox {
                    prefix: "$"
                    precision: 2
                    numberFrom: 0
                    numberTo: 100
                    editable: true
                    numberStep: 0.25
                    numberValue: 24.5
                    controlsPosition: "right"
                    width: Math.min(480, parent.width)
                }
            `)

            ESpinBox {
                prefix: "$"
                precision: 2
                numberFrom: 0
                numberTo: 100
                editable: true
                numberStep: 0.25
                numberValue: 24.5
                controlsPosition: "right"
                width: Math.min(480, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "EFormItem"

        ExampleCase {
            objectName: "showcaseEFormItem0"
            title: ELocale.texts.RequiredValidation
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        model: ({name: ""})
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            width: parent.width
                            labelPosition: "top"
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    model: ({name: ""})
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        width: parent.width
                        labelPosition: "top"
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EFormItem"

        ExampleCase {
            objectName: "showcaseEFormItem1"
            title: ELocale.texts.LabelAlignment
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        model: ({name: ""})
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            labelWidth: 100
                            width: parent.width
                            labelPosition: "left"
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    model: ({name: ""})
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        labelWidth: 100
                        width: parent.width
                        labelPosition: "left"
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EFormItem"

        ExampleCase {
            objectName: "showcaseEFormItem2"
            title: ELocale.texts.ValidationOnChange
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        model: ({name: ""})
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            width: parent.width
                            showSuccessState: true
                            validateOnChange: true
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    model: ({name: ""})
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        width: parent.width
                        showSuccessState: true
                        validateOnChange: true
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EFormItem"

        ExampleCase {
            objectName: "showcaseEFormItem3"
            title: ELocale.texts.DisabledForm
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        model: ({name: ""})
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            formDisabled: true
                            width: parent.width
                            helpText: "使用 3 到 20 个字符"
                            hideRequiredAsterisk: true
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    model: ({name: ""})
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        formDisabled: true
                        width: parent.width
                        helpText: "使用 3 到 20 个字符"
                        hideRequiredAsterisk: true
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadioButton"

        ExampleCase {
            objectName: "showcaseERadioButton0"
            title: ELocale.texts.SizesAndSelection
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERadioButton {
                        text: "选项"
                        checked: true
                        size: EStyle.Small
                    }
                    ERadioButton {
                        text: "选项"
                        checked: true
                        size: EStyle.Medium
                    }
                    ERadioButton {
                        text: "选项"
                        checked: true
                        size: EStyle.Large
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERadioButton {
                    text: "选项"
                    checked: true
                    size: EStyle.Small
                }
                ERadioButton {
                    text: "选项"
                    checked: true
                    size: EStyle.Medium
                }
                ERadioButton {
                    text: "选项"
                    checked: true
                    size: EStyle.Large
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadioButton"

        ExampleCase {
            objectName: "showcaseERadioButton1"
            title: ELocale.texts.SemanticColors
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERadioButton {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Primary
                    }
                    ERadioButton {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Success
                    }
                    ERadioButton {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Warning
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERadioButton {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Primary
                }
                ERadioButton {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Success
                }
                ERadioButton {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Warning
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadioButton"

        ExampleCase {
            objectName: "showcaseERadioButton2"
            title: ELocale.texts.DisabledStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERadioButton {
                        text: "不可修改"
                        checked: true
                        enabled: false
                    }
                    ERadioButton {
                        text: "不可修改"
                        checked: false
                        enabled: false
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERadioButton {
                    text: "不可修改"
                    checked: true
                    enabled: false
                }
                ERadioButton {
                    text: "不可修改"
                    checked: false
                    enabled: false
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadioButton"

        ExampleCase {
            objectName: "showcaseERadioButton3"
            title: ELocale.texts.SelectionBinding
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ERadioButton {
                        id: choice

                        text: "启用此选项"
                    }
                    EText {
                        text: choice.checked ? "已选中" : "未选中"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ERadioButton {
                    id: choice

                    text: "启用此选项"
                }
                EText {
                    text: choice.checked ? "已选中" : "未选中"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EThemeSwitch"

        ExampleCase {
            title: ELocale.texts.ThemeStatus
            objectName: "showcaseEThemeSwitch0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EThemeSwitch {
                        id: toggle

                        duration: 250
                        transitionDuration: 300
                    }
                    EText {
                        text: ETheme.dark ? "深色主题" : "浅色主题"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EThemeSwitch {
                    id: toggle

                    duration: 250
                    transitionDuration: 300
                }
                EText {
                    text: ETheme.dark ? "深色主题" : "浅色主题"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EThemeSwitch"

        ExampleCase {
            objectName: "showcaseEThemeSwitch1"
            title: ELocale.texts.CustomTrackColors
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EThemeSwitch {
                    darkIconColor: "#DCEBFF"
                    darkThumbColor: "#68B5FF"
                    darkTrackColor: "#173958"
                    lightIconColor: "#226DCA"
                    lightThumbColor: "#FFFFFF"
                    lightTrackColor: "#C3DEF4"
                }
            `)

            EThemeSwitch {
                darkIconColor: "#DCEBFF"
                darkThumbColor: "#68B5FF"
                darkTrackColor: "#173958"
                lightIconColor: "#226DCA"
                lightThumbColor: "#FFFFFF"
                lightTrackColor: "#C3DEF4"
            }
        }
    }
    ExampleSection {
        componentName: "EThemeSwitch"

        ExampleCase {
            objectName: "showcaseEThemeSwitch2"
            title: ELocale.texts.InstantTransition
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EThemeSwitch {
                    duration: 0
                    transitionDuration: 0
                }
            `)

            EThemeSwitch {
                duration: 0
                transitionDuration: 0
            }
        }
    }
    ExampleSection {
        componentName: "EThemeSwitch"

        ExampleCase {
            objectName: "showcaseEThemeSwitch3"
            title: ELocale.texts.DisabledToggle
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EThemeSwitch {
                    enabled: false
                }
            `)

            EThemeSwitch {
                enabled: false
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBoxButton"

        ExampleCase {
            objectName: "showcaseECheckBoxButton0"
            title: ELocale.texts.SizesAndSelection
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECheckBoxButton {
                        text: "选项"
                        checked: true
                        size: EStyle.Small
                    }
                    ECheckBoxButton {
                        text: "选项"
                        checked: true
                        size: EStyle.Medium
                    }
                    ECheckBoxButton {
                        text: "选项"
                        checked: true
                        size: EStyle.Large
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECheckBoxButton {
                    text: "选项"
                    checked: true
                    size: EStyle.Small
                }
                ECheckBoxButton {
                    text: "选项"
                    checked: true
                    size: EStyle.Medium
                }
                ECheckBoxButton {
                    text: "选项"
                    checked: true
                    size: EStyle.Large
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBoxButton"

        ExampleCase {
            title: ELocale.texts.SemanticColors
            objectName: "showcaseECheckBoxButton1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECheckBoxButton {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Primary
                    }
                    ECheckBoxButton {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Success
                    }
                    ECheckBoxButton {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Warning
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECheckBoxButton {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Primary
                }
                ECheckBoxButton {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Success
                }
                ECheckBoxButton {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Warning
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBoxButton"

        ExampleCase {
            title: ELocale.texts.DisabledStates
            objectName: "showcaseECheckBoxButton2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECheckBoxButton {
                        text: "不可修改"
                        checked: true
                        enabled: false
                    }
                    ECheckBoxButton {
                        text: "不可修改"
                        checked: false
                        enabled: false
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECheckBoxButton {
                    text: "不可修改"
                    checked: true
                    enabled: false
                }
                ECheckBoxButton {
                    text: "不可修改"
                    checked: false
                    enabled: false
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBoxButton"

        ExampleCase {
            title: ELocale.texts.SelectionBinding
            objectName: "showcaseECheckBoxButton3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ECheckBoxButton {
                        id: choice

                        text: "启用此选项"
                    }
                    EText {
                        text: choice.checked ? "已选中" : "未选中"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ECheckBoxButton {
                    id: choice

                    text: "启用此选项"
                }
                EText {
                    text: choice.checked ? "已选中" : "未选中"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EForm"

        ExampleCase {
            objectName: "showcaseEForm0"
            title: ELocale.texts.RequiredValidation
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        model: ({name: ""})
                        labelPosition: "top"
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            width: parent.width
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    model: ({name: ""})
                    labelPosition: "top"
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        width: parent.width
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EForm"

        ExampleCase {
            objectName: "showcaseEForm1"
            title: ELocale.texts.LabelAlignment
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        labelWidth: 100
                        model: ({name: ""})
                        labelPosition: "left"
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            width: parent.width
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    labelWidth: 100
                    model: ({name: ""})
                    labelPosition: "left"
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        width: parent.width
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EForm"

        ExampleCase {
            objectName: "showcaseEForm2"
            title: ELocale.texts.ValidationOnChange
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        model: ({name: ""})
                        validateOnChange: true
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            width: parent.width
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    model: ({name: ""})
                    validateOnChange: true
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        width: parent.width
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EForm"

        ExampleCase {
            objectName: "showcaseEForm3"
            title: ELocale.texts.DisabledForm
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EForm {
                        id: sample

                        disabled: true
                        model: ({name: ""})
                        hideRequiredAsterisk: true
                        width: Math.min(480, parent.width)

                        EFormItem {
                            prop: "name"
                            label: "项目名称"
                            required: true
                            width: parent.width
                            helpText: "使用 3 到 20 个字符"
                            rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                            EInput {
                                width: parent.width
                                placeholderText: "项目名称"
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "校验"
                            styleType: EStyle.Primary

                            onClicked: sample.validate()
                        }
                        EButton {
                            text: "重置"

                            onClicked: sample.resetFields()
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EForm {
                    id: sample

                    disabled: true
                    model: ({name: ""})
                    hideRequiredAsterisk: true
                    width: Math.min(480, parent.width)

                    EFormItem {
                        prop: "name"
                        label: "项目名称"
                        required: true
                        width: parent.width
                        helpText: "使用 3 到 20 个字符"
                        rules: [{min: 3, max: 20, message: "请输入 3 到 20 个字符"}]

                        EInput {
                            width: parent.width
                            placeholderText: "项目名称"
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "校验"
                        styleType: EStyle.Primary

                        onClicked: sample.validate()
                    }
                    EButton {
                        text: "重置"

                        onClicked: sample.resetFields()
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EInput"

        ExampleCase {
            objectName: "showcaseEInput0"
            title: ELocale.texts.ThreeSizes2
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EInput {
                        text: "Qt Quick"
                        size: EStyle.Small
                        width: Math.min(220, parent.width)
                    }
                    EInput {
                        text: "Qt Quick"
                        size: EStyle.Medium
                        width: Math.min(220, parent.width)
                    }
                    EInput {
                        text: "Qt Quick"
                        size: EStyle.Large
                        width: Math.min(220, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EInput {
                    text: "Qt Quick"
                    size: EStyle.Small
                    width: Math.min(220, parent.width)
                }
                EInput {
                    text: "Qt Quick"
                    size: EStyle.Medium
                    width: Math.min(220, parent.width)
                }
                EInput {
                    text: "Qt Quick"
                    size: EStyle.Large
                    width: Math.min(220, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EInput"

        ExampleCase {
            objectName: "showcaseEInput1"
            title: ELocale.texts.ReadOnlyAndDisabled
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EInput {
                        readOnly: true
                        text: "Qt Quick"
                        enabled: readOnly
                        width: Math.min(220, parent.width)
                    }
                    EInput {
                        readOnly: false
                        text: "Qt Quick"
                        enabled: readOnly
                        width: Math.min(220, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EInput {
                    readOnly: true
                    text: "Qt Quick"
                    enabled: readOnly
                    width: Math.min(220, parent.width)
                }
                EInput {
                    readOnly: false
                    text: "Qt Quick"
                    enabled: readOnly
                    width: Math.min(220, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EInput"

        ExampleCase {
            objectName: "showcaseEInput2"
            title: ELocale.texts.ValidationStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EInput {
                        text: "Qt Quick"
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Success
                    }
                    EInput {
                        text: "Qt Quick"
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Warning
                    }
                    EInput {
                        text: "Qt Quick"
                        validationMessage: "校验信息"
                        width: Math.min(220, parent.width)
                        validationState: EValidationState.Error
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EInput {
                    text: "Qt Quick"
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Success
                }
                EInput {
                    text: "Qt Quick"
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Warning
                }
                EInput {
                    text: "Qt Quick"
                    validationMessage: "校验信息"
                    width: Math.min(220, parent.width)
                    validationState: EValidationState.Error
                }
            }
        }
    }
    ExampleSection {
        componentName: "EInput"

        ExampleCase {
            objectName: "showcaseEInput3"
            title: ELocale.texts.PrefixSuffixAndClear
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EInput {
                    clearable: true
                    text: "workspace"
                    width: Math.min(480, parent.width)
                    suffixComponent: Component {
                        EText {
                            text: ".eui"
                        }
                    }
                    prefixComponent: Component {
                        EIcon {
                            iconSize: 16
                            iconSource: EIcons.MAGNIFIER
                        }
                    }
                }
            `)

            EInput {
                clearable: true
                text: "workspace"
                width: Math.min(480, parent.width)
                suffixComponent: Component {
                    EText {
                        text: ".eui"
                    }
                }
                prefixComponent: Component {
                    EIcon {
                        iconSize: 16
                        iconSource: EIcons.MAGNIFIER
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadio"

        ExampleCase {
            objectName: "showcaseERadio0"
            title: ELocale.texts.SizesAndSelection
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERadio {
                        text: "选项"
                        checked: true
                        size: EStyle.Small
                    }
                    ERadio {
                        text: "选项"
                        checked: true
                        size: EStyle.Medium
                    }
                    ERadio {
                        text: "选项"
                        checked: true
                        size: EStyle.Large
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERadio {
                    text: "选项"
                    checked: true
                    size: EStyle.Small
                }
                ERadio {
                    text: "选项"
                    checked: true
                    size: EStyle.Medium
                }
                ERadio {
                    text: "选项"
                    checked: true
                    size: EStyle.Large
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadio"

        ExampleCase {
            objectName: "showcaseERadio1"
            title: ELocale.texts.SemanticColors
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERadio {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Primary
                    }
                    ERadio {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Success
                    }
                    ERadio {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Warning
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERadio {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Primary
                }
                ERadio {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Success
                }
                ERadio {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Warning
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadio"

        ExampleCase {
            objectName: "showcaseERadio2"
            title: ELocale.texts.DisabledStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ERadio {
                        text: "不可修改"
                        checked: true
                        enabled: false
                    }
                    ERadio {
                        text: "不可修改"
                        checked: false
                        enabled: false
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ERadio {
                    text: "不可修改"
                    checked: true
                    enabled: false
                }
                ERadio {
                    text: "不可修改"
                    checked: false
                    enabled: false
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERadio"

        ExampleCase {
            objectName: "showcaseERadio3"
            title: ELocale.texts.SelectionBinding
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ERadio {
                        id: choice

                        text: "启用此选项"
                    }
                    EText {
                        text: choice.checked ? "已选中" : "未选中"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ERadio {
                    id: choice

                    text: "启用此选项"
                }
                EText {
                    text: choice.checked ? "已选中" : "未选中"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESlider"

        ExampleCase {
            objectName: "showcaseESlider0"
            title: ELocale.texts.StepsAndStops
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESlider {
                    from: 0
                    to: 100
                    value: 35
                    stepSize: 10
                    showStops: true
                    width: Math.min(480, parent.width)
                }
            `)

            ESlider {
                from: 0
                to: 100
                value: 35
                stepSize: 10
                showStops: true
                width: Math.min(480, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "ESlider"

        ExampleCase {
            objectName: "showcaseESlider1"
            title: ELocale.texts.MarksAndFormattedTooltip
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESlider {
                    from: 0
                    to: 100
                    value: 35
                    width: Math.min(480, parent.width)
                    marks: ({0: "最低", 25: "推荐", 75: "较高", 100: "最高"})
                    formatTooltip: function(value) { return Math.round(value) + "%" }
                }
            `)

            ESlider {
                from: 0
                to: 100
                value: 35
                width: Math.min(480, parent.width)
                marks: ({0: "最低", 25: "推荐", 75: "较高", 100: "最高"})
                formatTooltip: function(value) { return Math.round(value) + "%" }
            }
        }
    }
    ExampleSection {
        componentName: "ESlider"

        ExampleCase {
            objectName: "showcaseESlider2"
            title: ELocale.texts.DisabledRange
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESlider {
                    value: 35
                    enabled: false
                    showTooltip: false
                    width: Math.min(480, parent.width)
                }
            `)

            ESlider {
                value: 35
                enabled: false
                showTooltip: false
                width: Math.min(480, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "ESwitch"

        ExampleCase {
            objectName: "showcaseESwitch0"
            title: ELocale.texts.LabeledSizes
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        activeText: "开启"
                        inactiveText: "关闭"
                        size: EStyle.Small
                    }
                    ESwitch {
                        activeText: "开启"
                        inactiveText: "关闭"
                        size: EStyle.Medium
                    }
                    ESwitch {
                        activeText: "开启"
                        inactiveText: "关闭"
                        size: EStyle.Large
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ESwitch {
                    activeText: "开启"
                    inactiveText: "关闭"
                    size: EStyle.Small
                }
                ESwitch {
                    activeText: "开启"
                    inactiveText: "关闭"
                    size: EStyle.Medium
                }
                ESwitch {
                    activeText: "开启"
                    inactiveText: "关闭"
                    size: EStyle.Large
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESwitch"

        ExampleCase {
            objectName: "showcaseESwitch1"
            title: ELocale.texts.MappedValues
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        id: toggle

                        checked: true
                        activeValue: "enabled"
                        inactiveValue: "paused"
                    }
                    EText {
                        text: String(toggle.value)
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESwitch {
                    id: toggle

                    checked: true
                    activeValue: "enabled"
                    inactiveValue: "paused"
                }
                EText {
                    text: String(toggle.value)
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESwitch"

        ExampleCase {
            objectName: "showcaseESwitch2"
            title: ELocale.texts.LoadingAndDisabled
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        checked: true
                        loading: true
                        enabled: loading
                    }
                    ESwitch {
                        checked: true
                        loading: false
                        enabled: loading
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ESwitch {
                    checked: true
                    loading: true
                    enabled: loading
                }
                ESwitch {
                    checked: true
                    loading: false
                    enabled: loading
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBox"

        ExampleCase {
            objectName: "showcaseECheckBox0"
            title: ELocale.texts.SizesAndSelection
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECheckBox {
                        text: "选项"
                        checked: true
                        size: EStyle.Small
                    }
                    ECheckBox {
                        text: "选项"
                        checked: true
                        size: EStyle.Medium
                    }
                    ECheckBox {
                        text: "选项"
                        checked: true
                        size: EStyle.Large
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECheckBox {
                    text: "选项"
                    checked: true
                    size: EStyle.Small
                }
                ECheckBox {
                    text: "选项"
                    checked: true
                    size: EStyle.Medium
                }
                ECheckBox {
                    text: "选项"
                    checked: true
                    size: EStyle.Large
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBox"

        ExampleCase {
            objectName: "showcaseECheckBox1"
            title: ELocale.texts.SemanticColors
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECheckBox {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Primary
                    }
                    ECheckBox {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Success
                    }
                    ECheckBox {
                        text: "选项"
                        checked: true
                        styleType: EStyle.Warning
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECheckBox {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Primary
                }
                ECheckBox {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Success
                }
                ECheckBox {
                    text: "选项"
                    checked: true
                    styleType: EStyle.Warning
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBox"

        ExampleCase {
            objectName: "showcaseECheckBox2"
            title: ELocale.texts.DisabledStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ECheckBox {
                        text: "不可修改"
                        checked: true
                        enabled: false
                    }
                    ECheckBox {
                        text: "不可修改"
                        checked: false
                        enabled: false
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ECheckBox {
                    text: "不可修改"
                    checked: true
                    enabled: false
                }
                ECheckBox {
                    text: "不可修改"
                    checked: false
                    enabled: false
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECheckBox"

        ExampleCase {
            objectName: "showcaseECheckBox3"
            title: ELocale.texts.SelectionBinding
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ECheckBox {
                        id: choice

                        text: "启用此选项"
                    }
                    EText {
                        text: choice.checked ? "已选中" : "未选中"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ECheckBox {
                    id: choice

                    text: "启用此选项"
                }
                EText {
                    text: choice.checked ? "已选中" : "未选中"
                }
            }
        }
    }
}
