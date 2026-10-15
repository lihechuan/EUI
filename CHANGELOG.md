# 更新记录 / Changelog

记录各版本的功能、修复和兼容性变化。

Features, fixes, and compatibility changes for each version.

## 1.2.0 - 2026-10-15

1. 新增 `EFilePicker`、`EFolderPicker`，支持打开文件、多选、保存位置和目录选择。
2. 新增 `EToolBar`，支持操作排列、分隔、可切换操作、Qt Action 和溢出菜单。
3. 新增 `ECodeView`，支持 QML / JavaScript 高亮、行号、选择、滚动和源码复制；Gallery 源码面板复用该组件。
4. 新增 `ECommandPalette`，支持快捷键打开、搜索和键盘执行命令。
5. 新增 `EWizard`，支持步骤页面保留、前后导航、同步及异步离开校验。
6. Gallery 导航、总览和详情标记本版本新增组件，总览集中展示新增入口。
7. 修复 Qt 6 输入框、表单和穿梭框的尺寸绑定循环，以及 C++ 列表在表格、树选择器和级联选择器中的兼容问题。
8. 补齐更新空闲状态的中英文文案，修复数值输入框的按钮边界和开关销毁后的延迟回调；完善 Qt 5 / Qt 6 Gallery 与原生文件对话框回归。
9. 规避 Qt 5.15.2 Windows x64 的 JIT 异常表写入和嵌套异步创建崩溃，该环境下 Gallery 使用同步页面加载；修复测试退出时的异步创建清理。
10. 修复 Qt 6 表格按行对象展开和选择器索引同步、抽屉拖动缩放、点击菜单意外关闭及图表加载事件参数兼容问题。
11. 修复构建失败后同版本无法自动补发 Release 的问题，保留已公开版本及附件；仅修改文档和截图时跳过自动打包。

English: Adds file/folder pickers, a toolbar with overflow and Qt Action support, a reusable code view, a command palette, and a wizard with synchronous/asynchronous navigation validation. Gallery navigation, overview, and detail pages identify components introduced in the current version. Fixes Qt 6 sizing loops and C++ list compatibility in tables, tree selectors, and cascaders; fills the idle update translations, corrects spin-box bounds and deferred switch callbacks, extends Gallery and native file-dialog regression coverage for Qt 5 and Qt 6, and avoids the Qt 5.15.2 Windows x64 JIT unwind-table and nested asynchronous creation crashes by loading Gallery pages synchronously on that platform while ensuring pending creation completes before test shutdown. Also fixes Qt 6 row-object expansion, selector index synchronization, drawer resizing, click-triggered menus closing unexpectedly, and chart loading-event parameters. Failed releases can now recover on a later successful push without a version change; published releases and their assets are preserved. Documentation and screenshot changes alone skip automatic packaging.

## 1.1.0 - 2026-10-13

- 新增 `EAnimation` 单例，提供 `play(target, effect, options)` 一行调用和 `stop(target)` 停止重置。
- 提供 83 个动画预设，覆盖淡入淡出、滑动、缩放、旋转与滚动、弹跳、翻转、强调反馈和快速倾斜滑动；保留已有布局及属性绑定。
- Gallery 新增动画页面，支持分类筛选、时长与距离调整、预览和复制源码；同步中英文使用说明。
- 为 EUI 自身代码添加 MIT 许可证，整理第三方许可，后续通过当前脚本生成的 Windows 包将附带相关声明。
- 在中英文首页添加安装版、免安装版和 SHA-256 下载入口，以及已验证环境表。
- 补齐英文打包指南、贡献指南、Bug / 功能请求模板和 PR 模板。
- 添加 Windows 自动构建、打包、包内容与后台启动验证，以及构建产物上传。
- 版本号变化并推送到 `master` 后，自动发布安装包、免安装 ZIP 和校验文件到 GitHub Releases。

English: Adds the `EAnimation` singleton with one-line playback and stop/reset, 83 animation presets that preserve layout and property bindings, and a Gallery page with category filters, timing/distance controls, previews, and copyable source. Includes bilingual animation documentation, the EUI MIT license and third-party notices, bilingual downloads and compatibility information, English packaging instructions, contribution and feedback templates, Windows build/package verification, and automatic releases when the version changes on `master`.

## 1.0.0 - 2026-10-13

- 首次发布 Windows x64 Gallery 安装版、免安装 ZIP 和 SHA-256 校验文件。
- Gallery 提供组件交互示例、源码复制、中英文切换和明暗主题。
- 提供中英文接入、组件使用和部署指南。

English: First Windows x64 Gallery installer and portable release, with checksums, interactive component examples, source copying, language/theme switching, and bilingual integration and deployment guides.
