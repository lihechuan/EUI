# EUI 网页文档

本站使用 VitePress，中文为默认语言，英文页面位于 `/en/`。网页相关配置、主题和构建脚本集中在本目录；Qt 项目的构建不依赖 Node.js。

## 本地运行

安装 Node.js 22 或更新的 LTS 版本，在本目录运行：

```powershell
npm ci
npm run dev
```

打开终端输出的地址，默认是 `http://127.0.0.1:5173/EUI/`。

```powershell
npm run check
npm run build
npm run preview
```

`check` 检查中英文页面、内部链接及锚点、截图语言、组件示例、属性覆盖和关键方法。`build` 生成静态网站，输出到 `.vitepress/dist/`；`preview` 预览构建结果，默认地址为 `http://127.0.0.1:4173/EUI/`。

## 内容来源

| 内容 | 维护位置 |
| --- | --- |
| 首页布局与样式 | `.vitepress/theme/` |
| 快速开始与内容转换 | `scripts/generate.mjs` |
| 版本对比与下载地址 | `scripts/generate.mjs` |
| Python 发行版本号、指南与更新记录 | `content/pyside6/`，来源提交记录在 `source.json` |
| 构建、接入、功能、Gallery 及常见问题 | `../docs/guide.zh-CN.md`、`../docs/guide.en.md` |
| 组件分类、名称、示例入口与引入版本 | `../example/src/ExampleCatalog.cpp` |
| 属性类型、默认值与中英文说明 | `../example/data/component-api.json` |
| 组件示例 | Gallery 的 QML 页面、`ExampleData.cpp`、`PrimeSamples.js`、`AntDesignSamples.js` 和图表选项 |
| 信号和方法签名 | 组件 QML 源码及原生类的 `Q_INVOKABLE` 声明 |
| 信号、方法的中英文用途说明 | `scripts/interface-descriptions.mjs` |
| 截图、更新记录和标志 | `../docs/screenshots/`、`../CHANGELOG.md`、`../assets/eui-mark.svg` |

`dev`、`check` 和 `build` 会先生成 `.generated/`。修改内容来源后重新运行命令即可更新生成页面。开发服务器运行期间修改上游文档或 API 数据后，重新启动 `npm run dev`。

C++ / Qt Quick 指南和更新记录使用 `/guide/`、`/changelog`；Python / PySide6 使用 `/pyside6/guide/`、`/pyside6/changelog`，英文路径统一增加 `/en`。页面顶部可切换实现，并保留当前章节与语言。两版版本号和更新内容分别维护。

Python 内容是独立仓库已提交文档的本地快照，构建时无需访问网络或相邻仓库。同步时，从同一提交提取 `docs/guide.zh-CN.md`、`docs/guide.en.md` 和 `CHANGELOG.md` 到 `content/pyside6/`，并更新 `source.json` 的版本号与提交。当前两版指南使用的截图（含深色主题）内容相同，复用 `../docs/screenshots/1.3.2/`；同步新文档时需同时核对引用图片。文档来源链接指向快照对应的 Python 仓库提交。

生产预览启动后，可运行 `python scripts/check-editions.py [base-url]` 验证两版指南、更新记录、章节与语言切换，以及不同主题和屏幕宽度下的布局。需要已安装 Playwright 和 Chromium；也可通过 `CHROMIUM_EXECUTABLE` 指定浏览器。
