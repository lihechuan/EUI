# 组件扩展回归检查

此目录独立于仓库本机忽略的 `tests/` 目录，保存本批组件可版本管理的回归检查。先构建 EUI，再将 `EUI_IMPORT_PATH` 指向构建后 `EUI/` 模块目录的父目录。Qt SDK 必须与已构建的插件版本、编译器和架构一致。

在可使用 C++ 编译器与 CMake/Ninja 的终端执行，例如 Qt 5：

```powershell
cmake -S validation/primevue -B build/primevue-validation-qt5 -G Ninja `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_PREFIX_PATH=D:/application/qt/5.15.2/msvc2019_64 `
  -DEUI_IMPORT_PATH=D:/code/EUI/build/qt5-verify/source
cmake --build build/primevue-validation-qt5
ctest --test-dir build/primevue-validation-qt5 --output-on-failure
```

Qt 6 使用相同命令，将 SDK 路径与 `EUI_IMPORT_PATH` 换成对应 Qt 6 构建目录。

- `EUI.Components` 覆盖 26 个公开类型与 149 个展示代码、边界输入、选择/排序、实际鼠标拖动、标签聚焦、视口加载、动态弹窗传参与返回、图片翻转/保存、命令响应以及富文本格式和撤销。
- `EUI.GalleryInteractions` 实际触发弹窗确认/取消回调、菜单命令和排程点击，并循环展开/折叠字段分组；同时检查输入组合在不同宽度下的间距及 0～48px 运行时调整、任务看板的拖动预览和拒绝移动、圆环分段计量的数值与像素、横向和纵向分段条的圆角与窄色块，以及停靠栏四个方向和不同放大倍数的边界。除断言行为外，日志出现 `Binding loop`、`TypeError` 或 `ReferenceError` 也会判定失败。
- `EUI.ImageIO` 覆盖 data URL、本地文件、HTTP 成功、HTTP 失败、保存目标校验及失败时保留原文件。HTTP 使用本地测试服务器。
- 此 CMake 项目的界面检查使用 `-platform offscreen` 和软件后端，不打开桌面窗口。Qt 5.15.2 在本机 offscreen 插件下不能抓取帧缓冲，组件套件的 6 个渲染检查及交互套件的 5 个分段计量像素检查会明确跳过；Qt 6.8.3 执行这些检查，包含 Compare 叠层像素、列表选中底色及浅色/深色截图；列表样式检查同时验证复选框点击、键盘导航与禁用状态。需要验证 Qt 5 像素时，应在不切换到用户当前桌面的独立 Windows 桌面补跑。
- 本机 Qt 5.15.2 Win64 的默认 JIT 在完整组件连续创建回归中触发 [QTBUG-89513](https://github.com/qt/qtdeclarative/commit/35614462443c100b6753b335b58a134fed4b5c35)。原生栈定位到 `generateFunctionTable()` 写入页边界前的异常处理记录，内存页状态为 `PAGE_EXECUTE_READ`，与上游修复一致。此验证项目仅对 Qt 5.15.2 及更早的 Win64 测试进程设置 `QV4_FORCE_INTERPRETER=1`；沿用既有 Gallery 对 Qt 5.15.2 的兼容措施，未修改 EUI 插件或示例程序的运行环境。Qt 6 完整回归使用默认 JIT。旧 SDK 的默认 JIT 全套回归仍不能通过，应使用包含上游修复的 Qt 构建。
- 截图与保存测试文件写入 `build/primevue-completion/`。运行前可创建此目录：`New-Item -ItemType Directory build/primevue-completion -Force`。

完整 Gallery 的目录、API 和双语说明在本机既有 Gallery 检查套件中验证；组件用法见[功能扩展说明](../../docs/primevue-completion.md)。
