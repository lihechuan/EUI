# 第三方软件与资源 / Third-party notices

根目录 [LICENSE](LICENSE) 的 MIT 许可适用于 EUI 自身代码和文档。下列第三方软件、资源及其修改保留各自的版权和许可；EUI 的 MIT 许可不替代这些条款。

The root [LICENSE](LICENSE) applies to EUI's own code and documentation. Third-party software, assets, and their modifications retain their respective copyright and license terms.

| 软件或资源 / Component | 位置 / Location | 许可与说明 / License and notices |
| --- | --- | --- |
| Apache ECharts 6.1.0 | `source/ThirdParty/echarts/` | [LICENSE](source/ThirdParty/echarts/LICENSE)、[NOTICE](source/ThirdParty/echarts/NOTICE) 和[来源记录 / source](source/ThirdParty/echarts/README.md) |
| QR Code generator 1.8.0 | `source/ThirdParty/qrcodegen/` | MIT；完整许可保留在 [qrcodegen.hpp](source/ThirdParty/qrcodegen/qrcodegen.hpp) 和 `.cpp` 文件头 / full license in the source headers |
| Skill Icons 品牌 SVG | `source/Qt5/imports/EUI/Font/Icons/` | [MIT](source/Qt5/imports/EUI/Font/Icons/LICENSE)；[来源和修改说明 / source and modifications](source/Qt5/imports/EUI/Font/Icons/README.md) |
| Qt 5.15.2 运行库 / runtime | Windows 安装版和免安装版 / Windows packages | [LGPL-3.0](packaging/licenses/Qt-LGPL-3.0.txt)、[GPL-3.0](packaging/licenses/Qt-GPL-3.0.txt)；具体模块与商业授权见 [Qt licensing](https://www.qt.io/licensing/) |
| Qt WebEngine 内含组件 / bundled components | Windows 包中的 WebEngine 运行库 / WebEngine runtime | [Qt WebEngine licensing and third-party notices](https://doc.qt.io/archives/qt-5.15/qtwebengine-licensing.html) |
| OpenSSL 1.1.1w | Windows 包中的 OpenSSL DLL / OpenSSL DLLs | [OpenSSL / SSLeay license](packaging/licenses/OpenSSL-LICENSE.txt) |
| Microsoft Visual C++ Runtime | Windows 包中的 MSVC DLL / MSVC DLLs | 随 Visual Studio 提供的运行库，按其再分发条款分发 / distributed under the applicable Visual Studio redistribution terms |
| Inno Setup 简体中文翻译 / Simplified Chinese translation | [ChineseSimplified.isl](packaging/ChineseSimplified.isl) | 文件头保留上游译者和来源信息 / upstream translator and source notices retained in the file header |

通过当前打包脚本生成的 Windows 包在 `licenses/` 中附带 EUI、Qt、OpenSSL、ECharts、QR Code generator 和 Skill Icons 的许可，以及本清单。品牌名称和图形的商标权不因代码许可证而转移。

Packages built with the current script include these licenses and notices in `licenses/`. Code licenses do not transfer trademark rights in brand names or artwork.
