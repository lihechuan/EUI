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

Gallery 的打包和运行验证由 [GitHub Actions](https://github.com/lihechuan/EUI/actions/workflows/build-windows.yml) 执行。源码、资源、构建配置、打包脚本及随包许可证变化时自动构建；仅修改使用文档、截图、发行说明或 Issue / PR 模板时跳过。版本标签推送和手动运行仍可触发完整构建。修改工作流或 `packaging/` 下的脚本后，通过 PR 检查或手动运行工作流验证结果。成功运行的构建产物保留 14 天，下载方式见[中文指南](docs/guide.zh-CN.md#63-github-自动打包与下载)或 [English guide](docs/guide.en.md#63-github-packages-and-downloads)。

Gallery packaging and runtime checks run in GitHub Actions when source, resources, build configuration, packaging scripts, or bundled licenses change. Documentation, screenshots, release notes, and Issue / PR templates alone do not trigger packaging. Version tags and manual runs still trigger the complete build. After changing the workflow or scripts in `packaging/`, verify the PR checks or run the workflow manually. Successful build artifacts are retained for 14 days; see the guides above for downloads. These checks do not replace target-machine testing of charts, media, uploads, or printing.

## 修改范围与 PR / Changes and pull requests

- 保持修改聚焦，遵循现有代码风格，保留执行、布局和初始化顺序；避免无关格式化、重构和新增依赖。
- 修改公共属性、行为或部署方式时，同步示例和中英文文档。
- Bug 修复提供最小复现及针对性验证；界面修改附效果图和交互验证结果。
- PR 说明解决的问题、最终行为和已完成的验证；未验证内容明确标注。
- 新增第三方代码或资源时，记录来源并保留许可和版权声明。提交 EUI 原创贡献表示同意按根目录 MIT 许可证提供这些贡献。

Keep changes focused and follow existing style and semantic ordering. Update examples and both languages when public behavior changes. Include a reproduction for bug fixes, screenshots for UI changes, and verification results in the PR. Document third-party provenance and retain notices. Original contributions to EUI are submitted under the root MIT license.

## 正式发布 / Releases

将根目录 `CMakeLists.txt` 中 `project(EUI VERSION X.Y.Z)` 的版本号改为新版本并推送到 `master`，桌面工作流会分别构建 Windows x86、Windows x64、macOS Intel 和 Apple Silicon，全部打包与验证通过后创建 `vX.Y.Z` 标签及 GitHub Release，上传八个安装版或免安装包和 `SHA256SUMS.txt`。Windows 保留 Qt 5.15.2；macOS 使用 Qt 6.8.3。无需手动创建标签或配置额外令牌。

推送到 `master` 并触发打包时，会检查当前版本是否已有公开 Release；尚未发布时，即使版本号没有变化，也会在全部平台验证通过后发布，补齐之前失败的发布。PR 和手动运行只执行构建验证，构建产物保留 14 天。也可推送与版本号一致的 `vX.Y.Z` 标签触发发布。附件在草稿中上传并下载核对 SHA-256，全部通过后才公开。失败时在 Actions 中重跑失败的作业；已公开的版本保持原样，不覆盖附件。

Change `project(EUI VERSION X.Y.Z)` in the root `CMakeLists.txt` and push to `master`. The desktop workflow builds Windows x86, Windows x64, macOS Intel, and Apple Silicon. Once every platform passes packaging and verification, it creates the matching tag and GitHub Release with eight installer or portable packages and `SHA256SUMS.txt`. Windows retains Qt 5.15.2; macOS uses Qt 6.8.3. No manual tag or extra token is required. Every push to `master` that triggers packaging checks whether the current version has a published release. If it does not, a successful build publishes it even when the version is unchanged, allowing failed releases to recover. PRs and manual runs only build and verify. Build artifacts are retained for 14 days. Matching version tags can also trigger publication. Assets are uploaded to a draft and downloaded for SHA-256 verification before publication. Rerun failed jobs to resume; published versions are preserved.

1. 更新根目录 `CMakeLists.txt` 的版本、`CHANGELOG.md` 和中英文下载链接。用户文档统一使用 [GitHub Releases](https://github.com/lihechuan/EUI/releases) 作为正式下载入口。
2. 提交源码并推送到 GitHub 的 `master`，确认全部平台打包与自动发布作业通过；发布包只能来自已验证的干净源码。
3. 由 GitHub 工作流完成标签、发行版和附件发布。
4. Release 说明只用简短的编号列表列出本次新增、修改和修复的功能。使用方式、兼容环境、验证结果和已知限制放在用户文档中，不加入发行说明。
5. 检查 GitHub Release 的附件列表和下载链接，下载各系统和架构的文件并核对 SHA-256。全部附件发布并验证成功后，才算发布完成；任一附件失败时，明确说明原因和待补传文件，并继续补齐。
6. 保留已有版本标签和发布文件，不覆盖已发布附件；发行说明可根据已确认的功能变更更新。

Use [GitHub Releases](https://github.com/lihechuan/EUI/releases) for all documented release downloads. Push source changes to GitHub's `master` and require passing build and release jobs. The GitHub workflow creates the tags, releases, and attachments. Release notes use a short numbered list of features added, changed, or fixed in that version. Keep usage, compatibility, verification results, and known limitations in user documentation. Verify downloads and SHA-256 for every supported OS and architecture before declaring a release complete. Preserve existing version tags and published assets; descriptions may be updated to reflect confirmed feature changes.
