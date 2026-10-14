# EUI

English | [简体中文](docs/README.zh-CN.md)

[![Windows build and package](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml/badge.svg)](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml)
[![MIT License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

EUI is a UI component library for Qt Quick / QML desktop applications. It provides controls for forms, data tables, navigation, charts, and application feedback, with shared light and dark themes.

The **EUI Gallery** brings together **126 components** with interactive examples, copyable QML source, and API references. Use it to explore a control's behavior before adding it to your application.

Current source version: **1.2.0**. Adds file/folder pickers, a toolbar, code view, command palette, and wizard. The Gallery marks components introduced in this version. Windows packages include a Qt 6 runtime for the Littlest Tokyo 3D example, and the Gallery supports verified updates.

![EUI Gallery: component navigation, search, and interactive previews](docs/screenshots/overview-en.png)

## Components

| Area | Components | Capabilities |
| --- | --- | --- |
| Everyday controls | `EButton`, `EInput`, `ESelect` | Buttons, text entry, selection, sizes, and states |
| Forms | `EForm`, `EFormItem`, `EDatePicker`, `ETimePicker` | Field validation, error feedback, and date/time input |
| Data views | `ETable`, `ETree`, `EList`, `EListy` | Editable tables with sorting, filtering, and pagination; trees and virtual lists |
| Navigation and layout | `EMenu`, `ETabs`, `EBreadcrumb`, `EFlex` | Menus, page navigation, and flexible layouts |
| Messages and dialogs | `EApp` | Application-scoped messages, notifications, and modal dialogs |
| Charts and motion | `EChart`, `EECharts`, `EAnimation` | Native charts, Apache ECharts integration through WebEngine, and 83 animation presets |
| Appearance and language | `ETheme`, `ELocale`, `EConfigProvider` | Light/dark themes, English/Chinese text, and scoped configuration |

See [table, form, chart, and theme screenshots](docs/screenshots/README.md) for more examples of the interface.

## Run the Gallery

The build steps below use **Windows x64, Qt 5.15.2 MSVC 2019, CMake 3.20+, and Ninja**. Install the [required Qt modules](docs/guide.en.md#1-prerequisites), then open an x64 **Developer PowerShell for VS 2019** in the repository root.

Replace `$qtKit` with your Qt Kit directory:

```powershell
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH

cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release `
    "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON `
    -DEUI_BUILD_TESTS=OFF -DQML_PLUGIN_DUMP:FILEPATH=OFF
cmake --build build/local --target example --parallel
& ./bin/Release/example.exe
```

The build places the executable in `bin/Release/` and copies the EUI module beside it and into `$qtKit/qml/EUI/`. The Qt import directory must be writable.

In the Gallery:

1. Search for a component, such as `ETable`, or browse a category.
2. Try its examples, including editable values and component states.
3. Select **Show source** or **Copy source**, then check the API below the examples.

Use the language and theme controls to compare English/Chinese text and light/dark appearance.

## Use EUI in your application

Build and install the library with the same Qt Kit as your application, then add the complete `EUI/` module directory to the application's QML import path. The [integration guide](docs/guide.en.md#3-integrate-a-qt-quick-application) includes the library build, CMake project, C++ entry point, and module-copy commands.

This `main.qml` creates a window with a counter and a message. Click **Add one** to update the bound text and display feedback through `EApp`:

```qml
import EUI 1.0
import QtQuick 2.15

EApplicationWindow {
    id: window

    property int clicks: 0

    width: 480
    height: 320
    visible: true
    title: "EUI Demo"

    EApp {
        id: app

        anchors.fill: parent

        Column {
            x: 24
            y: 24
            spacing: 16

            EText {
                text: "Clicks: " + window.clicks
            }
            EButton {
                text: "Add one"
                styleType: EStyle.Primary

                onClicked: {
                    window.clicks += 1
                    app.message.open("Count: " + window.clicks)
                }
            }
        }
    }
}
```

Configure form models, table columns, and other controls using the [component examples](docs/guide.en.md#4-common-features-purpose-and-usage). Follow the [deployment steps](docs/guide.en.md#6-deploy-your-application) when distributing your application.

## Contributing and support

Report a bug or request a feature through [GitHub Issues](https://github.com/lihechuan/EUI/issues), or contact [xiaojiaqaq@gmail.com](mailto:xiaojiaqaq@gmail.com). Include the component name, Qt version, operating system, reproduction steps, and relevant error messages.

See [CONTRIBUTING.md](CONTRIBUTING.md) for contribution and build checks, and [CHANGELOG.md](CHANGELOG.md) for release history.

## License

EUI's own code and documentation use the [MIT License](LICENSE). Third-party software and assets retain their own licenses; see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
