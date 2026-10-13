# EUI

用于 Qt Quick / QML 桌面应用的 UI 组件库。Gallery 提供 **120 个组件**的交互示例、QML 源码和 API 参考。

[English](README.md) | 简体中文

[![Windows 构建与打包](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml/badge.svg)](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

![EUI Gallery：组件导航、搜索与交互预览](docs/screenshots/overview-zh.png)

## 主要功能

- **控件与布局**：按钮、输入框、选择器、弹性与栅格布局、菜单、弹窗和通知。
- **表单**：字段校验、日期与时间选择、文件上传。
- **数据展示**：支持编辑、排序、筛选、多选、导入导出和打印的表格，以及树和虚拟列表。
- **图表与动画**：原生图表、通过 WebEngine 接入的 Apache ECharts，以及 83 个动画预设。
- **主题与语言**：共享明暗调色板、局部配置和中英文界面。

<details>
<summary>更多截图：表格、表单与暗色主题</summary>

**可编辑表格**

![可编辑表格：行选择、编辑工具栏与分页](docs/screenshots/table-zh.png)

**表单与校验**

![表单：字段校验与提交反馈](docs/screenshots/form-zh.png)

**暗色主题**

![EUI Gallery 暗色主题，界面为英文](docs/screenshots/overview-dark-en.png)

图标、图表和源码预览见[全部截图](docs/screenshots/README.md)。

</details>

## 快速开始

将 EUI 模块安装到应用的 QML 导入路径后，通过 `import EUI 1.0` 使用组件：

```qml
import EUI 1.0
import QtQuick 2.15

EButton {
    id: button

    text: "播放动画"

    onClicked: EAnimation.play(button, "pulse", { duration: 500 })
}
```

完整应用的代码和接入步骤见[应用接入指南](docs/guide.zh-CN.md#3-接入自己的-qt-quick-应用)。也可以先构建 Gallery，体验组件效果。

**构建 Gallery：** 准备 Windows、Qt 5.15.2 MSVC 2019 x64、CMake 3.20+ 和 Ninja。在 x64 **Developer PowerShell for VS 2019** 中进入仓库根目录，将 `$qtKit` 改为自己的 Qt 路径后执行：

```powershell
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH

cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release `
    "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON -DEUI_BUILD_TESTS=OFF
cmake --build build/local --target example --parallel
& ./bin/Release/example.exe
```

所需 Qt 模块见[环境准备](docs/guide.zh-CN.md#1-环境准备)。构建会将 EUI 复制到 `$qtKit/qml/EUI/`，该目录需要可写。打开 Gallery 后，搜索组件、操作示例，再点击“显示源代码”或“复制源码”。

## 下载并体验

**v1.1.0 · Windows 10 / 11 x64**

[安装版](https://github.com/lihechuan/EUI/releases/download/v1.1.0/EUI-Gallery-1.1.0-windows-x64-setup.exe) · [免安装 ZIP](https://github.com/lihechuan/EUI/releases/download/v1.1.0/EUI-Gallery-1.1.0-windows-x64-portable.zip) · [SHA-256 校验文件](https://github.com/lihechuan/EUI/releases/download/v1.1.0/SHA256SUMS.txt) · [全部版本](https://github.com/lihechuan/EUI/releases)

下载包包含 Gallery 和运行库，无需安装 Qt 或 Visual Studio。可使用安装程序，或完整解压 ZIP 后运行 `EUI-Gallery.exe`，保留所有 DLL 和子目录。安装包未签名；可用 `Get-FileHash 文件路径 -Algorithm SHA256` 与校验文件核对下载内容。

当前构建与发行版验证覆盖 Windows x64 和 Qt 5.15.2 MSVC 2019。Qt 6、Linux、macOS、ARM64 和 MinGW 尚未完成完整的发行版验证。

## 文档

- [使用指南](docs/guide.zh-CN.md)：环境准备、应用接入与组件示例。
- [部署与打包](docs/guide.zh-CN.md#6-部署应用)：分发自己的应用或生成 Gallery 安装包。
- [常见问题](docs/guide.zh-CN.md#7-常见问题)：导入路径、运行库、图表和媒体。
- [更新记录](CHANGELOG.md) · [贡献指南](CONTRIBUTING.md)

## 问题反馈

通过 [GitHub Issues](https://github.com/lihechuan/EUI/issues) 或邮件 [xiaojiaqaq@gmail.com](mailto:xiaojiaqaq@gmail.com) 反馈问题。请附上 Qt 版本、操作系统、组件名称、复现步骤和错误信息；截图或最小示例有助于定位问题。

## 许可证

EUI 自身代码和文档采用 [MIT 许可证](LICENSE)。第三方软件和资源保留各自许可，见[第三方声明](THIRD_PARTY_NOTICES.md)。
