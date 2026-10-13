# 更新记录 / Changelog

记录各版本的功能、修复和兼容性变化。

Features, fixes, and compatibility changes for each version.

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
