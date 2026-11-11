# 更新记录 / Changelog

记录各版本的功能、修复和兼容性变化。

Features, fixes, and compatibility changes for each version.

## 1.3.3 - 2026-11-11

1. 保留 `ESteps.active` 的有效绑定，使 Wizard 在前进、后退、完成、重置和替换模型后同步步骤条。
2. 修复 Anchor 清空滚动目标后保留旧选中项及访问空对象的问题。
3. 修复 Card 和 Statistic 移除内容槽后保留旧尺寸，以及重新启用插槽时的 Loader 绑定循环。
4. 修复 DynamicDialog 零时长退出时先回传拒绝结果的问题，并保护延迟完成回调中的对象生命周期。
5. 修复 CommandPalette 连续筛选后立即缩放时使用旧列表高度的问题。
6. 修复 Image 切换错误占位内容时的 Loader 绑定循环，并显式声明预览键盘和滚轮事件参数，消除 Qt 6 参数注入警告。
7. 修复 Collapse 和 Timeline 移除内容插槽后残留旧高度及恢复时的 Loader 绑定循环。
8. 修复 InfiniteScroll 到达加载距离边界时未触发加载的问题。
9. 修复 Terminal 异步处理器重复回调时覆盖首次结果并重复发送完成通知，以及输出转换失败后一直处于忙碌状态的问题。
10. 修复高 DPI 软件渲染下背景采样与圆角裁剪使用错误区域，导致内容被放大或偏移的问题。
11. 修复 Calendar 范围比较包含时分秒时排除首日或末日的问题，统一按日判断范围边界。
12. 修复 Ripple 在零缩放首帧仍绘制孤立像素的问题。
13. 修复 BorderBeam 在小数尺寸或高 DPI 下反复绘制时边缘颜色残留、逐渐加深的问题。
14. 修复 Deferred 和 AnimateOnScroll 初始位于显式视口外时，随祖先移动进入视口后仍不更新，以及同级视口或其祖先移动后可见状态未更新的问题。
15. 修复 DynamicDialog 保留内容后切换组件时使用新参数重复创建旧组件的问题。

English: Keeps DynamicDialog component and parameter updates together so switching retained content does not recreate the old component with new parameters. Makes Deferred and AnimateOnScroll track movement of their ancestors, explicit viewports, and viewport ancestors. Prevents BorderBeam edge pixels from accumulating on repeated paints with fractional canvas sizes or high DPI. Hides the Ripple wave at zero scale to avoid a stray pixel on its initial frame. Keeps Calendar range boundaries inclusive for the entire first and last calendar days. Fixes backdrop sampling and rounded clipping using the wrong source region on high-DPI software rendering. Keeps Terminal asynchronous completion results and notifications single-shot, and completes commands with an error when their output cannot be converted to text. Fixes missed InfiniteScroll loads at the distance boundary, stale Collapse and Timeline content heights after delegate removal, and Loader binding loops when restoring these delegates. Preserves valid step bindings so Wizard navigation, completion, reset, and model changes keep the step indicator synchronized. Clears Anchor selection safely when its target is removed. Removes stale Card and Statistic slot sizes and reactivation binding loops. Keeps DynamicDialog completion results correct with zero-duration exits and guards deferred callbacks. Updates CommandPalette height after batched filtering and resizing. Removes Image error-delegate Loader binding loops and Qt 6 preview event-parameter warnings.

## 1.3.2 - 2026-10-20

1. 修复表单异步校验在重新校验、修改输入、清除、重置或销毁后仍等待旧结果的问题，避免过期回调覆盖当前校验状态。
2. 修复表单设置包含点号的字段名时误改嵌套数据的问题，优先匹配已存在的完整字段名。
3. 修复应用窗口动态替换或移除操作栏后宽度绑定未正确恢复的问题。
4. 改善 Gallery 左侧导航菜单文字的渲染清晰度，修复离屏图层导致的文字边缘锯齿。

English: Fixes asynchronous form validation remaining pending or applying stale results after revalidation, input changes, clearing, resetting, or destruction. Makes form setters prefer existing literal field names containing dots over nested paths. Restores width bindings when application window bars are replaced or removed. Improves Gallery sidebar text clarity by removing the offscreen layer that caused jagged text edges.

## 1.3.1 - 2026-10-15

1. 为头像、轮播、图片预览、上传、对比及视觉效果示例接入本地图片资源。
2. 修复 macOS 从图形界面准备 Qt 6 运行环境时无法找到 CMake 或 Ninja 的问题。
3. Qt 6 准备阶段使用循环进度条，进入安装后显示实际百分比，并完善中英文提示。
4. 修复循环进度条到达右端时整体跳回左端的问题，使移出与进入的部分连续衔接。
5. 图片预览仅在打开并启用缩略图时加载缩略图；错误示例改为点击后触发，避免打开页面时产生缺失图片警告。

English: Adds bundled local images to Gallery examples. Fixes CMake and Ninja discovery when preparing the Qt 6 runtime from macOS GUI applications. Shows indeterminate progress during runtime preparation and measured progress during installation. Makes linear indeterminate progress wrap continuously between the right and left edges. Defers preview thumbnails until needed and triggers the image error example only on request.

## 1.3.0 - 2026-10-15

1. 新增浮动标签、内嵌标签、关联标签、输入组合、旋钮和列表框。
2. 新增图片与内容对比、排序列表、组织图、分段计量和胶囊标签。
3. 新增字段分组、视口延迟加载、原位编辑、滚动动画、涟漪和焦点约束。
4. 新增停靠栏、多列菜单、响应式侧边导航、动态弹窗服务和交互终端。
5. 新增排程、任务看板和富文本编辑器，支持事件移动、任务拖拽及文本格式编辑。
6. 为既有组件增加密码强度与规则提示、穿梭框排序与跨栏拖动、右键坐标定位，以及图片缩略图、翻转和原图保存。
7. Gallery 增加 26 个组件入口和 149 个功能展示，同步中英文 API、类型提示及使用说明。
8. 修复自定义插槽尺寸循环、组件销毁及空数据异常、菜单弹窗时序、排程语言切换和深色示例文字对比度，完善 Qt 6 信号处理兼容性。
9. 优化无头像标签的加载开销。

English: Adds 26 public types covering field labels, grouped inputs, knobs, list boxes, content comparison, ordering, organization charts, meters, chips, containers, effects, focus management, navigation, dynamic dialogs, a terminal, a scheduler, a task board, and rich text editing. Extends password feedback, transfer ordering and dragging, context menu positioning, and image preview and saving. Gallery adds 149 runnable scenarios with bilingual API and usage documentation. Fixes slot sizing loops, lifecycle and null-data errors, popup timing, scheduler localization, dark-theme contrast, and Qt 6 signal handlers. Avoids unnecessary avatar loading in tags.

## 1.2.0 - 2026-10-15

1. 新增 `EFilePicker`、`EFolderPicker`，支持打开文件、多选、保存位置和目录选择。
2. 新增 `EToolBar`，支持操作排列、分隔、可切换操作、Qt Action 和溢出菜单。
3. 新增 `ECodeView`，支持 QML / JavaScript 高亮、行号、选择、滚动和源码复制；Gallery 源码面板复用该组件。
4. 新增 `ECommandPalette`，支持快捷键打开、搜索和键盘执行命令。
5. 新增 `EWizard`，支持步骤页面保留、前后导航、同步及异步离开校验。
6. Gallery 导航、总览和详情标记本版本新增组件，总览集中展示新增入口。
7. 修复 Qt 6 输入框、表单和穿梭框的尺寸绑定循环，以及 C++ 列表在表格、树选择器和级联选择器中的兼容问题。
8. 补齐更新空闲状态的中英文文案，修复数值输入框的按钮边界和开关销毁后的延迟回调。
9. 规避 Qt 5.15.2 Windows x64 的 JIT 异常表写入和嵌套异步创建崩溃，该环境下 Gallery 使用同步页面加载。
10. 修复 Qt 6 表格按行对象展开和选择器索引同步、抽屉拖动缩放、点击菜单意外关闭及图表加载事件参数兼容问题。

English: Adds file/folder pickers, a toolbar with overflow and Qt Action support, a reusable code view, a command palette, and a wizard with synchronous/asynchronous navigation validation. Gallery navigation, overview, and detail pages identify components introduced in the current version. Fixes Qt 6 sizing loops and C++ list compatibility in tables, tree selectors, and cascaders; fills the idle update translations, corrects spin-box bounds and deferred switch callbacks, and avoids the Qt 5.15.2 Windows x64 JIT unwind-table and nested asynchronous creation crashes by loading Gallery pages synchronously on that platform. Also fixes Qt 6 row-object expansion, selector index synchronization, drawer resizing, click-triggered menus closing unexpectedly, and chart loading-event parameters.

## 1.1.0 - 2026-10-13

- 新增 `EAnimation` 单例，提供 `play(target, effect, options)` 一行调用和 `stop(target)` 停止重置。
- 提供 83 个动画预设，覆盖淡入淡出、滑动、缩放、旋转与滚动、弹跳、翻转、强调反馈和快速倾斜滑动；保留已有布局及属性绑定。
- Gallery 新增动画页面，支持分类筛选、时长与距离调整、预览和复制源码；同步中英文使用说明。
- 为 EUI 自身代码添加 MIT 许可证，整理第三方许可和版权声明。
- 在中英文首页添加安装版、免安装版和 SHA-256 下载入口。
- 补齐贡献指南、Bug / 功能请求模板和 PR 模板。

English: Adds the `EAnimation` singleton with one-line playback and stop/reset, 83 animation presets that preserve layout and property bindings, and a Gallery page with category filters, timing/distance controls, previews, and copyable source. Includes bilingual animation documentation, the EUI MIT license and third-party notices, bilingual downloads and compatibility information, and contribution and feedback templates.

## 1.0.0 - 2026-10-13

- 首次发布 Windows x64 Gallery 安装版、免安装 ZIP 和 SHA-256 校验文件。
- Gallery 提供组件交互示例、源码复制、中英文切换和明暗主题。
- 提供中英文接入和组件使用指南。

English: First Windows x64 Gallery installer and portable release, with checksums, interactive component examples, source copying, language/theme switching, and bilingual integration and component usage guides.
