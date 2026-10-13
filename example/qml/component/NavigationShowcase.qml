import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "ETabs"

        ExampleCase {
            objectName: "showcaseETabs0"
            title: ELocale.texts.CardTabs
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ETabs {
                        type: "card"
                        stretch: true
                        tabPosition: "bottom"
                        width: Math.min(480, parent.width)
                        model: [{name: "overview", label: "概览", contentComponent: pane}, {name: "activity", label: "活动", contentComponent: pane}, {name: "settings", label: "设置", contentComponent: pane}, {name: "locked", label: "归档", disabled: true}]
                    }
                    Component {
                        id: pane

                        EText {
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: parent.entry.label + "：独立的标签内容"
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ETabs {
                    type: "card"
                    stretch: true
                    tabPosition: "bottom"
                    width: Math.min(480, parent.width)
                    model: [{name: "overview", label: "概览", contentComponent: pane}, {name: "activity", label: "活动", contentComponent: pane}, {name: "settings", label: "设置", contentComponent: pane}, {name: "locked", label: "归档", disabled: true}]
                }
                Component {
                    id: pane

                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: parent.entry.label + "：独立的标签内容"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETabs"

        ExampleCase {
            objectName: "showcaseETabs1"
            title: ELocale.texts.VerticalLazyTabs
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ETabs {
                        lazy: true
                        height: 220
                        tabPosition: "left"
                        navigationWidth: 120
                        width: Math.min(480, parent.width)
                        model: [{name: "overview", label: "概览", contentComponent: pane}, {name: "activity", label: "活动", contentComponent: pane}, {name: "settings", label: "设置", contentComponent: pane}, {name: "locked", label: "归档", disabled: true}]
                    }
                    Component {
                        id: pane

                        EText {
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: parent.entry.label + "：独立的标签内容"
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ETabs {
                    lazy: true
                    height: 220
                    tabPosition: "left"
                    navigationWidth: 120
                    width: Math.min(480, parent.width)
                    model: [{name: "overview", label: "概览", contentComponent: pane}, {name: "activity", label: "活动", contentComponent: pane}, {name: "settings", label: "设置", contentComponent: pane}, {name: "locked", label: "归档", disabled: true}]
                }
                Component {
                    id: pane

                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: parent.entry.label + "：独立的标签内容"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETabs"

        ExampleCase {
            objectName: "showcaseETabs2"
            title: ELocale.texts.EditableTabs
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ETabs {
                        addable: true
                        movable: true
                        closable: true
                        width: Math.min(480, parent.width)
                        model: [{name: "overview", label: "概览", contentComponent: pane}, {name: "activity", label: "活动", contentComponent: pane}, {name: "settings", label: "设置", contentComponent: pane}, {name: "locked", label: "归档", disabled: true}]

                        onTabAddRequested: model = model.concat([{name: "new" + count, label: "新标签 " + count, contentComponent: pane}])
                    }
                    Component {
                        id: pane

                        EText {
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: parent.entry.label + "：独立的标签内容"
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ETabs {
                    addable: true
                    movable: true
                    closable: true
                    width: Math.min(480, parent.width)
                    model: [{name: "overview", label: "概览", contentComponent: pane}, {name: "activity", label: "活动", contentComponent: pane}, {name: "settings", label: "设置", contentComponent: pane}, {name: "locked", label: "归档", disabled: true}]

                    onTabAddRequested: model = model.concat([{name: "new" + count, label: "新标签 " + count, contentComponent: pane}])
                }
                Component {
                    id: pane

                    EText {
                        width: parent.width
                        wrapMode: Text.WordWrap
                        text: parent.entry.label + "：独立的标签内容"
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESteps"

        ExampleCase {
            objectName: "showcaseESteps0"
            title: ELocale.texts.ClickableSteps
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESteps {
                    active: 1
                    clickable: true
                    alignCenter: true
                    width: Math.min(480, parent.width)
                    model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
                }
            `)

            ESteps {
                active: 1
                clickable: true
                alignCenter: true
                width: Math.min(480, parent.width)
                model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
            }
        }
    }
    ExampleSection {
        componentName: "ESteps"

        ExampleCase {
            objectName: "showcaseESteps1"
            title: ELocale.texts.VerticalSteps
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESteps {
                    active: 1
                    width: 320
                    direction: EOrientation.Vertical
                    model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
                }
            `)

            ESteps {
                active: 1
                width: 320
                direction: EOrientation.Vertical
                model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
            }
        }
    }
    ExampleSection {
        componentName: "ESteps"

        ExampleCase {
            objectName: "showcaseESteps2"
            title: ELocale.texts.SimpleSteps
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESteps {
                    active: 1
                    simple: true
                    width: Math.min(480, parent.width)
                    model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
                }
            `)

            ESteps {
                active: 1
                simple: true
                width: Math.min(480, parent.width)
                model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
            }
        }
    }
    ExampleSection {
        componentName: "ESteps"

        ExampleCase {
            objectName: "showcaseESteps3"
            title: ELocale.texts.StepStatus
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESteps {
                        id: sample

                        processStatus: "error"
                        finishStatus: "success"
                        width: Math.min(480, parent.width)
                        model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
                    }
                    EButton {
                        text: "下一步"

                        onClicked: sample.active = (sample.active + 1) % 4
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESteps {
                    id: sample

                    processStatus: "error"
                    finishStatus: "success"
                    width: Math.min(480, parent.width)
                    model: [{title: "选择", description: "选择工作区"}, {title: "配置", description: "配置项目"}, {title: "完成", description: "准备就绪"}]
                }
                EButton {
                    text: "下一步"

                    onClicked: sample.active = (sample.active + 1) % 4
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMenuBar"

        ExampleCase {
            objectName: "showcaseEMenuBar0"
            title: ELocale.texts.MenuShortcuts
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EMenuBar {
                        width: Math.min(480, parent.width)
                        model: [{text: "文件", children: [{index: "new", text: "新建", shortcut: "Ctrl+N"}, {index: "save", text: "保存", shortcut: "Ctrl+S"}, {index: "close", text: "关闭", divided: true}]}, {text: "编辑", children: [{index: "copy", text: "复制"}]}]

                        onCommand: result.text = index
                    }
                    EText {
                        id: result

                        text: "选择一个命令"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EMenuBar {
                    width: Math.min(480, parent.width)
                    model: [{text: "文件", children: [{index: "new", text: "新建", shortcut: "Ctrl+N"}, {index: "save", text: "保存", shortcut: "Ctrl+S"}, {index: "close", text: "关闭", divided: true}]}, {text: "编辑", children: [{index: "copy", text: "复制"}]}]

                    onCommand: result.text = index
                }
                EText {
                    id: result

                    text: "选择一个命令"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMenuBar"

        ExampleCase {
            objectName: "showcaseEMenuBar1"
            title: ELocale.texts.DisabledCommands
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EMenuBar {
                        width: Math.min(480, parent.width)
                        model: [{text: "项目", children: [{index: "open", text: "打开"}, {index: "locked", text: "归档", disabled: true}]}]

                        onCommand: result.text = index
                    }
                    EText {
                        id: result

                        text: "选择一个命令"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EMenuBar {
                    width: Math.min(480, parent.width)
                    model: [{text: "项目", children: [{index: "open", text: "打开"}, {index: "locked", text: "归档", disabled: true}]}]

                    onCommand: result.text = index
                }
                EText {
                    id: result

                    text: "选择一个命令"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMenuBar"

        ExampleCase {
            objectName: "showcaseEMenuBar2"
            title: ELocale.texts.NestedMenus
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EMenuBar {
                        width: Math.min(480, parent.width)
                        model: [{text: "工具", children: [{text: "导出", children: [{index: "png", text: "PNG"}, {index: "svg", text: "SVG"}]}]}]

                        onCommand: result.text = index
                    }
                    EText {
                        id: result

                        text: "选择一个命令"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EMenuBar {
                    width: Math.min(480, parent.width)
                    model: [{text: "工具", children: [{text: "导出", children: [{index: "png", text: "PNG"}, {index: "svg", text: "SVG"}]}]}]

                    onCommand: result.text = index
                }
                EText {
                    id: result

                    text: "选择一个命令"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EMenuBar"

        ExampleCase {
            objectName: "showcaseEMenuBar3"
            title: ELocale.texts.CommandFeedback
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EMenuBar {
                        width: Math.min(480, parent.width)
                        model: [{text: "视图", children: [{index: "light", text: "浅色"}, {index: "dark", text: "深色"}]}]

                        onCommand: result.text = index
                    }
                    EText {
                        id: result

                        text: "选择一个命令"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EMenuBar {
                    width: Math.min(480, parent.width)
                    model: [{text: "视图", children: [{index: "light", text: "浅色"}, {index: "dark", text: "深色"}]}]

                    onCommand: result.text = index
                }
                EText {
                    id: result

                    text: "选择一个命令"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDropdown"

        ExampleCase {
            objectName: "showcaseEDropdown0"
            title: ELocale.texts.SplitButton
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EDropdown {
                        text: "项目操作"
                        splitButton: true
                        model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                        onCommand: result.text = String(value)
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

                EDropdown {
                    text: "项目操作"
                    splitButton: true
                    model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                    onCommand: result.text = String(value)
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDropdown"

        ExampleCase {
            objectName: "showcaseEDropdown1"
            title: ELocale.texts.HoverDropdown
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EDropdown {
                        text: "项目操作"
                        hideTimeout: 300
                        showTimeout: 150
                        trigger: "hover"
                        model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                        onCommand: result.text = String(value)
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

                EDropdown {
                    text: "项目操作"
                    hideTimeout: 300
                    showTimeout: 150
                    trigger: "hover"
                    model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                    onCommand: result.text = String(value)
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDropdown"

        ExampleCase {
            objectName: "showcaseEDropdown2"
            title: ELocale.texts.DropdownSizing
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EDropdown {
                        text: "项目操作"
                        maxHeight: 140
                        menuWidth: 240
                        showArrow: true
                        model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                        onCommand: result.text = String(value)
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

                EDropdown {
                    text: "项目操作"
                    maxHeight: 140
                    menuWidth: 240
                    showArrow: true
                    model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                    onCommand: result.text = String(value)
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDropdown"

        ExampleCase {
            objectName: "showcaseEDropdown3"
            title: ELocale.texts.PersistentDropdown
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EDropdown {
                        text: "项目操作"
                        hideOnClick: false
                        model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                        onCommand: result.text = String(value)
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

                EDropdown {
                    text: "项目操作"
                    hideOnClick: false
                    model: [{text: "编辑", command: "edit", iconSource: EIcons.PENCIL}, {text: "复制", command: "copy"}, {text: "删除", command: "delete", divided: true, disabled: true}]

                    onCommand: result.text = String(value)
                }
                EText {
                    id: result

                    text: "等待操作"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EBreadcrumb"

        ExampleCase {
            objectName: "showcaseEBreadcrumb0"
            title: ELocale.texts.IconSeparator
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EBreadcrumb {
                    separatorIcon: EIcons.CHEVRON_RIGHT
                    model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
                }
            `)

            EBreadcrumb {
                separatorIcon: EIcons.CHEVRON_RIGHT
                model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
            }
        }
    }
    ExampleSection {
        componentName: "EBreadcrumb"

        ExampleCase {
            objectName: "showcaseEBreadcrumb1"
            title: ELocale.texts.TextSeparator
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EBreadcrumb {
                    separator: "→"
                    model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
                }
            `)

            EBreadcrumb {
                separator: "→"
                model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
            }
        }
    }
    ExampleSection {
        componentName: "EBreadcrumb"

        ExampleCase {
            objectName: "showcaseEBreadcrumb2"
            title: ELocale.texts.ClickableCurrentItem
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EBreadcrumb {
                    clickableLast: true
                    model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
                }
            `)

            EBreadcrumb {
                clickableLast: true
                model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
            }
        }
    }
    ExampleSection {
        componentName: "EBreadcrumb"

        ExampleCase {
            objectName: "showcaseEBreadcrumb3"
            title: ELocale.texts.DisabledBreadcrumb
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EBreadcrumb {
                    enabled: false
                    model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
                }
            `)

            EBreadcrumb {
                enabled: false
                model: [{text: "工作台"}, {text: "项目"}, {text: "详情"}]
            }
        }
    }
}
