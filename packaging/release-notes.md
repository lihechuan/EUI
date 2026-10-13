EUI Gallery，适用于 Windows 10 / 11 x64。运行包包含所需运行库，目标电脑无需安装 Qt 或 Visual Studio。

| 文件 | 使用方式 |
| --- | --- |
| `*-setup.exe` | 双击安装，支持开始菜单入口、可选桌面快捷方式和卸载 |
| `*-portable.zip` | 完整解压后运行 `EUI-Gallery.exe`，保留全部 DLL 和子目录 |
| `SHA256SUMS.txt` | 使用 `Get-FileHash 文件路径 -Algorithm SHA256` 核对同名文件的校验值 |

发布前已通过包内容、ZIP 完整性、隔离桌面启动和附件下载校验。安装包未进行代码签名；图表、媒体、上传和打印等功能仍需在实际目标电脑上验证。免安装版的设置保存在应用原有的用户配置位置。

English: EUI Gallery for Windows 10 / 11 x64. Required runtimes are included; Qt and Visual Studio are not needed. Run the installer, or extract the entire portable ZIP and launch `EUI-Gallery.exe`. Verify downloads against `SHA256SUMS.txt`.

Package contents, ZIP integrity, isolated-desktop startup, and downloaded release files are checked before publication. The installer is unsigned. Charts, media, uploads, and printing require testing on the target computer. The portable edition retains the application's existing per-user settings behavior.
