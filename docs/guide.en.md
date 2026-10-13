# EUI Usage Guide

[简体中文](guide.zh-CN.md) | English · [Back to the project](../README.en.md)

This guide uses **Windows, the Qt 5.15.2 MSVC 2019 64-bit Kit, C++17, and Ninja**. Choose one path when starting:

| Goal | Follow in order | Success looks like |
| --- | --- | --- |
| Try existing components | Section 1 → Section 2.1 | The Gallery opens with interactive examples and source |
| Create your application | Section 1 → Section 2.2 → Section 3 | Clicking a button increases a counter and shows a message |
| Integrate an existing Qt Quick project | Section 1 → Section 2.2 → Section 3.5 | Your existing window can use `import EUI 1.0` |

To try the Gallery without a development environment, use the [installer or portable download](../README.en.md#download-and-try). To create release packages yourself, see [section 6.3](#63-build-a-windows-installer-and-portable-package).

## Contents

- [1. Prerequisites](#1-prerequisites)
- [2. Build EUI](#2-build-eui)
- [3. Integrate a Qt Quick application](#3-integrate-a-qt-quick-application)
- [4. Common features: purpose and usage](#4-common-features-purpose-and-usage)
- [5. Learn components with the Gallery](#5-learn-components-with-the-gallery)
- [6. Deploy your application](#6-deploy-your-application)
- [7. Troubleshooting](#7-troubleshooting)

## 1. Prerequisites

### 1.1 Install the tools and open a terminal

1. Install the Visual Studio 2019 C++ development tools and use an x64 compilation environment.
2. Install the Qt 5.15.2 `msvc2019_64` Kit with the modules listed below.
3. Install CMake 3.20 or newer and Ninja, and add them to PATH.
4. Open **Developer PowerShell for VS 2019**. Run the remaining commands in this window.

| Purpose | Qt modules or files |
| --- | --- |
| Build the library | Core, Gui, Qml, Quick, Widgets, Network, Concurrent, PrintSupport, and matching `GuiPrivate` and `QmlPrivate` private development headers |
| Build the Gallery | Library dependencies plus WebEngine and WebChannel |
| Minimal application in this chapter | QtQuick 2.15, QtQml 2.15, QtQuick.Window 2.15, QtQuick.Controls 2.15, QtGraphicalEffects 1.15 |
| Additional controls as needed | Qt.labs.calendar for date panels; QtQuick.Dialogs for file dialogs; QtMultimedia for media; QtWebEngine and QtWebChannel for `EECharts`; the SVG image-format plugin for SVG icons |

Run `cmake --version`, `ninja --version`, and `cl` to confirm the tools are available. Running `cl` without arguments should display its version and usage.

### 1.2 Set repository and Qt paths

Replace the two paths with your actual directories:

```powershell
$euiRoot = 'D:/code/EUI'
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH
Set-Location $euiRoot
```

`$euiRoot` is this repository, containing the root `CMakeLists.txt` and `source/`. `$qtKit` contains Qt's `bin/`, `lib/`, and `qml/` directories. Later commands reuse these variables; repeat this section after opening a new terminal.

## 2. Build EUI

Choose either 2.1 or 2.2. Use 2.2 when creating your application; it produces the module directory you will copy into it.

### 2.1 Try the Gallery

**Working directory: the EUI repository root.**

```powershell
Set-Location $euiRoot
cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON -DEUI_BUILD_TESTS=OFF
cmake --build build/local
& ./bin/Release/example.exe
```

**Success:** The Gallery opens. Search for `EButton`, try the buttons, and select **Show source** and **Copy source**. The API appears below the examples.

The executable is at `$euiRoot/bin/Release/example.exe`, with the component module in the adjacent `EUI/` directory. Building also copies the module to `$qtKit/qml/EUI/`; this SDK directory must be writable.

### 2.2 Build the library for your application

**Working directory: the EUI repository root.**

```powershell
Set-Location $euiRoot
cmake -S . -B build/library -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=OFF -DEUI_BUILD_TESTS=OFF
cmake --build build/library
cmake --install build/library --prefix build/eui-install
```

**Success:** `$euiRoot/build/eui-install/imports/EUI/` contains `qmldir`, `euiplugin.dll`, and the remaining module files. Copy this complete directory in the next section. These commands build Release; use the same Qt Kit and Release configuration for your application.

## 3. Integrate a Qt Quick application

This section creates `MyEuiApp` from scratch. The window initially displays **Clicks: 0**. Each click on **Add one** increases the count and shows a message.

### 3.1 Create the application directory

Complete 2.2 first. The following path is your new application's directory, separate from the EUI repository:

```powershell
$appRoot = 'D:/code/MyEuiApp'
New-Item -ItemType Directory -Force -Path $appRoot | Out-Null
Set-Location $appRoot
```

Create four UTF-8 text files in `$appRoot`:

```text
MyEuiApp/
  CMakeLists.txt
  main.cpp
  main.qml
  resources.qrc
```

### 3.2 Save the complete example

Save each of the following four blocks to its named file. Keep the filenames unchanged; `main.qml` in `resources.qrc` must match the actual filename.

**`CMakeLists.txt`: define the application and Qt dependencies.**

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

**`main.cpp`: create the QML engine, add the module search directory, and load the window.**

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

**`resources.qrc`: embed the QML window in the application.**

```xml
<RCC>
    <qresource prefix="/">
        <file>main.qml</file>
    </qresource>
</RCC>
```

**`main.qml`: create the window, counter, button, and message interaction.**

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
        ELocale.language = "en"
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
                text: "Clicks: " + window.clickCount
            }
            EButton {
                text: "Add one"
                styleType: EStyle.Primary

                onClicked: {
                    window.clickCount += 1
                    app.message.open("Clicked " + window.clickCount + " times")
                }
            }
        }
    }
}
```

`EApplicationWindow` creates the window. `EApp` fills it and supplies a message manager. `EText.text` binds to the count. `EButton.onClicked` updates the count and calls `app.message.open()`. This example does not save data or send network requests.

### 3.3 Build the application and copy the module

**Working directory: your new application directory, `$appRoot`.** Reuse `$euiRoot` and `$qtKit` from section 1:

```powershell
Set-Location $appRoot
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit"
cmake --build build
cmake -E copy_directory "$euiRoot/build/eui-install/imports/EUI" "$appRoot/build/EUI"
```

The key files now look like this:

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
      ...remaining module files
```

Copy all of `EUI/`, including the plugin DLL. `main.cpp` adds the executable directory, `build/`, to the import path; the engine finds `import EUI 1.0` in its `EUI/` child directory. The application's CMake links Qt, and QML imports load the EUI plugin.

### 3.4 Run and check the result

**Working directory: `$appRoot`, keeping the Qt PATH from the same terminal.**

```powershell
& ./build/MyEuiApp.exe
```

1. The window is titled `EUI Demo` and displays **Clicks: 0**.
2. Select **Add one**. The text becomes **Clicks: 1**, and **Clicked 1 times** appears as a message.
3. Click again. The text becomes **Clicks: 2**.

After editing `main.qml`, close the application, run `cmake --build build`, and launch it again. QML is embedded in the executable, so source changes require a rebuild. Qt runtime libraries currently come from the PATH set in section 1; deploy them using section 6 before running on another computer.

### 3.5 Use an existing project or replace the example control

Keep your existing Qt Quick project's C++ entry point, window, and build configuration. Build EUI, copy the complete module beside the executable, and add that parent directory to your existing QML engine's import paths. Add `import EUI 1.0` at the top of your QML file and insert the controls you need. Place your content inside `EApp` when using its message managers.

Section 4's code blocks are standalone component definitions. To try one in this application's window:

1. Keep `EApplicationWindow`, `EApp`, and the window initialization code.
2. Replace the entire `Column { ... }` inside `EApp` with the selected example's root object.
3. Keep required imports at the top of `main.qml`, outside objects.
4. Close the application, rebuild, and run it again.

For example, placing the `ETable { ... }` example inside `EApp` displays two rows. Submitting the form example with an empty name shows required-field feedback. Replace the business data and signal handlers with your own logic. Before using `EECharts`, configure the WebEngine host as described in 4.5.

## 4. Common features: purpose and usage

The following blocks are component definitions with their required imports. Place each root object inside a window as described in section 3.5, and keep imports at the top of the file. Your application implements the business logic in signal handlers.

### 4.1 Themes, language, and configuration

**Purpose:** `ETheme` controls the global light/dark theme, `ELocale` supplies built-in Chinese and English text, and `EConfigProvider` supplies default sizes and scoped settings to child controls.

**When to use it:** Keep appearance consistent across an application, or give a particular region compact sizes, a disabled state, or a scoped locale.

```qml
import EUI 1.0
import QtQuick 2.15

EConfigProvider {
    width: 320
    height: 120
    locale: "en"
    size: EStyle.Small

    EButton {
        text: "Compact button"
        anchors.centerIn: parent
    }
}
```

Set `ELocale.language = "zh"` / `"en"` and `ETheme.dark = false` / `true` during window initialization or in a language/theme action. Sizes are `EStyle.Small`, `Medium`, and `Large`; a control's explicit size overrides its inherited default. A provider with `disabled: true` disables its descendants.

Language changes update built-in labels, hints, and Gallery text. Your own `text`, `label`, and business data do not translate automatically; use your application's localization bindings. Built-in locale resources are [zh.json](../source/Qt5/imports/EUI/i18n/zh.json) and [en.json](../source/Qt5/imports/EUI/i18n/en.json).

Set a provider's `locale: "en"` or `"zh"` to change built-in text only within that region for controls supporting configuration inheritance, without changing the global language. You can also pass `locale: ({name: "en", texts: {SelectAnOption: "Choose an item"}})` to override scoped built-in text.

For custom control colors, use palette methods such as `EPaletteManager.ThemePalette.textRegular(ETheme.dark)` and `surfaceComponent(ETheme.dark)`. For status colors, use `fetchColorForStyle(EStyle.Primary)`.

### 4.2 Forms and selectors

**Purpose:** `EForm` manages a model and validation rules, while `EFormItem` connects a model field to its input. `ESelect` separates display labels from selected values.

**When to use it:** Settings, profile editors, filters, or forms that must validate data before submission.

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
        rules: ({name: [{required: true, message: "Enter a name"}]})

        onSubmitted: {
            if (valid)
                console.log("Submit name", model.name)
        }

        EFormItem {
            prop: "name"
            label: "Name"
            width: parent.width

            EInput {
                width: parent.width
                placeholderText: "Enter a name"
            }
        }
    }
    EButton {
        text: "Submit"

        onClicked: form.submit()
    }
}
```

`prop` names the model field; `submit()` validates and emits `submitted(valid, model)`. Use `validate(callback)` for validation alone, `resetFields()` to restore initial values, and `clearValidate()` to remove validation feedback. For asynchronous rules, read the final result in a callback or completion signal.

```qml
import EUI 1.0
import QtQuick 2.15

ESelect {
    width: 240
    value: "list"
    clearable: true
    textRole: "label"
    valueRole: "value"
    placeholderText: "Choose a view"
    model: [{label: "List", value: "list"}, {label: "Grid", value: "grid"}]

    onChanged: console.log(value)
}
```

`textRole` identifies the displayed label, and `valueRole` identifies the business value. Set `value` to select an option; `clear()` clears selection. An option with `disabled: true` cannot be selected.

`EDatePicker.value` uses `yyyy-MM-dd`. Restrict the range with `minDate` / `maxDate`; `disabledDate(date)` receives a JavaScript `Date`. Use `EDatePickerPanel` for an embedded date panel.

### 4.3 Tables, trees, and long lists

**Purpose:** `ETable` separates row data from column settings; `ETree` displays parent/child nodes; `EListy` creates and reuses delegates on demand for long lists.

**When to use it:** Data records, file hierarchies, navigation trees, and large scrollable collections.

```qml
import EUI 1.0
import QtQuick 2.15

ETable {
    width: 640
    height: 280
    model: [{name: "Alice", score: 92}, {name: "Bob", score: 88}]
    columns: [
        {prop: "name", label: "Name"},
        {prop: "score", label: "Score", width: 120, sortable: true}
    ]

    onRowClicked: console.log(index, row.name)
}
```

`columns[].prop` names a row field, and `label` is its heading. Set a viewport height; row delegates are created with `ListView`. Call `sort("score", "ascending")` or `sort("score", "descending")`; an empty order clears sorting. The column must have `sortable: true`.

`currentRowIndex` and `selectRow(index)` refer to the original model index, including after sorting. `currentRow` returns the selection; `clearSelection()` clears it. Replace the entire `model` array to notify QML about updates. See the Gallery's `ETable` page for editing, filters, tree rows, toolbars, and export settings.

```qml
import EUI 1.0
import QtQuick 2.15

ETree {
    width: 320
    height: 240
    expandedKeys: ["root"]
    model: [{
        id: "root",
        label: "Workspace",
        children: [{id: "docs", label: "Documents"}]
    }]

    onNodeSelected: console.log(key, node.label)
}
```

Give each node a unique string `id` for a stable key. Use `select(key)` and `setExpanded(key, expanded)`, and read `currentKey` / `currentNode`. Disabling a parent disables its descendants.

Choose a list component according to data size and presentation needs:

| Component | Usage |
| --- | --- |
| `EList` / `EListItem` | Display titles, descriptions, avatars, and actions from a JavaScript array, with pagination and a responsive grid; configure `grid` using `columns`, `gutter`, and `itemHeight` |
| `EListy` | Assign a JavaScript array to `items`, set viewport dimensions and `rowKey`, and create/reuse delegates on demand by default for long lists |
| `EInfiniteScroll` | Use with `QAbstractItemModel`, `ListModel`, or incremental loading |

For `EListy`, use items such as `{id: 10, title: "Alpha", category: "A"}` and set `rowKey: "id"`. Group by a field with `group: ({key: "category"})`; `group.key` also accepts an `(item) => key` function. Assign a new `items` array to update the list.

Call `scrollTo(200)` for pixel scrolling, or `scrollTo({key: 10, align: "top", offset: 8})` to position a stable key; `align` supports `top/bottom/auto`. `scrollTo({groupKey: "A", align: "top"})` targets the first item in a group. Invalid requests and missing or duplicate keys return `false`. Read `errorString` if a grouping callback fails.

A custom `itemDelegate` can declare `itemData`, `itemIndex`, `itemKey`, `groupKey`, and `listControl`; the component updates these fields. Disabling `virtual` caches all content and is suited to small datasets.

### 4.4 Uploads and application messages

**Purpose:** `EUpload` provides file selection, queue management, and upload interactions. `EApp` supplies `message`, `modal`, and `notification` managers for a content region.

**When to use it:** Attachments, image uploads, save feedback, or business confirmations.

```qml
import EUI 1.0
import QtQuick 2.15

EUpload {
    limit: 3
    width: 480
    maxFileSize: 5 * 1024 * 1024
    nameFilters: ["Images (*.png *.jpg)"]

    onSelected: console.log("Selected", files.length)
    onFailed: console.log(file.name, message)
    onSucceeded: console.log(file.name, response)
}
```

The default is `autoUpload: false`, so this example only selects local files. To upload, configure your HTTP(S) `action` endpoint and call `upload(index)` or `submit()`, or enable `autoUpload`. Requests use multipart with `fieldName` (default `"file"`) and optional `headers`. `maxFileSize` is in bytes; `limit: 0` means no count limit. `fileList` exposes files and their status. Use `cancel()` to abort the current upload and `removeFile(index)` to remove an entry.

Call `app.message.open("Saved")` to display a message. The complete window in section 3.2 demonstrates showing messages from a button. You can also retrieve the current instance's managers with `app.useApp()`. Place `EApp` inside a window; your application implements persistence and service calls.

### 4.5 Choosing a chart

| Component | Purpose | Use it for |
| --- | --- | --- |
| `EChart` | Native Qt line, bar, and area rendering | Simple numeric series, lower runtime overhead, and software rendering |
| `EECharts` | Local Apache ECharts hosted by Qt WebEngine | ECharts options, additional chart types, legends, zoom, and interaction events |

A native line chart:

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

`EChart.type` supports `Line`, `Bar`, and `Area`. `model` accepts a numeric array or `{label, value}` data-point objects, such as `[{label: "Mon", value: 12}, {label: "Tue", value: 18}]`. `value` is numeric and `label` supplies the horizontal-axis label; omitted labels default to indices starting at 1.

An `EECharts` example:

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

ECharts scripts, HTML, and bridge resources are bundled locally in the plugin; loading the chart engine does not require a CDN. Whether remote data or images require network access depends on your `option`. Options must be JSON-compatible, so JavaScript formatter functions cannot pass through the bridge. Read `ready` / `errorString`, use methods such as `setOption()` and `dispatchAction()`, and handle `clicked`, `legendSelected`, and `dataZoomed`.

For Qt 5 `EECharts`, add this include to the minimal host's `main.cpp`:

```cpp
#include <QtWebEngine/qtwebengineglobal.h>
```

Call this **before** constructing `QGuiApplication`:

```cpp
QtWebEngine::initialize();
```

Add these application CMake dependencies:

```cmake
find_package(Qt5 5.15 REQUIRED COMPONENTS WebEngine WebChannel)
target_link_libraries(MyEuiApp PRIVATE Qt5::WebEngine Qt5::WebChannel)
```

The Gallery already initializes the host; see [example/src/main.cpp](../example/src/main.cpp). With `QT_QUICK_BACKEND=software`, `EECharts` displays an unavailable explanation and does not create a WebEngine chart view. Remove this environment variable when using `EECharts` and verify the target machine's graphics backend. Qt Quick's software scene graph and `QT_OPENGL=software` are different settings.

### 4.6 Layouts, floating buttons, and border effects

**Purpose:** `EFlex` distributes space between children; `EFloatButton` / `EFloatButtonGroup` provide floating actions; `EBorderBeam` draws animated light along a border.

**When to use it:** Toolbars, flexible panes, shortcuts at fixed positions, and cards needing visual emphasis.

| Component | Key settings |
| --- | --- |
| `EFlex` | Set `gap` to a number or `[horizontal, vertical]`; `justify` supports `start/center/end/space-between/space-around/space-evenly`, and `align` supports `start/center/end/stretch` |
| `EFlexItem` | Wrap content that should share remaining space; `grow` / `shrink` control growth and shrinkage, `basis` sets the initial size, and `minSize` / `maxSize` constrain it |
| `EFloatButton` | Position with anchors or x/y; configure icons, text, badges, hints, and disabled state |
| `EFloatButtonGroup` | Use `trigger: "click"` or `"hover"` for an expanding menu, or an empty value for persistent actions; `placement` supports `top/bottom/left/right` |
| `EBorderBeam` | Configure `count`, `size`, `radius`, `duration`, and `color`; `duration` is in seconds, `running: false` pauses, and `reducedMotion: true` stops and hides the decoration |

Ordinary `EFlex` children retain their existing sizes; `EFlexItem` enables growth and shrinkage. `basis: -1` uses the content's implicit size, so provide `implicitWidth/implicitHeight` or set `basis` explicitly. Avoid position anchors on children managed by the layout.

Floating menus can extend beyond their group, and a parent with `clip: true` clips them. Border beams do not intercept input, so their content remains interactive. The Gallery's component pages contain complete interactions and copyable examples.

### 4.7 Call animations in one line

`EAnimation` is a singleton provided by `import EUI 1.0`; no animation object needs to be declared in the page.

```qml
EAnimation.play(card, "fadeIn")
EAnimation.play(input, "shake")
EAnimation.play(card, "bounceIn")
EAnimation.play(card, "flipInY")
EAnimation.play(card, "slideInUp", { duration: 300, distance: 24 })
```

These are separate examples; `card` and `input` are existing visual item IDs. An event handler can use `onClicked: EAnimation.play(card, "fadeIn")`.

| Category | Complete effect names | Behavior |
| --- | --- | --- |
| Fade (18) | `fadeIn`, `fadeOut`, `fadeInUp`, `fadeInDown`, `fadeInLeft`, `fadeInRight`, `fadeOutUp`, `fadeOutDown`, `fadeOutLeft`, `fadeOutRight`, `fadeInTopLeft`, `fadeInTopRight`, `fadeInBottomLeft`, `fadeInBottomRight`, `fadeOutTopLeft`, `fadeOutTopRight`, `fadeOutBottomLeft`, `fadeOutBottomRight` | Directional variants combine opacity with movement, including four diagonal directions. |
| Slide (8) | `slideInUp`, `slideInLeft`, `slideInDown`, `slideInRight`, `slideOutUp`, `slideOutDown`, `slideOutLeft`, `slideOutRight` | Slide from or toward each direction while fading in or out. |
| Scale and zoom (12) | `scaleIn`, `scaleOut`, `zoomIn`, `zoomOut`, `zoomInUp`, `zoomInDown`, `zoomInLeft`, `zoomInRight`, `zoomOutUp`, `zoomOutDown`, `zoomOutLeft`, `zoomOutRight` | Scale enters from or exits toward 85%; zoom uses 30% and can include directional movement. |
| Rotate and roll (13) | `rotateIn`, `rotateOut`, `rotateInUpLeft`, `rotateInUpRight`, `rotateInDownLeft`, `rotateInDownRight`, `rotateOutUpLeft`, `rotateOutUpRight`, `rotateOutDownLeft`, `rotateOutDownRight`, `spin`, `rollIn`, `rollOut` | Rotate around the center or a corresponding corner; roll combines movement and rotation; spin rotates once. |
| Bounce (11) | `bounce`, `bounceIn`, `bounceOut`, `bounceInUp`, `bounceInDown`, `bounceInLeft`, `bounceInRight`, `bounceOutUp`, `bounceOutDown`, `bounceOutLeft`, `bounceOutRight` | Bounce for emphasis or enter/exit with rebound, including directional variants. |
| Flip (5) | `flip`, `flipInX`, `flipInY`, `flipOutX`, `flipOutY` | Flip once around the Y axis, or enter/exit around the specified X/Y axis. |
| Attention and feedback (12) | `shake`, `pulse`, `shakeX`, `shakeY`, `swing`, `tada`, `wobble`, `jello`, `rubberBand`, `flash`, `heartBeat`, `headShake` | Shake horizontally or vertically, pulse, swing, celebrate, wobble, skew elastically, stretch, flash, or beat; shake is the compatibility name for shakeX. |
| Light speed (4) | `lightSpeedInLeft`, `lightSpeedInRight`, `lightSpeedOutLeft`, `lightSpeedOutRight` | Combine horizontal movement and skew with fading in or out. |

`EAnimation.effects` returns all 83 effect names. The optional third argument accepts:

| Option | Default | Values |
| --- | --- | --- |
| `duration` | `300` | A nonnegative integer in milliseconds; `0` completes immediately, maximum `2147483647`. |
| `distance` | `24` | Movement or shake distance in pixels, from `0` to `1000000`; affects directional movement, bounce, and shake. |
| `easing` | `Easing.OutCubic`; attention effects and bounce use `Easing.Linear` | A regular Qt easing enum, such as `Easing.InOutSine`; curves requiring additional control points are unsupported. |

`play()` returns `true` on success. A null target, unknown effect/option, or invalid number returns `false` without interrupting the current animation. Playing again on the same item replaces its animation without accumulating visual transforms; different items can animate simultaneously.

`EAnimation.stop(target)` stops playback, clears animation movement, scale, rotation, flip, and skew, and restores the opacity from before the animation took control. It returns `false` if the target has no animation record. Existing anchors, position, rotation, scale, and property bindings are preserved. Exit effects finish at opacity `0`; entering again restores the original opacity, or uses `1` for an initially transparent item. Attention effects restore the original state when finished.

Animations control visual appearance; application logic manages `visible`, `enabled`, and layout occupancy. Transparent items can still receive input. The Gallery's Visual Effects → Animation page offers eight categories plus an all-effects filter, parameter controls, previews, invocation copying, and complete example source.

## 5. Learn components with the Gallery

The Gallery offers category navigation and name search on the left; `Ctrl+K` focuses search. The upper-right controls change language and theme. Component pages contain interactive examples, source actions, and API documentation below the examples.

![Button source panel showing interactive controls, expanded QML, and a copy-source action](screenshots/source-en.png)

Recommended workflow:

1. Search for a control such as `EInput` and try the interaction in its preview.
2. Expand the relevant source card to inspect its model, properties, and event handlers.
3. Copy the source into an existing window and adjust layout and business text.
4. Read the API section for defaults, signal parameters, and method purposes.
5. Implement and verify your application's models, submission logic, and network requests.

## 6. Deploy your application

### 6.1 Deploy this chapter's MyEuiApp

Complete section 3 and close the application. Run this in the terminal from section 1. Absolute paths make it independent of the working directory:

```powershell
& "$qtKit/bin/windeployqt.exe" --release --qmldir "$appRoot" --qmlimport "$appRoot/build" "$appRoot/build/MyEuiApp.exe"
```

`--qmldir` points to the QML source directory; `--qmlimport` points to the parent directory containing your current `EUI/` module. The deployment tool places Qt libraries, QML runtime modules, and platform plugins in the application directory. Keep the complete `EUI/` directory copied in section 3.3, and distribute all of `$appRoot/build/`. Run `MyEuiApp.exe` on the target computer. Check that the window, button, and messages work on a target without the development environment. For other applications, replace the arguments with their source directories, module parent directories, and executables.

### 6.2 Deploy the Gallery or additional components

To deploy the Gallery from section 2.1, run:

```powershell
& "$qtKit/bin/windeployqt.exe" --release --qmldir "$euiRoot/example/qml" --qmlimport "$euiRoot/bin/Release" "$euiRoot/bin/Release/example.exe"
```

Distribute all of `$euiRoot/bin/Release/`, including its `EUI/` directory and the runtime files generated by the deployment tool.

For `EECharts`, include Qt WebEngine / WebChannel, `QtWebEngineProcess`, Chromium resource files, and `qtwebengine_locales`, retaining your Qt Kit's directory layout. Media controls require Qt Multimedia; SVG icons require the SVG image-format plugin.

### 6.3 Build a Windows installer and portable package

The repository provides [package-windows.ps1](../packaging/package-windows.ps1) for the current Gallery. It targets Windows 10 / 11 x64 using a Qt 5.15 MSVC x64 Kit.

Prepare an x64 Visual Studio Developer PowerShell, CMake, Ninja, and [Inno Setup 6.5 or newer](https://jrsoftware.org/isdl.php). OpenSSL must be a Qt-compatible **1.1.1 x64** runtime containing `libssl-1_1-x64.dll` and `libcrypto-1_1-x64.dll`; OpenSSL 3 cannot replace these DLLs. The [FireDaemon 1.1.1w ZIP](https://download.firedaemon.com/FireDaemon-OpenSSL/openssl-1.1.1w.zip) has the publisher's SHA-256 `1870B15BF6749E65FFBBADF52CDFF3EE0E9F02943550BF4395574BB432AF3EB8`.

From the repository root, replace these tool paths with your local paths:

```powershell
./packaging/package-windows.ps1 `
    -QtDir 'D:/Qt/5.15.2/msvc2019_64' `
    -IsccPath 'C:/Program Files (x86)/Inno Setup 6/ISCC.exe' `
    -OpenSslDir 'D:/OpenSSL/openssl-1.1/x64/bin'
```

`QtDir` is inferred from `qmake.exe` on PATH by default. `IsccPath` is discovered on PATH, in common installation directories, or at `build/packaging-tools/inno/ISCC.exe`. `OpenSslDir` is discovered in the Qt Kit's `bin/`, Qt's `Tools/OpenSSL/Win_x64/bin/`, or `build/packaging-tools/openssl/openssl-1.1/x64/bin/`. If these tools are already configured, run `./packaging/package-windows.ps1` directly.

The script rebuilds the Release Gallery and collects EUI, Qt QML modules, WebEngine resources and subprocess, Multimedia, SVG, MSVC runtime DLLs, and OpenSSL DLLs, with EUI and third-party license notices. The version comes from the root `CMakeLists.txt`; version `1.1.0` produces:

| Output | Usage |
| --- | --- |
| `dist/EUI-Gallery-1.1.0-windows-x64-setup.exe` | Installs per user without administrator privileges; includes a Start menu entry, optional desktop shortcut, and uninstall support |
| `dist/EUI-Gallery-1.1.0-windows-x64-portable.zip` | Extract the complete directory and run `EUI-Gallery.exe` |
| `dist/EUI-Gallery/` | Deployed application directory; regenerated by the next packaging run, so do not store personal files here |
| `dist/SHA256SUMS.txt` | SHA-256 checksums for the installer and portable ZIP |

The target computer does not need Qt or Visual Studio. Keep all DLLs and subdirectories in the portable package; settings such as table layouts retain the application's existing per-user storage behavior. The script does not sign executables. Run `python packaging/check_windows_package.py` to check package contents and startup on an isolated desktop with development tools removed from PATH. Before delivery, test charts, media, uploads, and printing on the actual target computer.

The [Windows workflow](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml) runs the same packaging and verification commands on Windows Server 2022 with MSVC v142, Qt 5.15.2, CMake, Ninja, and Inno Setup. Its startup check sets `QT_OPENGL_DLL=opengl32sw.dll` to use the packaged software OpenGL library without a runner GPU. It retains the installer, portable ZIP, checksums, and `BUILD-INFO.txt` for 14 days. Changing the root `CMakeLists.txt` version and pushing to `master` automatically publishes verified packages and checksums to the matching [Release](https://github.com/lihechuan/EUI/releases) for long-term download. An unchanged version does not publish a new release. See the [release process](../CONTRIBUTING.md#正式发布--releases) for details and failed-job retries.

## 7. Troubleshooting

| Symptom | What to check |
| --- | --- |
| `module "EUI" is not installed` | Does the import path point to the parent of the complete `EUI/` directory? Is `qmldir` present? |
| Plugin loading fails or a DLL is missing | Match Qt version, architecture, compiler, and Debug/Release; deploy plugin dependencies and the Qt runtime |
| Theme or utilities are missing after copying QML | Copy the complete generated module, including the compiled plugin |
| `cl`, `ninja`, or Qt cannot be found | Use Developer PowerShell, check PATH, and correct `CMAKE_PREFIX_PATH` |
| Changing a table/tree array does not refresh the UI | Assign a new `model` array instead of mutating a plain JavaScript array in place |
| Business text does not change with the language | `ELocale` manages built-in text; add your own localization bindings for business `text` / `label` |
| Selected files are not uploaded | Selection does not send files by default; configure `action` and call an upload method or enable `autoUpload` |
| `EECharts` reports an unsupported software backend | Remove `QT_QUICK_BACKEND=software`, then check host initialization, graphics, and WebEngine deployment |
| Brand icons or video do not display | Check the SVG plugin or Multimedia backend and supported media formats |
