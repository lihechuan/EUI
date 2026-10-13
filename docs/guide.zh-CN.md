# EUI 使用指南

简体中文 | [English](guide.en.md) · [返回首页](../README.md)

本指南使用 **Windows、Qt 5.15.2 MSVC 2019 64 位 Kit、C++17 和 Ninja**。第一次使用时，先选择一条流程：

| 目标 | 按顺序完成 | 完成标志 |
| --- | --- | --- |
| 体验现成组件 | 第 1 节 → 第 2.1 节 | Gallery 打开，能操作组件并查看源码 |
| 创建自己的应用 | 第 1 节 → 第 2.2 节 → 第 3 节 | 点击按钮后计数增加，并出现消息 |
| 接入已有 Qt Quick 项目 | 第 1 节 → 第 2.2 节 → 第 3.5 节 | 现有窗口中可以使用 `import EUI 1.0` |

## 目录

- [1. 环境准备](#1-环境准备)
- [2. 构建 EUI](#2-构建-eui)
- [3. 接入自己的 Qt Quick 应用](#3-接入自己的-qt-quick-应用)
- [4. 常用功能：是什么、何时用、如何用](#4-常用功能是什么何时用如何用)
- [5. 使用 Gallery 学习组件](#5-使用-gallery-学习组件)
- [6. 部署应用](#6-部署应用)
- [7. 常见问题](#7-常见问题)

## 1. 环境准备

### 1.1 安装工具并打开终端

1. 安装 Visual Studio 2019 的 C++ 开发工具，使用 x64 编译环境。
2. 安装 Qt 5.15.2 的 `msvc2019_64` Kit，准备下表中的模块。
3. 安装 CMake 3.20 或更高版本及 Ninja，并将它们加入 PATH。
4. 打开 **Developer PowerShell for VS 2019**。后续命令都在这个窗口执行。

| 用途 | Qt 模块或文件 |
| --- | --- |
| 构建组件库 | Core、Gui、Qml、Quick、Widgets、Network、Concurrent、PrintSupport；匹配 Kit 的 `GuiPrivate`、`QmlPrivate` 私有开发头文件 |
| 构建 Gallery | 组件库依赖，以及 WebEngine、WebChannel |
| 本章最小应用 | QtQuick 2.15、QtQml 2.15、QtQuick.Window 2.15、QtQuick.Controls 2.15、QtGraphicalEffects 1.15 |
| 按需使用其他组件 | 日期面板需要 Qt.labs.calendar；文件对话框需要 QtQuick.Dialogs；媒体需要 QtMultimedia；`EECharts` 需要 QtWebEngine、QtWebChannel；SVG 图标需要 SVG 图片格式插件 |

在终端执行 `cmake --version`、`ninja --version`、`cl`，确认工具能够找到；`cl` 无参数显示版本和用法即可。

### 1.2 设置仓库和 Qt 路径

将两条路径改成你的实际目录：

```powershell
$euiRoot = 'D:/code/EUI'
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH
Set-Location $euiRoot
```

`$euiRoot` 指向本仓库，目录里应有根 `CMakeLists.txt` 和 `source/`；`$qtKit` 指向包含 `bin/`、`lib/`、`qml/` 的 Qt Kit。后续命令沿用这两个变量，重新打开终端时先执行本节。

## 2. 构建 EUI

选择 2.1 或 2.2 执行即可。创建自己的应用时使用 2.2；它会生成可以复制到应用中的模块目录。

### 2.1 体验 Gallery

**执行位置：EUI 仓库根目录。**

```powershell
Set-Location $euiRoot
cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON -DEUI_BUILD_TESTS=OFF
cmake --build build/local
& ./bin/Release/example.exe
```

**完成标志：** Gallery 打开。搜索 `EButton`，操作按钮，点击“显示源代码”和“复制源码”；页面下方显示组件 API。

程序在 `$euiRoot/bin/Release/example.exe`，组件模块在旁边的 `EUI/`。构建还会将模块复制到 `$qtKit/qml/EUI/`，这个 SDK 目录需要可写。

### 2.2 为自己的应用构建组件库

**执行位置：EUI 仓库根目录。**

```powershell
Set-Location $euiRoot
cmake -S . -B build/library -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=OFF -DEUI_BUILD_TESTS=OFF
cmake --build build/library
cmake --install build/library --prefix build/eui-install
```

**完成标志：** `$euiRoot/build/eui-install/imports/EUI/` 中有 `qmldir`、`euiplugin.dll` 和其余模块文件。后续复制这个完整目录。以上使用 Release 配置，应用也使用同一 Qt Kit 和 Release 配置。

## 3. 接入自己的 Qt Quick 应用

本节从零创建 `MyEuiApp`。完成后，窗口显示“点击次数：0”；每次点击“点击加一”，数字增加并出现消息。

### 3.1 创建应用目录

先完成 2.2。下面路径是你的新应用目录，与 EUI 仓库是两个独立目录：

```powershell
$appRoot = 'D:/code/MyEuiApp'
New-Item -ItemType Directory -Force -Path $appRoot | Out-Null
Set-Location $appRoot
```

在 `$appRoot` 中创建四个 UTF-8 文本文件，目录结构为：

```text
MyEuiApp/
  CMakeLists.txt
  main.cpp
  main.qml
  resources.qrc
```

### 3.2 写入完整示例

将下面四个代码块分别保存到对应文件。文件名必须一致，`resources.qrc` 中的 `main.qml` 与实际文件名对应。

**`CMakeLists.txt`：定义程序和 Qt 依赖。**

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyEuiApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 5.15 REQUIRED COMPONENTS Quick Qml)

add_executable(MyEuiApp main.cpp resources.qrc)
target_link_libraries(MyEuiApp PRIVATE Qt5::Quick Qt5::Qml)
```

**`main.cpp`：创建 QML 引擎，添加组件搜索目录，加载窗口。**

```cpp
#include <QUrl>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    engine.addImportPath(QCoreApplication::applicationDirPath());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;
    return app.exec();
}
```

**`resources.qrc`：将 QML 窗口打包到程序中。**

```xml
<RCC>
    <qresource prefix="/">
        <file>main.qml</file>
    </qresource>
</RCC>
```

**`main.qml`：窗口、计数文字、按钮和消息交互。**

```qml
import EUI 1.0
import QtQuick 2.15

EApplicationWindow {
    id: window

    property int clickCount: 0

    width: 640
    height: 420
    visible: true
    title: "EUI Demo"

    Component.onCompleted: {
        ELocale.language = "zh"
        ETheme.dark = false
    }

    EApp {
        id: app

        anchors.fill: parent

        Column {
            x: 24
            y: 24
            spacing: 16

            EText {
                text: "点击次数：" + window.clickCount
            }
            EButton {
                text: "点击加一"
                styleType: EStyle.Primary

                onClicked: {
                    window.clickCount += 1
                    app.message.open("已点击 " + window.clickCount + " 次")
                }
            }
        }
    }
}
```

`EApplicationWindow` 是窗口；`EApp` 填满窗口并提供消息管理器；`EText.text` 绑定计数；`EButton.onClicked` 更新计数并调用 `app.message.open()`。该示例没有保存或网络请求。

### 3.3 构建应用并复制模块

**执行位置：新应用目录 `$appRoot`。** 继续使用第 1 节设置的 `$euiRoot` 和 `$qtKit`：

```powershell
Set-Location $appRoot
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit"
cmake --build build
cmake -E copy_directory "$euiRoot/build/eui-install/imports/EUI" "$appRoot/build/EUI"
```

完成后的关键文件如下：

```text
MyEuiApp/
  CMakeLists.txt
  main.cpp
  main.qml
  resources.qrc
  build/
    MyEuiApp.exe
    EUI/
      qmldir
      euiplugin.dll
      plugins.qmltypes
      Controls/
      ...其余模块文件
```

必须复制整个 `EUI/`，包括插件 DLL。`main.cpp` 添加的是可执行文件目录 `build/`，QML 引擎会从其下的 `EUI/` 找到 `import EUI 1.0`。应用的 CMake 只链接 Qt，EUI 插件通过 QML 导入加载。

### 3.4 运行并检查结果

**执行位置：新应用目录 `$appRoot`，保持同一终端中的 Qt PATH 设置。**

```powershell
& ./build/MyEuiApp.exe
```

1. 窗口标题为 `EUI Demo`，文字为“点击次数：0”。
2. 点击“点击加一”，文字变为“点击次数：1”，同时出现“已点击 1 次”消息。
3. 再点击一次，文字变为“点击次数：2”。

修改 `main.qml` 后先关闭程序，再执行 `cmake --build build` 并重新运行。QML 已打包进可执行文件，修改源文件需要重新构建。此时 Qt 运行库来自第 1 节的 PATH；交给其他电脑运行前，按第 6 节部署运行库。

### 3.5 接入已有项目或替换示例组件

已有 Qt Quick 项目可以继续使用原来的 C++ 入口、窗口和构建配置。构建 EUI 后，把完整模块复制到可执行文件旁，给现有 QML 引擎添加该父目录，在 QML 文件顶部写 `import EUI 1.0`，然后放入所需控件。使用消息管理器时，把界面内容放进 `EApp`。

第 4 节的代码块都是独立组件定义。要在本章示例中试用：

1. 保留 `EApplicationWindow`、`EApp` 和窗口初始化代码。
2. 用所选示例的根对象替换 `EApp` 内的整个 `Column { ... }`。
3. 将所需 imports 放在 `main.qml` 顶部，不放进对象内部。
4. 关闭程序，重新构建并运行。

例如，表格示例的 `ETable { ... }` 放入 `EApp` 后会显示两行数据；表单示例点击空内容的“提交”会显示必填校验。修改业务数据和信号处理器即可接入自己的逻辑。使用 `EECharts` 前，先按 4.5 配置 WebEngine 宿主。

## 4. 常用功能：是什么、何时用、如何用

以下代码块是组件定义，均列出所需 imports。按第 3.5 节将根对象放入窗口，imports 留在文件顶部。信号处理器中的业务逻辑由应用实现。

### 4.1 主题、语言与配置

**是什么：** `ETheme` 控制全局明暗主题，`ELocale` 提供中英文内置文本，`EConfigProvider` 为子控件提供默认尺寸和局部配置。

**何时用：** 为整套应用统一外观，或让某一区域使用紧凑尺寸、禁用状态及局部语言设置。

```qml
import EUI 1.0
import QtQuick 2.15

EConfigProvider {
    width: 320
    height: 120
    locale: "en"
    size: EStyle.Small

    EButton {
        text: "紧凑按钮"
        anchors.centerIn: parent
    }
}
```

在窗口初始化或语言切换事件中设置 `ELocale.language = "zh"` / `"en"`、`ETheme.dark = false` / `true`。`size` 支持 `EStyle.Small`、`Medium`、`Large`；控件显式设置的尺寸优先于默认值。Provider 的 `disabled: true` 会禁用其子控件。

语言切换更新内置标签、提示和 Gallery 文本。你直接写入的 `text`、`label` 和业务数据不会自动翻译，应使用应用自己的翻译或绑定。内置语言资源位于 [zh.json](../source/Qt5/imports/EUI/i18n/zh.json) 和 [en.json](../source/Qt5/imports/EUI/i18n/en.json)。

Provider 的 `locale: "en"` 或 `"zh"` 只设置该内容区域中支持配置继承的控件文本，不改变全局语言。也可传入 `locale: ({name: "zh", texts: {SelectAnOption: "选择项目"}})` 覆盖局部内置文案。

自定义控件颜色可使用 `EPaletteManager.ThemePalette.textRegular(ETheme.dark)`、`surfaceComponent(ETheme.dark)` 等调色板方法；状态色使用 `fetchColorForStyle(EStyle.Primary)`。

### 4.2 表单与选择器

**是什么：** `EForm` 管理模型与校验规则，`EFormItem` 关联模型字段及输入控件；`ESelect` 将显示文本与选项值分开。

**何时用：** 设置页、用户信息编辑、筛选条件或提交前必须检查数据的表单。

```qml
import EUI 1.0
import QtQuick 2.15

Column {
    width: 480
    spacing: 16

    EForm {
        id: form

        width: parent.width
        model: ({name: ""})
        rules: ({name: [{required: true, message: "请输入姓名"}]})

        onSubmitted: {
            if (valid)
                console.log("提交姓名", model.name)
        }

        EFormItem {
            label: "姓名"
            prop: "name"
            width: parent.width

            EInput {
                width: parent.width
                placeholderText: "请输入姓名"
            }
        }
    }
    EButton {
        text: "提交"

        onClicked: form.submit()
    }
}
```

`prop` 对应模型字段，`submit()` 触发校验及 `submitted(valid, model)`。通过 `validate(callback)` 仅执行校验，`resetFields()` 恢复初始字段值，`clearValidate()` 清除校验反馈。存在异步规则时，在回调或完成信号中读取最终校验结果。

```qml
import EUI 1.0
import QtQuick 2.15

ESelect {
    width: 240
    value: "list"
    clearable: true
    textRole: "label"
    valueRole: "value"
    placeholderText: "选择视图"
    model: [{label: "列表", value: "list"}, {label: "网格", value: "grid"}]

    onChanged: console.log(value)
}
```

`textRole` 是用户看到的名称，`valueRole` 是业务使用的值。通过 `value` 设置当前选项；`clear()` 清空选择。选项对象可通过 `disabled: true` 禁止选择。

日期选择使用 `EDatePicker.value`，格式为 `yyyy-MM-dd`；`minDate` / `maxDate` 限制范围，`disabledDate(date)` 接收 JavaScript `Date`。内嵌日期面板用 `EDatePickerPanel`。

### 4.3 表格、树与长列表

**是什么：** `ETable` 将行模型与列配置分开；`ETree` 展示父子节点；`EListy` 为长列表提供按需创建与复用的委托。

**何时用：** 数据台账、文件层级、导航树以及需要滚动浏览的大量条目。

```qml
import EUI 1.0
import QtQuick 2.15

ETable {
    width: 640
    height: 280
    model: [{name: "Alice", score: 92}, {name: "Bob", score: 88}]
    columns: [
        {prop: "name", label: "姓名"},
        {prop: "score", label: "分数", width: 120, sortable: true}
    ]

    onRowClicked: console.log(index, row.name)
}
```

`columns[].prop` 对应行对象字段，`label` 是列标题。设置固定的视口高度，行委托通过 `ListView` 创建。`sort("score", "ascending")` / `sort("score", "descending")` 排序，空排序字符串清除排序；列需配置 `sortable: true`。

`currentRowIndex` 和 `selectRow(index)` 使用原始模型索引，排序后仍保持该含义。`currentRow` 返回选择的行，`clearSelection()` 清除选择。替换整个 `model` 数组以通知 QML 数据更新。编辑、筛选、树行、工具栏和导出等配置见 Gallery 的 `ETable` 页面。

```qml
import EUI 1.0
import QtQuick 2.15

ETree {
    width: 320
    height: 240
    expandedKeys: ["root"]
    model: [{
        id: "root",
        label: "工作区",
        children: [{id: "docs", label: "文档"}]
    }]

    onNodeSelected: console.log(key, node.label)
}
```

给节点提供唯一字符串 `id` 作为稳定 key。通过 `select(key)` 选择节点，`setExpanded(key, expanded)` 设置展开状态；读取 `currentKey` / `currentNode` 获取选择。父节点禁用时，后代也会禁用。

列表组件按数据量和展示需求选择：

| 组件 | 用法 |
| --- | --- |
| `EList` / `EListItem` | 使用 JavaScript 数组展示标题、说明、头像及操作，支持分页和响应式网格；`grid` 可配置 `columns`、`gutter` 和 `itemHeight` |
| `EListy` | 将 JavaScript 数组赋给 `items`，设置视口宽高和 `rowKey`，默认按需创建、复用委托，适合长列表 |
| `EInfiniteScroll` | 需要直接使用 `QAbstractItemModel`、`ListModel` 或增量加载时使用 |

`EListy` 的条目可采用 `{id: 10, title: "Alpha", category: "A"}` 等对象，并设置 `rowKey: "id"`。用 `group: ({key: "category"})` 按字段分组；`group.key` 也接受 `(item) => key` 函数。更新列表时重新赋值 `items` 数组。

通过 `scrollTo(200)` 按像素滚动，或 `scrollTo({key: 10, align: "top", offset: 8})` 按稳定 key 定位；`align` 支持 `top/bottom/auto`。`scrollTo({groupKey: "A", align: "top"})` 定位到分组的首条数据。请求无效、key 重复或不存在时返回 `false`；分组回调失败时可读取 `errorString`。

自定义 `itemDelegate` 可以声明 `itemData`、`itemIndex`、`itemKey`、`groupKey` 和 `listControl` 属性，组件会更新这些字段。关闭 `virtual` 会缓存全部内容，适合少量数据。

### 4.4 文件上传与应用消息

**是什么：** `EUpload` 提供文件选择、队列及上传交互；`EApp` 为内容区域提供 `message`、`modal`、`notification` 管理器。

**何时用：** 选择附件、上传图片、提示保存结果或弹出业务确认。

```qml
import EUI 1.0
import QtQuick 2.15

EUpload {
    limit: 3
    width: 480
    maxFileSize: 5 * 1024 * 1024
    nameFilters: ["Images (*.png *.jpg)"]

    onSelected: console.log("已选择", files.length)
    onFailed: console.log(file.name, message)
    onSucceeded: console.log(file.name, response)
}
```

默认 `autoUpload: false`，上述示例仅选择本地文件。真正上传需要配置你的 HTTP(S) `action` 地址，再调用 `upload(index)` 或 `submit()`，也可以开启 `autoUpload`。请求默认使用 multipart，字段名是 `fieldName`（默认 `"file"`），可提供 `headers`。`maxFileSize` 单位为字节，`limit: 0` 表示不限制数量。`fileList` 提供文件及状态；`cancel()` 中止当前上传，`removeFile(index)` 移除条目。

用 `app.message.open("已保存")` 显示消息，第 3.2 节的完整窗口演示了按钮触发消息的用法。也可通过 `app.useApp()` 获取当前实例的管理器。`EApp` 是内容容器，需放入已有窗口；保存数据或调用服务的逻辑由应用实现。

### 4.5 图表选择

| 组件 | 是什么 | 适用情况 |
| --- | --- | --- |
| `EChart` | 使用 Qt 原生绘制的折线、柱状和面积图 | 简单数值序列、较低运行开销、软件渲染 |
| `EECharts` | 通过 Qt WebEngine 承载本地 Apache ECharts 分发文件 | ECharts option、多种图表、图例、缩放和事件交互 |

原生折线图：

```qml
import EUI 1.0
import QtQuick 2.15

EChart {
    width: 480
    height: 240
    type: EChart.Line
    model: [12, 18, 15, 26, 32, 28]
}
```

`EChart.type` 支持 `Line`、`Bar`、`Area`；`model` 可使用数值数组，也可使用 `{label, value}` 数据点数组，例如 `[{label: "周一", value: 12}, {label: "周二", value: 18}]`。`value` 是数值，`label` 用作横轴标签；省略标签时显示从 1 开始的序号。

`EECharts` 示例：

```qml
import EUI 1.0
import QtQuick 2.15

EECharts {
    width: 640
    height: 360
    renderer: "canvas"
    option: ({
        xAxis: {type: "category", data: ["Mon", "Tue", "Wed"]},
        yAxis: {type: "value"},
        series: [{type: "bar", data: [12, 20, 16]}]
    })

    onFailed: console.log(message)
}
```

该组件的 ECharts 脚本、HTML 和桥接资源在插件内本地打包，加载图表引擎不需要 CDN；远程数据或图片是否需要网络由你的 `option` 决定。`option` 必须能通过 JSON 传输，因此 JavaScript formatter 函数不能通过此桥接发送。读取 `ready` / `errorString`，使用 `setOption()`、`dispatchAction()` 等方法，并监听 `clicked`、`legendSelected`、`dataZoomed`。

使用 Qt 5 的 `EECharts` 时，将最小宿主的 `main.cpp` 增加头文件：

```cpp
#include <QtWebEngine/qtwebengineglobal.h>
```

在构造 `QGuiApplication` **之前**调用：

```cpp
QtWebEngine::initialize();
```

并在应用 CMake 中增加依赖：

```cmake
find_package(Qt5 5.15 REQUIRED COMPONENTS WebEngine WebChannel)
target_link_libraries(MyEuiApp PRIVATE Qt5::WebEngine Qt5::WebChannel)
```

Gallery 已实现宿主初始化，可参考 [example/src/main.cpp](../example/src/main.cpp)。`QT_QUICK_BACKEND=software` 下 `EECharts` 显示不可用说明，不创建 WebEngine 图表视图。启用该组件时应移除这项环境变量，并验证目标机器的图形后端。不要将 Qt Quick 软件场景图与 `QT_OPENGL=software` 混为同一设置。

### 4.6 布局、悬浮按钮与边框效果

**是什么：** `EFlex` 分配子项空间，`EFloatButton` / `EFloatButtonGroup` 提供悬浮操作入口，`EBorderBeam` 绘制沿边框运动的装饰流光。

**何时用：** 工具栏、可伸缩面板、固定位置的快捷操作，以及需要突出显示的卡片。

| 组件 | 关键配置 |
| --- | --- |
| `EFlex` | `gap` 设置间距，可使用数字或 `[horizontal, vertical]`；`justify` 支持 `start/center/end/space-between/space-around/space-evenly`，`align` 支持 `start/center/end/stretch` |
| `EFlexItem` | 包裹需要分配剩余空间的内容；`grow` / `shrink` 控制增长和收缩，`basis` 指定基础尺寸，`minSize` / `maxSize` 限制尺寸 |
| `EFloatButton` | 通过 anchors 或 x/y 放置；可设置图标、文字、徽标、提示和禁用状态 |
| `EFloatButtonGroup` | `trigger: "click"` 或 `"hover"` 展开菜单，空值表示常驻；`placement` 支持 `top/bottom/left/right` |
| `EBorderBeam` | 设置 `count`、`size`、`radius`、`duration`、`color` 等参数；`duration` 单位为秒，`running: false` 暂停，`reducedMotion: true` 停止并隐藏装饰 |

普通 `EFlex` 子项按现有尺寸排列，使用 `EFlexItem` 才启用增长和收缩。`basis: -1` 使用内容的隐式尺寸；自定义内容应提供 `implicitWidth/implicitHeight`，或显式设置 `basis`。不要同时给布局管理的子项设置位置 anchors。

悬浮按钮组的菜单可以超出组本身的边界，父项设置 `clip: true` 时会裁剪菜单。边框流光不捕获输入，内部内容仍可操作。完整交互与可复制的示例见 Gallery 中各组件页面。

## 5. 使用 Gallery 学习组件

Gallery 左侧提供分类导航和名称搜索；`Ctrl+K` 聚焦搜索框。右上角切换语言和主题。每个组件页面包含可操作的示例卡片、源码按钮及页面下方的 API 文档。

![英文按钮页的源码面板：交互预览、展开源码与复制源码](screenshots/source-en.png)

推荐按以下顺序学习：

1. 搜索控件名，例如 `EInput`，先在预览中验证所需交互。
2. 展开对应卡片的源码，确认输入模型、关键属性和事件处理器。
3. 复制源码到已有窗口，调整布局及业务文字。
4. 阅读页面下方的 API，确认默认值、信号参数和方法用途。
5. 为自己的业务模型、提交或网络请求编写逻辑，并验证实际结果。

## 6. 部署应用

### 6.1 部署本章的 MyEuiApp

先完成第 3 节并关闭程序。在第 1 节使用的终端执行，下面使用绝对路径，无需切换工作目录：

```powershell
& "$qtKit/bin/windeployqt.exe" --release --qmldir "$appRoot" --qmlimport "$appRoot/build" "$appRoot/build/MyEuiApp.exe"
```

`--qmldir` 指向 QML 源文件目录，`--qmlimport` 指向包含当前 `EUI/` 的父目录。部署工具将 Qt 动态库、QML 运行模块和平台插件放到程序目录。保留第 3.3 节已复制的完整 `EUI/`，将整个 `$appRoot/build/` 目录交给目标电脑运行，其中的 `MyEuiApp.exe` 是入口。先在没有开发环境的目标机器上检查程序能打开、按钮和消息能工作；其他应用将参数替换为自己的源码目录、模块父目录和可执行文件。

### 6.2 部署 Gallery 或额外组件

部署第 2.1 节的 Gallery 时执行：

```powershell
& "$qtKit/bin/windeployqt.exe" --release --qmldir "$euiRoot/example/qml" --qmlimport "$euiRoot/bin/Release" "$euiRoot/bin/Release/example.exe"
```

分发整个 `$euiRoot/bin/Release/` 目录，包括旁边的 `EUI/` 和部署工具生成的运行文件。

使用 `EECharts` 时还需部署 Qt WebEngine / WebChannel、`QtWebEngineProcess`、Chromium 资源文件和 `qtwebengine_locales`，保持所用 Qt Kit 的目录结构。使用媒体组件时部署 Qt Multimedia；使用 SVG 图标时部署 SVG 图片格式插件。

## 7. 常见问题

| 现象 | 检查方式 |
| --- | --- |
| `module "EUI" is not installed` | 导入路径是否指向完整 `EUI/` 的父目录？目录里是否有 `qmldir`？ |
| 插件无法加载或提示 DLL 缺失 | 检查 Qt 版本、架构、编译器及 Debug/Release 一致；检查插件依赖和 Qt 运行库是否已部署 |
| 只复制了 QML，主题或工具不可用 | 复制完整构建产物，包含编译后的插件 |
| `cl`、`ninja` 或 Qt 找不到 | 使用 Developer PowerShell，确认工具在 PATH 中，并修正 `CMAKE_PREFIX_PATH` |
| 更新表格或树数组后 UI 不刷新 | 重新赋值一个新的 `model` 数组；避免只原地修改普通 JavaScript 数组 |
| 语言切换后业务文字未改变 | `ELocale` 只管理内置文本；给业务 `text` / `label` 添加应用自己的翻译绑定 |
| 选择文件后未上传 | 默认不会自动发送；配置 `action`，调用上传方法或启用 `autoUpload` |
| `EECharts` 提示软件后端不可用 | 移除 `QT_QUICK_BACKEND=software`，确认宿主初始化、图形后端及 WebEngine 部署 |
| 品牌图标或视频无法显示 | 检查 SVG 插件或 Multimedia 后端及支持的媒体格式 |
