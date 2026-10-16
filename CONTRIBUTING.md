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

## 提交 PR / Pull requests

请说明 PR 解决的问题，附上复现步骤和验证结果；界面修改可附截图。新增第三方代码或资源时，注明来源并保留许可和版权声明。EUI 原创贡献按根目录 MIT 许可证提供。

Describe the problem your PR solves and include reproduction steps and verification results. Screenshots help explain UI changes. Record the source of third-party code or assets and retain their licenses and copyright notices. Original contributions to EUI use the root MIT license.
