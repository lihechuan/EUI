# 组件扩展回归检查

此目录包含组件扩展的功能与交互回归检查。先构建 EUI，再将 `EUI_IMPORT_PATH` 指向构建后 `EUI/` 模块目录的父目录。Qt SDK 必须与已构建的插件版本、编译器和架构一致。

在可使用 C++ 编译器与 CMake/Ninja 的终端执行，例如 Qt 5：

```powershell
cmake -S validation/primevue -B build/primevue-validation-qt5 -G Ninja `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_PREFIX_PATH=D:/Qt/5.15.2/msvc2019_64 `
  -DEUI_IMPORT_PATH=D:/code/EUI/build/qt5-verify/source
cmake --build build/primevue-validation-qt5
ctest --test-dir build/primevue-validation-qt5 --output-on-failure
```

Qt 6 使用相同命令，将 SDK 路径与 `EUI_IMPORT_PATH` 换成对应 Qt 6 构建目录。

- `EUI.Components` 覆盖 26 个公开类型与 149 个展示代码、边界输入、选择/排序、实际鼠标拖动、标签聚焦、视口加载、动态弹窗传参与返回、图片翻转/保存、命令响应以及富文本格式和撤销。
- `EUI.GalleryInteractions` 实际触发弹窗确认/取消回调、菜单命令和排程点击，并循环展开/折叠字段分组；同时检查输入组合在不同宽度下的间距及 0～48px 运行时调整、任务看板的拖动预览和拒绝移动、圆环分段计量的数值与像素、横向和纵向分段条的圆角与窄色块，以及停靠栏四个方向和不同放大倍数的边界。除断言行为外，日志出现 `Binding loop`、`TypeError` 或 `ReferenceError` 也会判定失败。
- `EUI.ImageIO` 覆盖 data URL、本地文件、HTTP 成功、HTTP 失败、保存目标校验及失败时保留原文件。HTTP 使用本地测试服务器。
- 截图与保存测试文件写入 `build/primevue-completion/`。运行前可创建此目录：`New-Item -ItemType Directory build/primevue-completion -Force`。

组件用法见[功能扩展说明](../../docs/primevue-completion.md)。
