# 全库回归检查

本目录的测试文件可随源码管理，覆盖 `qmldir` 中全部公开 QML 类型、两个可创建的 C++ 类型和 `EAnimation` 单例。QML 组件清单在运行时读取 `qmldir`，新增公开 QML 类型会自动进入检查，避免固定数量过期而漏测；新增 C++ 类型需要同步执行器清单。

本次实测结果、修复项目和逐组件清单见[1.3.0 全库测试报告](../../docs/library-validation-1.3.0.md)。

## 执行

先构建与 Qt SDK 版本、架构一致的 EUI 插件。在启用 `EUI_BUILD_TESTS` 的完整本地构建中执行：

```powershell
ctest --test-dir build/qt6-verify -R '^EUI.LibraryRegression$' --output-on-failure
ctest --test-dir build/qt5-verify -R '^EUI.LibraryRegression$' --output-on-failure
```

需要 Python 3。仓库本机忽略的 `tests/` 目录不存在时，仍会注册本目录的公开类型和组件功能回归；完整本地套件与 Gallery 矩阵需要该目录中的测试目标。

也可以直接运行，不依赖 CMake 注册：

```powershell
python validation/library/run.py `
  --runner build/qt-sdk/6.8.3/msvc2022_64/bin/qmltestrunner.exe `
  --qt-bin build/qt-sdk/6.8.3/msvc2022_64/bin `
  --imports bin/Qt6-Quick3D `
  --gallery-runner bin/Qt6-Quick3D/eui_gallery_tests.exe `
  --output build/library-validation/qt6
```

Qt 5.15.2 Win64 增加 `--qt5-interpreter`，避开该 SDK 已知的 JIT 问题。省略 `--gallery-runner` 时仍执行全部公开类型、边界条件、功能扩展与插槽回归；Gallery 页面矩阵需要带 `exampleCatalog` 等上下文的 `eui_gallery_tests`。SDK 和输出路径可按本机配置修改。

完整本地 CTest 清单可由后台执行器运行，默认串行，适合检查页面打开耗时：

```powershell
python validation/library/run_full.py `
  --build build/qt6-verify `
  --qt-bin build/qt-sdk/6.8.3/msvc2022_64/bin `
  --output build/library-validation/full-qt6
```

执行器保留每个测试指定的渲染后端，将 Windows 窗口测试交给本机 `tests/run_isolated_qml.py` 的隔离桌面，其余界面测试强制后台执行。需要完整本地 `tests/` 测试资料和已构建测试目标；`--only` 可筛选测试名，`--jobs` 可设置并发数。受机器负载影响的耗时检查应单独串行复测。此执行器同样检查日志中的运行错误，避免仅依赖 Qt GUI 程序的标准输出。

## 覆盖范围

- `tst_public_components.qml`：全部公开可创建类型的浅色/深色、中英文、320/640 宽度组合；0、1、320、1280 宽度切换；禁用、显示隐藏和销毁；声明了模型属性的组件检查 `null`、空数组与恢复原模型；`EAnimation` 全部 83 个效果及无效效果、空目标、停止恢复。
- `tst_slot_regressions.qml`：选择器、复选框组、标签输入和数字输入的自定义插槽尺寸；固定标签绑定恢复及销毁；列表点击转发、加载遮罩滚轮拦截；标签头像按需加载与清空；排程日/周/月切换语言；16 个示例在亮暗主题下的 32 项文字对比度检查。
- `tst_gallery_matrix.qml`：全部非演示目录项在浅色/深色、1040/640 页面宽度下加载全部示例，检查尺寸、预览边界、渲染输出；代表页面另外检查连续缩放至 1920/2560；侧栏检查中英文、亮暗主题字体一致性，以及徽章/输入组合页面与菜单之间反复悬停后的文字像素。截图输出到 `build/library-validation/gallery-images/`。
- 复用 `validation/primevue` 中的功能与交互回归，包含选择/排序/拖动、禁用与只读、空值和越界、键盘焦点、滚动加载、弹窗确认/取消、富文本格式/撤销、排程/看板、图片操作等。

执行器强制使用 `offscreen` 与软件后端，不打开前台窗口。它读取 QtTest 日志文件，同时检查进程退出码和 QML 运行错误；断言通过但日志出现绑定循环、类型错误、未定义引用、无效赋值或隐式信号参数警告时仍判定失败。结果包括原始日志、`results.json` 和逐组件的 `component-coverage.csv`。

## 验证边界

创建和通用状态覆盖不等同于穷举每个属性及所有输入组合。具体业务行为由对应功能测试补充。截图覆盖指定环境和状态，不能代替所有 DPI、字体、驱动和操作系统的验证。

软件后端不支持部分 ShaderEffect 和 WebEngine 图表渲染。完整本地套件另在隔离 Windows 桌面测试图表、主题过渡、视觉效果、动画、原生文件/目录选择器，期间不切换用户桌面。Qt 5 offscreen 无法抓取帧缓冲的 6 项检查明确跳过，由 Qt 6 执行。macOS 权限/链接以及窗口管理器接管的原生缩放拖动需要目标环境验证。

Gallery 中故意不存在的图片用于验证失败回退，其加载警告是预期行为。真实网络、音视频设备、所有驱动和长时间运行仍需各使用环境补充验证。
