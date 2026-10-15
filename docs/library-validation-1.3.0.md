# EUI 1.3.0 全库测试与修复报告

实测日期：2026-10-15。环境：Windows x64、MSVC、Qt 5.15.2 与 Qt 6.8.3，版本号保持 `1.3.0`。

本次覆盖全部 156 个公开 QML 类型、2 个可创建 C++ 类型和 `EAnimation` 单例，共 159 项；全部 152 个组件目录页执行双主题、双页面宽度矩阵。两套 Qt 环境的完整 81 组本地测试均通过，运行错误检查同时通过。

测试使用 offscreen，或通过 `CreateDesktopW` 创建不切换到用户桌面的独立 Windows 桌面。原生文件选择器、图表及需要 Windows 渲染后端的测试在独立桌面执行；没有在用户当前桌面打开测试程序。日志中的前台句柄前后快照也会保留，但用户自身切换窗口会使该快照变化，因此不将它单独解释为测试抢占焦点的证据。

## 测试结果

| 环境 | 完整测试组 | 失败组 | QtTest 通过记录 | QtTest 跳过记录 |
| --- | ---: | ---: | ---: | ---: |
| Qt 5.15.2 Win64，解释器 | 81 / 81 | 0 | 4752 | 16 |
| Qt 6.8.3 Win64，默认 JIT | 81 / 81 | 0 | 4786 | 4 |

统计包含各套件的初始化/清理记录、复用套件和环境重复执行，不能解释为相同数量的独立功能。Python/静态检查以测试组通过计入，不虚构 QtTest 记录。新增 `EUI.LibraryRegression` 内部结果如下：

| 套件 | Qt 5 通过 / 失败 / 跳过 | Qt 6 通过 / 失败 / 跳过 |
| --- | --- | --- |
| `tst_public_components.qml` | 756 / 0 / 0 | 756 / 0 / 0 |
| `tst_slot_regressions.qml` | 57 / 0 / 0 | 57 / 0 / 0 |
| `tst_primevue_components.qml` | 189 / 0 / 6 | 195 / 0 / 0 |
| `tst_gallery_interactions.qml` | 21 / 0 / 0 | 21 / 0 / 0 |
| `tst_gallery_matrix.qml` | 624 / 0 / 0 | 624 / 0 / 0 |

独立的 `validation/primevue` CMake 项目中，组件功能、Gallery 交互、图片 IO 三组测试也均通过。Qt 5 与 Qt 6 的插件、示例及本地测试目标均构建成功。文档校验覆盖 152 个目录入口和 3466 条属性记录。

## 实际修复

| 问题 | 修复与回归 |
| --- | --- |
| `ESelect`、`ECheckBoxGroup`、`EInputTag`、`ESpinBox` 自定义插槽在 Qt 6 形成尺寸绑定循环 | 使用已有的内容尺寸中间属性方式，避免 Loader 与已加载项相互依赖；16 个主题/宽度状态回归。 |
| `EIftaLabel` 与内部输入框一起销毁时还原高度绑定，导致父对象已释放的类型错误 | 区分内部目标销毁与外部目标解绑，保证外部目标恢复原来的高度和内边距；测试切换 fixed、销毁和展示代码。 |
| `ETaskBoard.columns = null` 时直接读取长度 | 为隐式宽度计算增加数组校验；空值、空数组及恢复检查。 |
| `EAvatarGroup.avatars = null` 时读取长度报错 | 非数组按 0 计数，同步 API 属性表达式；覆盖空值恢复及既有头像组功能。 |
| 表格表头随窗口缩放或销毁时，几何绑定访问空 owner | 为坐标和尺寸绑定补齐生命周期保护；复测表格功能与完整 Gallery 创建/销毁。 |
| 菜单切换布局、折叠或销毁过程中弹窗尚未具有可用父窗口 | 为弹窗设置明确父对象，延迟到对象树稳定后同步打开状态；复测菜单、键盘和页面切换。 |
| Qt 6 对隐式注入信号参数发出兼容性警告 | 鼠标、键盘、拖动、日历、图表、列表点击与加载遮罩滚轮等处理器改用显式参数；保留信号转发行为。 |
| `EScheduler` 星期标签取系统语言，切换 EUI 语言后不一致 | 日/周/月标签按 `ELocale.language` 使用明确 locale；验证 en→zh→en 动态切换。 |
| `ETag` 在无头像时仍创建头像对象，拖慢包含大量标签的页面 | 按头像来源按需加载、清空时释放；无头像、图标、有效头像及页面打开耗时检查通过。 |
| 深色主题中延迟加载、滚动动画、涟漪及右键菜单演示固定浅底，文字对比不足 | 16 个演示使用主题背景；32 个亮暗主题文字对比度检查达到 4.5:1。 |
| 截图在画布或虚拟列表完成绘制前保存 | 在滚回顶部、完成布局后等待绘制，重新生成 608 个页面状态截图并复查。 |
| 测试仅依据进程退出码，会漏掉“断言成功但 QML 仍报错”的情形 | 读取 QtTest 实际日志，同时检查 TypeError、ReferenceError、SyntaxError、RangeError、绑定循环、无效赋值和隐式信号参数；命中则整组失败。 |

此前用户截图中的 `EFieldset` 高度循环和示例调用 `EMessage.info()` 错误，也纳入本轮功能、展示代码及页面回归验证。

## 覆盖方式

- 158 个可创建类型分别检查浅色中文 640、深色英文 320、浅色英文 320、深色中文 640 四个状态；每次切换 0、1、320、1280 及原宽度，检查有限尺寸、禁用/恢复、显示/隐藏与销毁。
- 对模型、项目、事件、任务、列、头像、步骤及多值数组属性检查 null、空数组、恢复原值；有意拒绝输入的组件允许保留错误状态，不允许产生 QML 运行错误。
- `EAnimation` 全部 83 个效果检查调用、停止及透明度恢复，并覆盖无效效果与空目标；既有动画渲染套件另外检查像素与变换行为。
- 功能套件覆盖选择/排序/拖动、禁用/只读、键盘焦点、滚动加载、弹窗确认/取消/返回值、富文本格式/撤销、排程/看板、图片翻转和保存、表格编辑/导入导出、原生选择器等。
- 152 个目录页在浅色中文/深色英文、1040/640 页面宽度下加载全部示例，检查预览边界并保存截图，共 608 个页面状态；代表页面连续缩放至 1920/2560。截图为自动渲染与人工复查依据，不等同于所有示例所有交互状态的视觉穷举。

## 逐组件清单

“创建状态”对应上述四种状态；“页面状态”对应双主题、双页面宽度。无独立目录页的容器/基础类型仍执行全部创建和生命周期检查，并由组合示例使用。EAnimation 使用单例效果检查替代创建检查。“空值/恢复”是本次模型通用测试的属性名，其他边界行为由功能套件补充。

| 公开类型 | Qt 5 创建/效果状态 | Qt 6 创建/效果状态 | 页面状态 Qt 5 / Qt 6 | 空值/恢复属性 |
| --- | ---: | ---: | --- | --- |
| `EAcrylic` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAdvancedRectangle` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAffix` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAlert` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAnchor` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EAnimateOnScroll` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAnimation` | 83 / 83 | 83 / 83 | 4 / 4 | — |
| `EApp` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAppBar` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EApplicationWindow` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `EAside` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `EAutocomplete` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EAvatar` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EAvatarGroup` | 4 / 4 | 4 / 4 | 4 / 4 | avatars |
| `EBacktop` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EBadge` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EBorderBeam` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EBreadcrumb` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EButton` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECalendar` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECaptcha` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECard` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECarousel` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ECascader` | 4 / 4 | 4 / 4 | 4 / 4 | options |
| `EChart` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECheckBox` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECheckBoxButton` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECheckBoxGroup` | 4 / 4 | 4 / 4 | 4 / 4 | options |
| `EChip` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECircleProgress` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EClip` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECodeView` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECol` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `ECollapse` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EColorPicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EColorPickerPanel` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECommandPalette` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ECompare` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EComponentStateObject` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `EConfigProvider` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EContainer` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECopyableText` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ECountdown` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDatePicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDatePickerPanel` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDateTimePicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDeferred` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDescriptions` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EDialog` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDialogService` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDivider` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDock` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EDrawer` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EDropdown` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EDynamicDialog` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EECharts` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EEmpty` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFieldset` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFilePicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFlex` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFlexItem` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFloatButton` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFloatButtonGroup` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFloatLabel` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFocusTrap` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFolderPicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EFooter` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `EForm` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EFormItem` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EHeader` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `EIcon` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EIftaLabel` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EImage` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EImageViewer` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EInfiniteScroll` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EInplace` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EInput` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EInputGroup` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EInputOtp` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EInputTag` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EKnob` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ELabel` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ELink` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EList` | 4 / 4 | 4 / 4 | 4 / 4 | items |
| `EListItem` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EListbox` | 4 / 4 | 4 / 4 | 4 / 4 | model, values |
| `EListy` | 4 / 4 | 4 / 4 | 4 / 4 | items |
| `ELoading` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EMain` | 4 / 4 | 4 / 4 | 无独立目录页 | — |
| `EMediaPlayer` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EMegaMenu` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EMention` | 4 / 4 | 4 / 4 | 4 / 4 | options |
| `EMenu` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EMenuBar` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EMessage` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EMessageBox` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EMeterGroup` | 4 / 4 | 4 / 4 | 4 / 4 | values |
| `ENotification` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EOrderList` | 4 / 4 | 4 / 4 | 4 / 4 | model, values |
| `EOrgChart` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EPageHeader` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EPagination` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EParallaxView` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EPopconfirm` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EPopover` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EProgress` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EQRCode` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERadio` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERadioButton` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERadioGroup` | 4 / 4 | 4 / 4 | 4 / 4 | options |
| `ERangeSlider` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERate` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERectangle` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EResult` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERipple` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERotatingIcon` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ERow` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EScheduler` | 4 / 4 | 4 / 4 | 4 / 4 | events |
| `EScrollbar` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESegmented` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ESelect` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EShadow` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EShortcutPicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESidebar` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ESkeleton` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESkeletonItem` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESlider` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESpace` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESpinBox` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESplitter` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESplitterPanel` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EStaggeredLayout` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EStatistic` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EStatusView` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ESteps` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ESwitch` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETable` | 4 / 4 | 4 / 4 | 4 / 4 | model, columns |
| `ETabs` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ETag` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETaskBoard` | 4 / 4 | 4 / 4 | 4 / 4 | tasks, columns |
| `ETerminal` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EText` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETextArea` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETextButton` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETextEditor` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EThemeSwitch` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETimePicker` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETimeSelect` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETimeline` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EToolBar` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `EToolTip` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `ETour` | 4 / 4 | 4 / 4 | 4 / 4 | steps |
| `ETransfer` | 4 / 4 | 4 / 4 | 4 / 4 | data |
| `ETree` | 4 / 4 | 4 / 4 | 4 / 4 | model |
| `ETreeSelect` | 4 / 4 | 4 / 4 | 4 / 4 | data |
| `ETypography` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EUpload` | 4 / 4 | 4 / 4 | 4 / 4 | data |
| `EWatermark` | 4 / 4 | 4 / 4 | 4 / 4 | — |
| `EWizard` | 4 / 4 | 4 / 4 | 4 / 4 | model |

## 跳过项和未验证范围

| 项目 | 当前结论 |
| --- | --- |
| Qt 5 offscreen 的 6 个帧缓冲/像素检查 | 明确跳过，Qt 6 执行通过；Qt 5 另有独立 Windows 桌面的原生渲染检查。 |
| 两个 offscreen 主题过渡 ShaderEffect 检查 | 两个 Qt 版本均跳过；独立 Windows 桌面的 `EUI.theme_rendering` 5 项检查分别通过。 |
| macOS app bundle 权限/链接 | Windows 环境各跳过 1 项，需 macOS 实机验证。 |
| 原生窗口管理器接管的标题栏缩放手势 | 每个 Qt 版本跳过 1 项；其他标题栏与窗口动作检查通过。 |
| Qt 5.15.2 Win64 默认 JIT | 本机旧 SDK 使用已有解释器兼容措施；本报告不将其默认 JIT 全套运行列为通过。Qt 6 使用默认 JIT。 |
| 预期警告 | 故意不存在的图片用于失败回退；Qt 5 offscreen 不支持动态 OpenGL；裸 qmltestrunner 的 QSettings 缺少组织名；Qt 5 原生文件选择器初始化的 Model size 警告。均保留日志，后者 8 项原生选择器断言与实际返回结果通过；未宣称全部日志零警告。 |
| 平台与长期行为 | 其他 OS/架构、全部 DPI/字体/驱动组合、真实音视频设备/编解码器、所有网络故障、长时间压力及全部用户输入组合未穷举。 |

本次已消除可复现的组件运行错误和列出的样式问题，但这些结果不能作为“任何环境、任何输入均无问题”的保证。

## 重复执行与证据

执行方式见 [validation/library/README.md](../validation/library/README.md)。主项目启用 `EUI_BUILD_TESTS` 后注册 `EUI.LibraryRegression`；即使没有本机忽略的 `tests/` 目录，公开类型和功能套件仍可执行。

- 完整本地清单与逐组日志：`build/library-validation/release-qt5/summary.json`、`release-qt6/summary.json` 及同目录 `EUI.*.txt`。
- 五组新增全库回归：`build/qt5-verify/library-validation/results.json`、`build/qt6-verify/library-validation/results.json`。
- 逐组件 CSV：上述两个目录中的 `component-coverage.csv`。
- 页面截图：`build/library-validation/gallery-images/`，文件名为 `light|dark-1040|640-组件名.png`。
- 所有测试与报告均在当前分支进行；保留原有工作区修改，未提交或推送。原始日志与截图位于本机构建输出目录，逐组件清单与回归源码位于仓库可管理路径。
