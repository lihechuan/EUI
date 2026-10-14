# EUI

[English](../README.md) | 简体中文

[当前源码版本 **1.2.0**，新增文件/目录选择、工具栏、代码展示、命令面板和步骤向导；Gallery 标记本版本新增组件。Windows 包包含用于 Littlest Tokyo 3D 示例的 Qt 6 运行时，Gallery 支持经过校验的更新安装。

![Windows 构建与打包](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml/badge.svg)](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](../LICENSE)

EUI 是面向 Qt Quick / QML 桌面应用的 UI 组件库，提供表单、数据表格、导航、图表和消息反馈等控件，并使用统一的明暗主题。

**EUI Gallery** 集中了 **126 个组件**的交互示例、可复制的 QML 源码和 API 参考。接入自己的应用前，可以先在 Gallery 中查看组件表现并尝试交互。

![EUI Gallery：组件导航、搜索与交互预览](screenshots/overview-zh.png)

## 组件能力

| 用途 | 组件 | 功能 |
| --- | --- | --- |
| 常用控件 | `EButton`、`EInput`、`ESelect` | 按钮、文本输入、选择，以及尺寸和状态配置 |
| 表单 | `EForm`、`EFormItem`、`EDatePicker`、`ETimePicker` | 字段校验、错误反馈和日期时间输入 |
| 数据展示 | `ETable`、`ETree`、`EList`、`EListy` | 支持编辑、排序、筛选和分页的表格，以及树和虚拟列表 |
| 导航与布局 | `EMenu`、`ETabs`、`EBreadcrumb`、`EFlex` | 菜单、页面导航和弹性布局 |
| 消息与弹窗 | `EApp` | 在应用范围内管理消息、通知和模态对话框 |
| 图表与动画 | `EChart`、`EECharts`、`EAnimation` | 原生图表、通过 WebEngine 接入的 Apache ECharts，以及 83 个动画预设 |
| 外观与语言 | `ETheme`、`ELocale`、`EConfigProvider` | 明暗主题、中英文文本和局部配置 |

更多界面效果见[表格、表单、图表和主题截图](screenshots/README.md)。

## 运行 Gallery

下列构建步骤使用 **Windows x64、Qt 5.15.2 MSVC 2019、CMake 3.20+ 和 Ninja**。准备好[所需 Qt 模块](guide.zh-CN.md#1-环境准备)，在仓库根目录打开 x64 **Developer PowerShell for VS 2019**。

将 `$qtKit` 改为自己的 Qt Kit 路径：

```powershell
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH

cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release `
    "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON `
    -DEUI_BUILD_TESTS=OFF -DQML_PLUGIN_DUMP:FILEPATH=OFF
cmake --build build/local --target example --parallel
& ./bin/Release/example.exe
```

构建会将可执行文件放在 `bin/Release/`，并将 EUI 模块复制到可执行文件旁和 `$qtKit/qml/EUI/`。Qt 导入目录需要可写。

在 Gallery 中：

1. 搜索组件，例如 `ETable`，或从分类中选择。
2. 操作示例，尝试编辑数据和切换组件状态。
3. 点击“显示源代码”或“复制源码”，再查看示例下方的 API。

通过语言和主题控件，可以比较中英文文本及明暗主题的表现。

## 接入自己的应用

使用与应用相同的 Qt Kit 构建并安装组件库，将完整的 `EUI/` 模块目录加入应用的 QML 导入路径。[应用接入指南](guide.zh-CN.md#3-接入自己的-qt-quick-应用)包含组件库构建、CMake 工程、C++ 入口和模块复制命令。

下面的 `main.qml` 创建一个包含计数器和消息提示的窗口。点击“加一”后，绑定的文本会更新，`EApp` 会显示消息：

```qml
import EUI 1.0
import QtQuick 2.15

EApplicationWindow {
    id: window

    property int clicks: 0

    width: 480
    height: 320
    visible: true
    title: "EUI 示例"

    EApp {
        id: app

        anchors.fill: parent

        Column {
            x: 24
            y: 24
            spacing: 16

            EText {
                text: "点击次数：" + window.clicks
            }
            EButton {
                text: "加一"
                styleType: EStyle.Primary

                onClicked: {
                    window.clicks += 1
                    app.message.open("当前计数：" + window.clicks)
                }
            }
        }
    }
}
```

表单模型、表格列和其他控件的配置方式见[常用组件示例](guide.zh-CN.md#4-常用功能是什么何时用如何用)。分发应用时，按[部署步骤](guide.zh-CN.md#6-部署应用)准备运行库和模块文件。

## 参与贡献与反馈

通过 [GitHub Issues](https://github.com/lihechuan/EUI/issues) 提交问题或功能建议，也可以联系 [xiaojiaqaq@gmail.com](mailto:xiaojiaqaq@gmail.com)。请附上组件名称、Qt 版本、操作系统、复现步骤和相关错误信息。

贡献与构建检查见 [CONTRIBUTING.md](../CONTRIBUTING.md)，版本变化见 [CHANGELOG.md](../CHANGELOG.md)。

## 许可证

EUI 自身代码和文档采用 [MIT 许可证](../LICENSE)。第三方软件和资源保留各自许可，见 [THIRD_PARTY_NOTICES.md](../THIRD_PARTY_NOTICES.md)。
