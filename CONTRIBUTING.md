# 参与贡献 / Contributing

欢迎通过 [GitHub Issues](https://github.com/lihechuan/EUI/issues) 提交问题或功能建议，也可联系 `xiaojiaqaq@gmail.com`。请使用 Issue 模板，先搜索已有问题；附上 Qt 版本、操作系统、编译器、组件名称、复现步骤、预期行为和错误日志。日志和示例不要包含密码、令牌或个人数据。

Use [GitHub Issues](https://github.com/lihechuan/EUI/issues) or email `xiaojiaqaq@gmail.com`. Search existing issues and use the templates. Include your Qt version, OS, compiler, component, reproduction steps, expected behavior, and logs without credentials or personal data.

## 构建与验证 / Build and verification

从[中文使用指南](docs/guide.zh-CN.md#1-环境准备)或 [English guide](docs/guide.en.md#1-prerequisites) 准备 Windows、Qt 5.15.2 MSVC 2019 x64 Kit、CMake 3.20+ 和 Ninja。在 x64 Visual Studio Developer PowerShell 中执行：

Prepare the toolchain using the guide above, then run in an x64 Visual Studio Developer PowerShell:

```powershell
$qtKit = 'D:/Qt/5.15.2/msvc2019_64'
$env:PATH = "$qtKit/bin;" + $env:PATH
cmake -S . -B build/local -G Ninja -DCMAKE_BUILD_TYPE=Release "-DCMAKE_PREFIX_PATH=$qtKit" -DEUI_BUILD_EXAMPLES=ON -DEUI_BUILD_TESTS=OFF -DQML_PLUGIN_DUMP:FILEPATH=OFF
cmake --build build/local --target example --parallel
git diff --check
```

修改打包流程时，按[中文打包指南](docs/guide.zh-CN.md#63-生成-windows-安装包和免安装版)或 [English packaging guide](docs/guide.en.md#63-build-a-windows-installer-and-portable-package) 准备 Inno Setup 与 OpenSSL，执行打包后运行 `python packaging/check_windows_package.py`。验证脚本在隔离桌面启动程序，不显示到当前桌面。

For packaging changes, follow the packaging guide, build the packages, and run `python packaging/check_windows_package.py`. The check launches the application on an isolated desktop. GitHub Actions builds and packages the public sources, checks ZIP/runtime files and background startup, and retains the installer, portable ZIP, and checksums for 14 days. These checks do not replace target-machine testing of charts, media, uploads, or printing.

## 修改范围与 PR / Changes and pull requests

- 保持修改聚焦，遵循现有代码风格，保留执行、布局和初始化顺序；避免无关格式化、重构和新增依赖。
- 修改公共属性、行为或部署方式时，同步示例和中英文文档。
- Bug 修复提供最小复现及针对性验证；界面修改附效果图和交互验证结果。
- PR 说明解决的问题、最终行为和已完成的验证；未验证内容明确标注。
- 新增第三方代码或资源时，记录来源并保留许可和版权声明。提交 EUI 原创贡献表示同意按根目录 MIT 许可证提供这些贡献。

Keep changes focused and follow existing style and semantic ordering. Update examples and both languages when public behavior changes. Include a reproduction for bug fixes, screenshots for UI changes, and verification results in the PR. Document third-party provenance and retain notices. Original contributions to EUI are submitted under the root MIT license.

## 正式发布 / Releases

将根目录 `CMakeLists.txt` 中 `project(EUI VERSION X.Y.Z)` 的版本号改为新版本并推送到 `master`，桌面工作流会分别构建 Windows x86、Windows x64、macOS Intel 和 Apple Silicon，全部打包与验证通过后创建 `vX.Y.Z` 标签及 GitHub Release，上传八个安装版或免安装包和 `SHA256SUMS.txt`。Windows 保留 Qt 5.15.2；macOS 使用 Qt 6.8.3。无需手动创建标签或配置额外令牌。

普通提交、PR 和手动运行继续执行构建验证，构建产物保留 14 天；版本号不变时不发布新 Release。也可推送与版本号一致的 `vX.Y.Z` 标签触发发布。附件在草稿中上传并下载核对 SHA-256，全部通过后才公开。失败时在 Actions 中重跑失败的作业；已公开的版本保持原样，不覆盖附件。

Change `project(EUI VERSION X.Y.Z)` in the root `CMakeLists.txt` and push to `master`. The desktop workflow builds Windows x86, Windows x64, macOS Intel, and Apple Silicon. Once every platform passes packaging and verification, it creates the matching tag and GitHub Release with eight installer or portable packages and `SHA256SUMS.txt`. Windows retains Qt 5.15.2; macOS uses Qt 6.8.3. No manual tag or extra token is required. Regular commits, PRs, and manual runs retain build artifacts for 14 days without publishing a new release. Matching version tags can also trigger publication. Assets are uploaded to a draft and downloaded for SHA-256 verification before publication. Rerun failed jobs to resume; published versions are preserved.

1. 更新根目录 `CMakeLists.txt` 的版本、`CHANGELOG.md`、中英文下载链接和打包示例中的版本。用户文档只展示当前仓库所属平台的入口，不并列展示不同托管平台。
2. 提交并同步源码，确认 GitHub 的全部平台打包与自动发布作业通过；发布包只能来自已验证的干净源码。
3. GitHub 由工作流完成标签、发行版和附件发布；如另有要求发布的托管平台，为对应源码创建相同的 `vVERSION` 标签和发行版，并上传同次构建生成的附件，保持文件名和内容一致。
4. Release 说明只用简短的编号列表列出本次新增、修改和修复的功能。使用方式、兼容环境、验证结果和已知限制放在用户文档中，不加入发行说明。
5. 分别检查各平台的附件列表和下载链接，下载文件并核对 SHA-256。要求发布的所有平台全部附件发布并验证成功后，才算发布完成；任一平台失败时，明确说明失败的平台、原因和待补传文件，并继续补齐。
6. 保留已有版本标签和发布文件，不覆盖已发布附件；发行说明可根据已确认的功能变更更新。

User documentation links only to its own hosting platform. Synchronize the source and require passing build and release jobs on GitHub. For any additional required hosting platform, use matching version tags and identical build attachments. Release notes use a short numbered list of features added, changed, or fixed in that version. Keep usage, compatibility, verification results, and known limitations in user documentation. Verify downloads and SHA-256 on each required platform before declaring a release complete. Preserve existing version tags and published assets; descriptions may be updated to reflect confirmed feature changes.
