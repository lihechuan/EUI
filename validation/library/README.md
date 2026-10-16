# 全库回归检查

本目录的测试文件可随源码管理，覆盖 `qmldir` 中全部公开 QML 类型、两个可创建的 C++ 类型和 `EAnimation` 单例。QML 组件清单在运行时读取 `qmldir`，新增公开 QML 类型会自动进入检查，避免固定数量过期而漏测；新增 C++ 类型需要同步执行器清单。

## 执行

先构建与 Qt SDK 版本、架构一致的 EUI 插件。在启用 `EUI_BUILD_TESTS` 的完整本地构建中执行：

```powershell
ctest --test-dir build/qt6-verify -R '^EUI.LibraryRegression$' --output-on-failure
ctest --test-dir build/qt5-verify -R '^EUI.LibraryRegression$' --output-on-failure
```

需要 Python 3。

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

使用 `run_full.py` 运行 CTest 清单：

```powershell
python validation/library/run_full.py `
  --build build/qt6-verify `
  --qt-bin build/qt-sdk/6.8.3/msvc2022_64/bin `
  --output build/library-validation/full-qt6
```

`--only` 筛选测试名，`--jobs` 设置并发数。

## 覆盖范围

- `tst_public_components.qml`：全部公开可创建类型的浅色/深色、中英文、320/640 宽度组合；0、1、320、1280 宽度切换；禁用、显示隐藏和销毁；声明了模型属性的组件检查 `null`、空数组与恢复原模型；`EAnimation` 全部 83 个效果及无效效果、空目标、停止恢复。
- `tst_slot_regressions.qml`：选择器、复选框组、标签输入和数字输入的自定义插槽尺寸；固定标签绑定恢复及销毁；列表点击转发、加载遮罩滚轮拦截；标签头像按需加载与清空；排程日/周/月切换语言；16 个示例在亮暗主题下的 32 项文字对比度检查。
- `tst_gallery_matrix.qml`：全部非演示目录项在浅色/深色、1040/640 页面宽度下加载全部示例，检查尺寸、预览边界、渲染输出；代表页面另外检查连续缩放至 1920/2560；侧栏检查中英文、亮暗主题字体一致性，以及徽章/输入组合页面与菜单之间反复悬停后的文字像素。截图输出到 `build/library-validation/gallery-images/`。
- 复用 `validation/primevue` 中的功能与交互回归，包含选择/排序/拖动、禁用与只读、空值和越界、键盘焦点、滚动加载、弹窗确认/取消、富文本格式/撤销、排程/看板、图片操作等。

结果包括 QtTest 日志、`results.json` 和逐组件的 `component-coverage.csv`。运行器同时检查进程退出码和日志中的 QML 错误。
