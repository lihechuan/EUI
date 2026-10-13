# EUI

A UI component library for Qt Quick / QML desktop applications.

[简体中文](README.md) | English

## Start here

| Your goal | Where to start | What you will get |
| --- | --- | --- |
| Try the components | “Run the component examples” below | A Gallery with interactive examples and copyable source |
| Use EUI in your application | [Complete integration example](docs/guide.en.md#3-integrate-a-qt-quick-application) | An application whose button updates a counter and shows a message |
| Use a particular control | [Common component examples](docs/guide.en.md#4-common-features-purpose-and-usage) | Forms, selectors, tables, trees, uploads, charts, and layouts |
| Run on another computer | [Deploy your application](docs/guide.en.md#6-deploy-your-application) | An application directory containing EUI and its Qt runtime |

![English EUI overview with navigation, search, interactive previews, and component categories](docs/screenshots/overview-en.png)

## Run the component examples

### 1. Prepare your environment

Use **Windows, the Qt 5.15.2 MSVC 2019 64-bit Kit, CMake 3.20 or newer, and Ninja**. Open **Developer PowerShell for VS 2019** with an x64 compilation environment. See [prerequisites](docs/guide.en.md#1-prerequisites) for the Qt module list.

### 2. Build the Gallery

Change the two paths below to your repository and Qt Kit directories, then execute the commands in the same PowerShell window:

```powershell
$euiRoot = 'D:/code/EUI'
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH
Set-Location $euiRoot

cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON -DEUI_BUILD_TESTS=OFF
cmake --build build/local
```

The application is generated at `$euiRoot/bin/Release/example.exe`, with the component module in the adjacent `EUI/` directory. Building the Gallery also copies the module to `$qtKit/qml/EUI/`; this directory must be writable.

### 3. Run and try a component

From the repository root, run:

```powershell
& ./bin/Release/example.exe
```

After the Gallery opens:

1. Search for `EButton` on the left and open its page.
2. Click the example buttons to try their states and interactions.
3. Select **Show source**, then **Copy source**.
4. Read the properties, signals, and methods below the examples.
5. Use the language button and theme switch in the upper-right corner.

## Use the components in your application

Follow the [complete integration example](docs/guide.en.md#3-integrate-a-qt-quick-application) in order:

1. Build and install EUI to obtain the complete module directory.
2. Create `MyEuiApp` with `CMakeLists.txt`, `main.cpp`, `main.qml`, and `resources.qrc`.
3. Build the application and copy the complete `EUI/` directory beside its executable.
4. Run it and select **Add one**. The counter should increase and a message should appear.

The guide provides every file, build and copy commands, and the resulting directory structure in one chapter. For an existing Qt Quick application, see [integrating an existing project](docs/guide.en.md#35-use-an-existing-project-or-replace-the-example-control).

## Choose a component

| Need | Components | Configure first |
| --- | --- | --- |
| Buttons, inputs, and selection | `EButton`, `EInput`, `ESelect` | Display text, input values, option models, and event handlers |
| Form validation | `EForm`, `EFormItem` | `model`, `rules`, and each field's `prop` |
| Tables and trees | `ETable`, `ETree` | Rows and columns, or nodes with unique `id` values |
| Lists | `EList`, `EListy` | Data arrays, viewport dimensions, and stable item keys |
| Messages and dialogs | `EApp` | Place content inside it and use `message`, `modal`, or `notification` |
| Charts | `EChart`, `EECharts` | Numeric `model` or ECharts `option`; the latter needs WebEngine |
| Layouts and decoration | `EFlex`, `EFlexItem`, `EBorderBeam` | Spacing and space allocation, or beam dimensions and duration |
| Themes and language | `ETheme`, `ELocale`, `EConfigProvider` | `dark`, `language`, or scoped configuration |

See the [English usage guide](docs/guide.en.md) for examples and [troubleshooting](docs/guide.en.md#7-troubleshooting) for loading or deployment problems.

## Feature screenshots

### Editable tables

Use `ETable` for data workspaces. The Gallery demonstrates editing, validation, sorting, multi-selection, pagination, column configuration, import/export, and printing. Configure columns and business data in your own application.

![English ETable example with editing tools, data rows, selection, column settings, and pagination](docs/screenshots/table-en.png)

### Forms and validation

`EForm` manages data and rules; `EFormItem` connects a field with its label and validation feedback. Handle `submitted(valid, model)` to perform your application's submission logic.

![English EForm example with amount input, submission validation, required-name feedback, and email input](docs/screenshots/form-en.png)

### Dark theme

Built-in controls use shared palette tokens. Change `ETheme.dark` or use `EThemeSwitch` to switch appearance.

![Dark English Gallery overview with navigation, inputs, a table, and a theme control](docs/screenshots/overview-dark-en.png)

### Icons and native charts

The icon page supports search and copying enum references or QML snippets. `EChart` draws simple line, bar, and area charts from numeric models. Choose `EECharts` for ECharts options and interactions.

![EIcon browser with search, icon tiles, and copy actions](docs/screenshots/icons-en.png)

![Native EChart examples with numeric data and several chart styles](docs/screenshots/chart-en.png)

The chart screenshot uses `EChart`. `EECharts` requires host WebEngine initialization and a supported graphics backend; see [choosing a chart](docs/guide.en.md#45-choosing-a-chart).
