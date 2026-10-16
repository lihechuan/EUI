# Littlest Tokyo：Qt Quick 3D 关键帧动画

用 QML / Qt Quick 3D 实现 [Three.js animation / keyframes 示例](https://threejs.org.cn/examples/#webgl_animation_keyframes)。使用同一个 Littlest Tokyo 模型，保留建筑、招财猫、电车、贴图、骨骼和原始 10 秒关键帧动画。

## 在 EUI Gallery 中运行

入口：**视觉效果 → 3D 动画**，左侧带“仅展示”标识，悬停时说明它不是 EUI 组件；也可以搜索 `Tokyo` 或 `3D`。此 3D 动画仅用于展示效果，并不是 EUI 组件。模型、纹理和动画已嵌入程序资源，不需要联网。

点击示例下方的“显示源代码”后，左侧列出相关文件，右侧显示文件路径和带行号的代码。默认打开 `TokyoScene.qml`，可切换到 `TokyoView.qml`、组件调用示例 `Usage.qml` 和完整的 `assets/LittlestTokyo.qml`；切换后代码滚动位置归零，“复制源码”复制当前文件。每个实现文件从打包时嵌入的实际源码读取。该场景用 Qt Quick 3D 原生渲染，没有 WebEngineView、网页嵌入或 Three.js JavaScript 运行依赖；模型来自同一份 Three.js 示例资产，已转换为 Qt 的网格、材质和关键帧。

使用 Qt 6.8 或更高版本并安装 Qt Quick 3D、Qt Quick Timeline 时，CMake 自动启用三维场景。Qt 5、较早的 Qt 6、缺少模块或软件渲染模式下，示例入口保留并显示运行条件。CMake 配置输出会显示是否启用 `Quick 3D example`。

Windows x64 下，Qt 5 Gallery 的该页面首次提供“安装并打开 3D 示例”按钮，显示文件安装、校验和启动进度。安装结果保存在当前用户目录，重启后按钮变为“打开 3D 示例”；同一版本的文件没有变化时直接启动已安装的 Qt 6 Gallery，不重复安装或全量读取运行库。准备好后自动切换，保持语言和主题并直接打开此页面；确认三维视口开始渲染后，Qt 5 Gallery 才退出。安装或启动失败时保留原窗口，可以重试。

Windows 版 Gallery 的 3D 示例需要 64 位 Windows，无需另外安装 Qt SDK。

自行构建时，用 Qt 6.8+ SDK 配置项目，并为该构建设置独立的输出目录，避免覆盖现有 Qt 5 程序：

```powershell
cmake -S . -B build/quick3d-gallery -G Ninja -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_PREFIX_PATH=C:/Qt/6.8.3/msvc2022_64 `
  -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=D:/code/EUI/bin/Qt6-Quick3D
cmake --build build/quick3d-gallery --target example
```

以上命令在已配置 MSVC 和 Ninja 的终端中执行，SDK、仓库和输出目录按实际位置调整。

## 独立运行

需要 Qt 6.8 或更高版本，安装 Qt Quick 3D、Qt Quick Timeline 和 Qt Quick Controls，并先构建当前项目的 Qt 6 EUI 模块。示例可独立打开窗口；界面使用 `EButton`、`ESelect`、`EText` 和 `ERectangle`，与 Gallery 共用 EUI 主题。资源已转换并随示例提供，正常运行不需要联网、Node.js 或资产导入工具。

本仓库已有 Qt 6.8.3 SDK 时，在仓库根目录执行：

```powershell
.\example\quick3d\littlest-tokyo\run.ps1
```

使用其他 Windows Qt SDK：

```powershell
.\example\quick3d\littlest-tokyo\run.ps1 -QtBin 'C:\Qt\6.8.3\msvc2022_64\bin'
```

其他平台可使用同版本 Qt 的 QML Runtime，或直接将 `TokyoScene.qml` 嵌入自己的 Qt Quick 界面：

```sh
/path/to/Qt/bin/qml -I /path/to/compiled/EUI-parent Main.qml
```

最后一条命令在本示例目录执行。三维渲染需要可用的图形后端，不能使用 `QT_QUICK_BACKEND=software`；Windows 启动脚本会为本次运行清除该设置，并选用 Basic 控件样式。

`-I` 指定已编译 Qt 6 EUI 模块的父目录；Windows 脚本会自动查找，也可使用 `-EuiImportDir` 显式指定。

## 操作

- 鼠标拖拽：绕模型旋转；滚轮或触控板：缩放。
- 暂停 / 播放：冻结或恢复当前关键帧。
- 重新播放：回到动画起点，保留当前播放或暂停状态。
- 重置视角：恢复初始相机角度和距离。
- 0.5× / 1× / 2×：立即改变播放速度。
- 底部显示动画时间与当前视口 FPS；暂停且无交互时不持续重绘，FPS 可能降为 0。

## 实现

- `Main.qml`：独立运行窗口。
- `TokyoView.qml`：Gallery 和独立窗口共用的视图，包含标题、自动换行的控制栏、语言切换和模型署名。
- `TokyoScene.qml`：三维视口、相机、灯光、轨道控制，以及 `FrameAnimation` 驱动的动画时间轴。
- `assets/LittlestTokyo.qml`：Balsam 导入的原模型节点、材质、蒙皮和关键帧分组；公开 `timeline` 供场景控制。
- `assets/meshes/`、`assets/maps/`、`assets/animations/`：Qt 原生网格、贴图和二进制关键帧。
- `assets/maps/studio.hdr`：本示例生成的柔光环境贴图。

模型自身的关键帧和蒙皮由 Qt Quick Timeline / Qt Quick 3D 处理。`FrameAnimation` 只按实际帧间隔推进时间，便于暂停、重播和即时切换倍速。照明使用柔光环境与方向光，构图参考原示例；由于渲染器和光照设置不同，颜色与 Three.js 画面会有差异。

## 资源来源

原模型作者为 Glen Fox（glenatron），模型采用 CC BY 4.0；详细署名、来源、转换方式与校验值见 [assets/ATTRIBUTION.md](assets/ATTRIBUTION.md)。本示例的源码许可遵循仓库 LICENSE，第三方模型保留原许可。
