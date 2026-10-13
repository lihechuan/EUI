# 更新记录 / Changelog

版本号来自根目录 `CMakeLists.txt`。正式发布使用对应 GitHub 提交上的 `v版本号` 标签；Release 记录完整源码提交号、构建环境和验证结果。

Versions come from the root `CMakeLists.txt`. Each release uses a `vVERSION` tag on its GitHub source commit and records the full commit, build environment, and verification results.

## 未发布 / Unreleased

- 为 EUI 自身代码添加 MIT 许可证，整理第三方许可，后续通过当前脚本生成的 Windows 包将附带相关声明。
- 在中英文首页添加安装版、免安装版和 SHA-256 下载入口，以及已验证环境表。
- 补齐英文打包指南、贡献指南、Bug / 功能请求模板和 PR 模板。
- 添加 Windows 自动构建、打包、包内容与后台启动验证，以及构建产物上传。
- 为旧 Release 补充标签源码与打包脚本提交的对应关系；现有安装版和免安装版保持不变。

English: Adds the EUI MIT license and third-party notices for subsequent builds, bilingual downloads and compatibility information, English packaging instructions, contribution and feedback templates, Windows build/package verification, and historical release provenance. Existing release binaries remain unchanged.

## 1.0.0 - 2026-10-13

- 首次发布 Windows x64 Gallery 安装版、免安装 ZIP 和 SHA-256 校验文件。
- Gallery 提供组件交互示例、源码复制、中英文切换和明暗主题。
- 提供中英文接入、组件使用和部署指南。

English: First Windows x64 Gallery installer and portable release, with checksums, interactive component examples, source copying, language/theme switching, and bilingual integration and deployment guides.

历史说明：`v1.0.0` 指向 `92529480e849693d34bdc711f25ec10fe47a85f6`；打包脚本随后在 `ca9579f08e74b2bef6e73cd9e9cc85d54c78349e` 提交。该旧标签的自动源码包没有 `packaging/`；原始打包流程的完整源码见[打包提交源码快照](https://github.com/lihechuan/EUI/archive/ca9579f08e74b2bef6e73cd9e9cc85d54c78349e.zip)。后续正式发布必须从包含打包脚本且通过验证的提交创建版本标签。

Historical note: The `v1.0.0` tag predates the packaging-script commit, so its automatic source archive lacks `packaging/`. The source snapshot linked above includes the original packaging process. Future releases must tag a verified commit containing the packaging scripts.
