EUI Gallery — Windows @ARCH@

运行要求：@WINDOWS_REQUIREMENTS@。
免安装版：解压完整目录后双击 EUI-Gallery.exe；请保留旁边的 DLL 和所有子目录。
安装版：运行 EUI-Gallery-版本-windows-@ARCH@-setup.exe，按向导安装。
安装到当前用户目录，无需管理员权限；可创建桌面快捷方式，并通过 Windows 应用列表卸载。

运行文件已包含 Qt、EUI QML 模块、WebEngine、Multimedia、SVG、MSVC 和 OpenSSL 运行库。
运行程序不需要安装 Qt、Visual Studio 或其他开发工具。
“免安装”表示无需安装程序；表格布局等设置仍遵循应用原有的用户配置存储方式。

软件更新：每次启动会自动检查正式版本，顶部 dock 栏提供更新入口，点击可查看状态和进度。
发现新版后点击“立即更新”，进度包含下载、SHA-256 校验和安装准备，随后自动重启。
新版本成功启动后显示“更新完成”；失败时保留或恢复旧版本，可点击“重新检查”。
安装版与免安装版均在原目录更新，用户设置保留。旧目录保留为旁边的 .before-.eui-update-* 备份。
确认新版本正常后可手动删除备份；缺少对应系统、架构的更新包时不会安装其他架构的包。

3D 示例：进入“视觉效果 → Littlest Tokyo · 3D 动画”，点击“安装并打开 3D 示例”。
安装进度显示在页面中；完成后自动启动 Qt 6 Gallery，直接显示模型，原 Qt 5 窗口退出。
Qt 6 运行环境已包含在 quick3d-runtime/，不需要联网下载。
文件校验后安装到 %LOCALAPPDATA%/EUI/Quick3D；后续使用前会校验并复用缓存。
需要 Windows 10 / 11 64 位和可用的图形后端；32 位 Windows 无法运行此 Qt 6 3D 示例。

Qt: https://www.qt.io/licensing/
Qt WebEngine third-party notices: https://doc.qt.io/archives/qt-5.15/qtwebengine-licensing.html
OpenSSL: https://www.openssl.org/
EUI 使用 MIT 许可证。EUI、Qt、OpenSSL、ECharts、QR Code generator 和 Skill Icons 许可证见 licenses/。
第三方许可与来源说明见 licenses/THIRD_PARTY_NOTICES.md。

Requirements: @WINDOWS_REQUIREMENTS_EN@. Qt and Visual Studio are not required on the target computer.
Portable: extract the complete directory and run EUI-Gallery.exe; keep all DLLs and subdirectories.
Installer: run the setup executable; it installs per user without administrator privileges.
Updates: startup checks official releases. Use the top dock bar to view status and progress; click Update now to download, verify, install and restart.
Completion is shown after the new version starts. Failures preserve or restore the previous version.
Settings are retained. The adjacent .before-.eui-update-* directory keeps a backup; remove it after verifying the update.
3D: open Visual effects → Littlest Tokyo and click Install and open 3D example.
The bundled Qt 6 runtime installs per user with progress, then opens the 3D page in a new Gallery.
No network or Qt SDK is required. The 3D runtime requires Windows x64 and a working graphics backend.
EUI is MIT licensed. Third-party licenses and notices are included in licenses/.
Downloads and source: https://github.com/lihechuan/EUI/releases
