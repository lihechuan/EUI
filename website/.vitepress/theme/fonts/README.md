# 网页字体

页面文字使用 Noto Sans SC，代码使用 JetBrains Mono。WOFF2 文件从下列 Google Fonts 上游可变字体生成，保留当前网页内容中的字符、Latin-1 字符和 400–700 字重。未收录的字符回退到系统字体。

| 字体 | 固定版本来源 | 原始 TTF 的 SHA-256 | 许可 |
| --- | --- | --- | --- |
| Noto Sans SC | [Google Fonts a85815a](https://github.com/google/fonts/blob/a85815a42757630ce188fdad368c2dfc444d4773/ofl/notosanssc/NotoSansSC%5Bwght%5D.ttf) | `a3041811a78c361b1de50f953c805e0244951c21c5bd412f7232ef0d899af0da` | [SIL OFL 1.1](NotoSansSC-OFL.txt) |
| JetBrains Mono | [Google Fonts 6e4b84c](https://github.com/google/fonts/blob/6e4b84c976cadb3c49a40fd9a1c203e4f7fcf2da/ofl/jetbrainsmono/JetBrainsMono%5Bwght%5D.ttf) | `48715a42ec242c21e9f02692891e147d022299a52e48d5e413e1a942193ffeda` | [SIL OFL 1.1](JetBrainsMono-OFL.txt) |

版权和完整许可同时保存在 WOFF2 的 `name` 元数据中。修改字体内容后须保留这些信息。

更新较多文档文字后，可以下载以上两个 TTF 文件，在 `website/` 中重新生成子集：

```powershell
npm run generate
python -m pip install 'fonttools[woff]'
python scripts/subset-fonts.py --sans <NotoSansSC.ttf> --mono <JetBrainsMono.ttf>
npm run build
```

Python 和 FontTools 仅用于维护已提交的字体文件，日常网站构建不依赖它们。
