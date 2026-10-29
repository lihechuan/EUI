import assert from 'node:assert/strict'
import { copyFileSync, existsSync, mkdirSync, readFileSync, readdirSync, rmSync, writeFileSync } from 'node:fs'
import path from 'node:path'
import { gzipSync } from 'node:zlib'
import { createHash } from 'node:crypto'
import { fileURLToPath } from 'node:url'
import { prepareExamples } from '../playground/examples.mjs'
import { describeInterface } from './interface-descriptions.mjs'

const website = fileURLToPath(new URL('../', import.meta.url))
const root = path.resolve(website, '..')
const output = path.join(website, '.generated')
const repository = 'https://github.com/lihechuan/EUI'
const moduleRoot = 'source/qt5/imports/eui'
const read = file => readFileSync(path.join(root, file), 'utf8').replace(/\r\n/g, '\n')
const pythonSource = JSON.parse(read('website/content/pyside6/source.json'))
const pythonVersion = pythonSource.version
const pythonRepository = pythonSource.repository
const escape = text => String(text ?? '').replaceAll('&', '&amp;').replaceAll('<', '&lt;').replaceAll('>', '&gt;').replaceAll('"', '&quot;')
const version = read('CMakeLists.txt').match(/project\(EUI VERSION ([\d.]+)\)/)?.[1]
assert(version, 'Cannot read the EUI version from CMakeLists.txt')

const api = JSON.parse(read('example/data/component-api.json'))
const locales = Object.fromEntries(['zh', 'en'].map(language => [language, JSON.parse(read(`${moduleRoot}/i18n/${language}.json`))]))
const catalog = read('example/src/ExampleCatalog.cpp')
const definitions = new Map([...catalog.matchAll(/constexpr Entry (\w+)\[\] = \{([\s\S]*?)\n\};/g)].map(([, name, body]) => [
  name,
  [...body.matchAll(/\{"([^"]+)", "([^"]+)", "([^"]+)", "([^"]+)"(?:, (true|false))?(?:, "([^"]+)")?\}/g)]
    .map(([, name, source, zh, en, demo, introducedIn]) => ({ name, source, zh, en, demo: demo === 'true', introducedIn }))
]))
const groups = [...catalog.matchAll(/\{sizeof\((\w+)\) \/ sizeof\(Entry\), "(\w+)", \1, "([^"]+)", "([^"]+)"\}/g)]
  .map(([, name, id, zh, en]) => ({ id, zh, en, items: definitions.get(name) }))
assert(groups.length && groups.every(group => group.items?.length), 'The Gallery catalog format has changed')
const components = groups.flatMap(group => group.items.filter(item => !item.demo).map(item => ({ ...item, group })))
assert.equal(new Set(components.map(item => item.name)).size, components.length, 'Duplicate Gallery component')
for (const item of components) assert(api[item.name]?.properties, `Missing API: ${item.name}`)

const qmlTypes = new Map([...read(`${moduleRoot}/qmldir`).matchAll(/^(?:internal )?(\w+)(?: 1\.0)? (controls\/.*\.qml)$/gm)]
  .map(([, name, source]) => [name, `${moduleRoot}/${source}`]))
const samples = new Map([...read('example/src/ExampleData.cpp').matchAll(/\{"(\w+)",\s*R"qml\(([\s\S]*?)\)qml",\s*R"qml\(([\s\S]*?)\)qml"\}/g)]
  .map(([, name, zh, en]) => [name, { zh, en }]))
assert(samples.size > 0, 'Cannot read localized Gallery samples')

function jsonAssignment(file) {
  const text = read(file)
  return JSON.parse(text.slice(text.indexOf('{'), text.lastIndexOf('}') + 1))
}

const prime = jsonAssignment('example/qml/component/PrimeSamples.js')
const core = jsonAssignment('example/qml/component/AntDesignSamples.js')
const desktopText = read('example/qml/component/DesktopExamples.qml')
const desktop = JSON.parse(desktopText.match(/readonly property var definitions:\s*(\[[\s\S]*?\n\])/)[1])
const charts = JSON.parse(read('example/data/echarts-options.json'))
const chartLabels = new Map([...read('example/src/ExampleData.cpp').matchAll(/\{QStringLiteral\("value"\), QStringLiteral\("(\w+)"\)\},\s*\{QStringLiteral\("label"\), QString::fromUtf8\(chinese \? "([^"]+)" : "([^"]+)"\)\}/g)]
  .map(([, name, zh, en]) => [name, { zh, en }]))
const guideNames = ['prerequisites', 'build', 'integration', 'features', 'gallery', 'troubleshooting']
const guides = Object.fromEntries(['zh', 'en'].map(language => {
  const file = `docs/guide.${language === 'zh' ? 'zh-CN' : 'en'}.md`
  const text = read(file)
  const headings = [...text.matchAll(/^## (\d+)\. (.+)$/gm)]
  assert.equal(headings.length, guideNames.length, `The guide structure has changed: ${file}`)
  return [language, headings.map((heading, index) => ({
    file,
    route: `${language === 'en' ? '/en' : ''}/guide/${guideNames[index]}`,
    text: text.slice(heading.index, headings[index + 1]?.index ?? text.length).trim().replace(/^(#{2,}) /gm, (_, marks) => marks.slice(1) + ' '),
    title: heading[2]
  }))]
}))

const pythonGuides = Object.fromEntries(['zh', 'en'].map(language => {
  const file = `website/content/pyside6/guide.${language === 'zh' ? 'zh-CN' : 'en'}.md`
  const text = read(file)
  const headings = [...text.matchAll(/^## (\d+)\. (.+)$/gm)]
  assert.equal(headings.length, guideNames.length, `The Python guide structure has changed: ${file}`)
  return [language, headings.map((heading, index) => ({
    file: `docs/${path.basename(file)}`,
    route: `${language === 'en' ? '/en' : ''}/pyside6/guide/${guideNames[index]}`,
    text: text.slice(heading.index, headings[index + 1]?.index ?? text.length).trim().replace(/^(#{2,}) /gm, (_, marks) => marks.slice(1) + ' ')
  }))]
}))

function write(file, text) {
  const target = path.join(output, file)
  mkdirSync(path.dirname(target), { recursive: true })
  writeFileSync(target, text)
}

function page(route, body, source, extra = {}) {
  const frontmatter = Object.entries({ source, ...extra }).map(([key, value]) => `${key}: ${JSON.stringify(value)}`).join('\n')
  write(`${route.replace(/^\//, '').replace(/\/$/, '/index') || 'index'}.md`, `---\n${frontmatter}\n---\n\n${body}\n`)
}

function guideRoute(language, hash = '', edition = '') {
  const prefix = `${language === 'en' ? '/en' : ''}${edition}`
  if (!hash) return `${prefix}/guide/getting-started`
  const section = Number(hash.match(/^#(\d)/)?.[1])
  assert(section >= 1 && section <= guideNames.length, `Unknown guide anchor: ${hash}`)
  return `${prefix}/guide/${guideNames[section - 1]}${hash}`
}

function editionNavigation(language, edition, route) {
  const prefix = language === 'en' ? '/en' : ''
  const directory = path.posix.dirname(`${prefix}${edition}${route}`)
  return `<nav class="edition-switch" aria-label="${language === 'zh' ? '实现版本' : 'Implementation'}">${[['', 'C++ / Qt Quick'], ['/pyside6', 'Python / PySide6']].map(([target, label]) => `<a href="${path.posix.relative(directory, `${prefix}${target}${route}`)}.html"${target === edition ? ' aria-current="page"' : ''}>${label}</a>`).join('')}</nav>\n\n`
}

function changelogPage(language, edition = '') {
  const chinese = language === 'zh'
  const python = edition === '/pyside6'
  const sections = read(python ? 'website/content/pyside6/CHANGELOG.md' : 'CHANGELOG.md').split(/^## /m).slice(1)
  assert(sections.length, 'No changelog versions found')
  const entries = sections.map(section => {
    const end = section.indexOf('\n')
    const heading = section.slice(0, end)
    const translations = section.slice(end).split(python ? /(?=^1\. )/m : /^English: */m).map(text => text.trim()).filter(Boolean)
    assert(translations.length === 2 && translations.every(Boolean), `Missing changelog translation: ${heading}`)
    return `## ${heading}\n\n${translations[chinese ? 0 : 1]}`
  })
  const introduction = chinese
    ? '# 更新记录\n\n记录各版本的功能、修复和兼容性变化。'
    : '# Changelog\n\nFeatures, fixes, and compatibility changes for each version.'
  const implementation = python ? `EUI-PySide6 · Python/PySide6 · v${pythonVersion}` : `EUI · C++ / Qt Quick · v${version}`
  return editionNavigation(language, edition, '/changelog') + introduction + `\n\n**${implementation}**\n\n` + entries.join('\n\n')
}

function screenshotMarkup(target, title, original = '') {
  const images = /-dark-(zh|en)\.png$/.test(target)
    ? [target] : [target, target.replace(/-(zh|en)\.png$/, '-dark-$1.png')]
  return images.map((image, index) => {
    assert(existsSync(path.join(output, 'public', image)), `Missing themed screenshot: ${image}`)
    const theme = images.length > 1 ? ` data-preview-theme="${index ? 'dark' : 'light'}"` : ''
    const markup = `<img${original ? '' : theme} src="${image}" alt="${escape(title)}" loading="lazy">`
    return original ? `<a${theme} href="${original}${image}" target="_blank" rel="noreferrer">${markup}</a>` : markup
  }).join('')
}

function rewriteLinks(text, source, language, edition = '') {
  const prefix = language === 'en' ? '/en' : ''
  const python = edition === '/pyside6'
  let fence = false
  return text.split('\n').map(line => {
    if (/^\s*```/.test(line)) fence = !fence
    if (fence) return line
    if (/^#{1,6} /.test(line) && !line.includes('{#')) {
      const slug = line.replace(/^#+ /, '').toLowerCase().replace(/[^\p{L}\p{N}_ -]/gu, '').replaceAll(' ', '-')
      line += ` {#${slug}}`
    }
    return line.replace(/(!?\[[^\]]*\])\(([^)]+)\)/g, (original, label, target) => {
      if (/^(?:[\w+.-]+:|\/)/.test(target)) return original
      const [file, anchor] = target.split('#')
      const hash = anchor ? `#${anchor}` : ''
      const resolved = file ? path.posix.normalize(path.posix.join(path.posix.dirname(source), file)) : source
      let route
      if (/^docs\/guide\.(zh-CN|en)\.md$/.test(resolved)) {
        route = guideRoute(resolved.includes('zh-CN') ? 'zh' : 'en', hash, edition)
      } else if (resolved === 'README.md' || resolved === 'docs/README.zh-CN.md') {
        route = python ? `${prefix}/versions${hash}` : `${resolved === 'README.md' ? '/en' : ''}/` + hash
      } else if (resolved === 'CHANGELOG.md') {
        route = `${prefix}${edition}/changelog${hash}`
      } else if (resolved === 'docs/primevue-completion.md' && !python) {
        route = `${prefix}/guide/extensions${hash}`
      } else if (resolved === 'docs/screenshots/README.md') {
        route = `${prefix}/guide/screenshots${hash}`
      } else if (/^docs\/screenshots\/[^/]+\/[^/]+\.png$/.test(resolved)) {
        route = '/' + resolved.replace(/^docs\//, '')
        if (label.startsWith('!')) return screenshotMarkup(route, label.slice(2, -1))
      } else {
        if (!python) assert(existsSync(path.join(root, resolved)), `Missing document target: ${source} -> ${target}`)
        route = `${python ? pythonRepository : repository}/blob/${python ? pythonSource.revision : 'master'}/${resolved}${hash}`
      }
      return `${label}(${route})`
    })
  }).join('\n')
}

function dedent(text) {
  const lines = text.replace(/^\n/, '').trimEnd().split('\n')
  const indentation = Math.min(...lines.filter(line => line.trim()).map(line => line.match(/^\s*/)[0].length))
  return lines.map(line => line.slice(indentation)).join('\n')
}

function translateCode(code, language) {
  return code.replace(/\bELocale\.texts\.(\w+)\b/g, (original, key) => locales[language][key] ? `qsTr(${JSON.stringify(locales[language][key])})` : original)
}

function examples(item, language) {
  const result = [...(prime[item.name]?.cases || []), ...(core[item.name] || []), ...(desktop.find(entry => entry.name === item.name)?.cases || [])].map(sample => ({
    title: locales[language][sample.title] || sample.title,
    code: sample.code
  }))
  if (item.name === 'EECharts') {
    for (const [name, option] of Object.entries(charts)) result.push({
      title: chartLabels.get(name)?.[language] || name,
      code: `import EUI 1.0\nimport QtQuick 2.15\n\nEECharts {\n    height: 360\n    width: parent.width\n    option: (${JSON.stringify(option, null, 4).replace(/\n/g, '\n    ')})\n}`
    })
  }
  const qml = read(`example/qml/component/${item.source}`)
  for (const section of qml.split(/\bExampleSection\s*\{/).slice(1)) {
    if (section.match(/componentName: "(\w+)"/)?.[1] !== item.name) continue
    const expressions = /code:\s*(?:GalleryStyle\.sourceCode\(`([\s\S]*?)`\)|exampleData\.samples\.(\w+))/g
    for (const match of section.matchAll(expressions)) {
      const preceding = section.slice(0, match.index)
      const title = [...preceding.matchAll(/title: (?:ELocale\.texts\.(\w+)|"([^"]+)")/g)].at(-1)
      const code = match[1] !== undefined ? dedent(match[1]) : samples.get(match[2])?.[language]
      assert(code, `Missing Gallery sample: ${item.name} ${match[2]}`)
      result.push({ title: locales[language][title?.[1]] || title?.[2] || item[language], code })
    }
  }
  const unique = new Map()
  for (const sample of result) {
    let code = translateCode(sample.code, language).trim()
    if (!/^import /m.test(code)) code = `import EUI 1.0\nimport QtQuick 2.15\n\n${code}`
    if (!unique.has(code)) unique.set(code, { title: sample.title, code })
  }
  return [...unique.values()]
}

function interfaces(name, visited = new Set()) {
  if (visited.has(name)) return { signals: [], methods: [] }
  visited.add(name)
  const file = qmlTypes.get(name)
  if (file) {
    const text = read(file)
    const base = text.match(/^(\w+)\s*\{/m)?.[1]
    const inherited = base ? interfaces(base, visited) : { signals: [], methods: [] }
    return {
      signals: [...inherited.signals, ...[...text.matchAll(/^ {4}signal ([^\n]+)/gm)].map(match => ({ signature: match[1], owner: name }))],
      methods: [...inherited.methods, ...[...text.matchAll(/^ {4}function (\w+\([^\n]*\))\s*\{/gm)].map(match => ({ signature: match[1], owner: name }))]
    }
  }
  const header = api[name]?.properties.find(row => row.owner === name && row.source.endsWith('.h'))?.source
  if (!header) return { signals: [], methods: [] }
  const text = read(header)
  const signalSection = text.match(/signals:\s*([\s\S]*?)(?:\n(?:private|public|protected):|\n\};)/)?.[1] || ''
  return {
    signals: [...signalSection.matchAll(/void ([^;]+);/g)].map(match => ({ signature: match[1].replace(/\s+/g, ' '), owner: name })),
    methods: [...text.matchAll(/Q_INVOKABLE\s+([^;]+);/g)].map(match => ({ signature: match[1].replace(/\s+/g, ' '), owner: name }))
  }
}

function propertyReference(rows, language) {
  const chinese = language === 'zh'
  let text = '<div class="api-reference" v-pre>\n'
  text += `<div class="api-reference-head"><span>${chinese ? '属性' : 'Property'}</span><span>${chinese ? '说明' : 'Description'}</span><span>${chinese ? '取值与默认值' : 'Values and default'}</span></div>\n`
  for (const row of rows) {
    const id = `property-${escape(row.name)}`
    const defaultValue = row.default?.[language] ?? '—'
    const details = row.details?.[language]
    text += `<section class="api-property" id="${id}"><div class="api-property-heading"><span class="api-property-name"><code>${escape(row.name)}</code></span><span class="api-type">${escape(row.type)}</span>`
    if (row.readonly) text += `<span class="api-readonly">${chinese ? '只读' : 'Read only'}</span>`
    text += `</div><div class="api-property-body"><p class="api-description">${escape(row.description?.[language])}</p></div><div class="api-property-values">`
    if (details && details !== 'true / false') {
      const formatted = details.split(/("[^"\n]*"|\b[A-Z]\w*(?:\.\w+)+\b)/g)
        .map((value, index) => index % 2 ? `<code>${escape(value)}</code>` : escape(value).replaceAll('\n', '<br>')).join('')
      text += `<p class="api-details">${formatted}</p>`
    }
    text += `<div class="api-default"><span class="api-default-label">${chinese ? '默认值' : 'Default'}</span>`
    text += defaultValue.length > 80
      ? `<details><summary>${chinese ? '展开默认值' : 'Show default value'}</summary><pre><code>${escape(defaultValue)}</code></pre></details>`
      : `<code class="api-default-value">${escape(defaultValue)}</code>`
    text += '</div></div></section>\n'
  }
  return text + '</div>\n\n'
}

function interfaceReference(entries, kind, language) {
  const unique = new Map(entries.map(entry => [entry.signature.match(/(\w+)\s*\(/)[1], entry]))
  const rows = [...unique.entries()].map(([name, entry]) => ({ name, entry, description: describeInterface(entry, kind, language) })).filter(row => row.description)
  if (!rows.length) return ''
  const chinese = language === 'zh'
  let text = `## ${kind === 'signals' ? chinese ? '信号' : 'Signals' : chinese ? '方法' : 'Methods'}\n\n`
  text += `<div class="api-interfaces" v-pre><div class="api-interface-head"><span>${chinese ? '签名' : 'Signature'}</span><span>${chinese ? '作用' : 'Purpose'}</span></div>\n`
  for (const { name, entry, description } of rows) {
    const parameters = entry.signature.match(/\(([\s\S]*)\)$/)[1]
    const signature = kind === 'signals' ? entry.signature : `${name}(${parameters.split(',').filter(value => value.trim()).map(value => {
      const parameter = value.split('=')[0].trim().match(/(\w+)$/)?.[1]
      assert(parameter, `Cannot read method parameter: ${entry.owner}.${entry.signature}`)
      return parameter
    }).join(', ')})`
    text += `<section class="api-interface" id="${kind === 'signals' ? 'signal' : 'method'}-${name}"><code>${escape(signature)}</code><p>${escape(description)}</p></section>\n`
  }
  return text + '</div>\n\n'
}

function screenshotPage(language) {
  const chinese = language === 'zh'
  const local = value => value.split(' / ')[chinese ? 0 : 1] || value
  let text = chinese ? '# 功能截图\n\n点击图片查看完整的 Gallery 窗口。\n\n' : '# Feature screenshots\n\nOpen an image to view the complete Gallery window.\n\n'
  text += '<div class="screenshot-grid">\n'
  for (const line of read('docs/screenshots/README.md').split('\n')) {
    if (!line.startsWith('|')) continue
    const cells = line.split('|').map(value => value.trim())
    const image = cells[2]?.match(/\]\(([^)]+)-(?:zh|en)\.png\)/)?.[1]
    if (!image) continue
    const target = `/screenshots/${image}-${language}.png`
    const title = local(cells[1])
    text += `<figure>${screenshotMarkup(target, title, chinese ? '..' : '../..')}<figcaption><strong>${escape(title)}</strong><span>${escape(local(cells[3]))}</span></figcaption></figure>\n`
  }
  return text + '</div>\n'
}

function componentPage(item, language) {
  const chinese = language === 'zh'
  const prefix = chinese ? '' : '/en'
  const documentation = api[item.name]
  const sampleList = prepareExamples(item.name, examples(item, language), language)
  const declarations = interfaces(item.name)
  const introduced = item.introducedIn ? `<span class="component-introduced">${chinese ? '引入于' : 'Introduced in'} ${item.introducedIn}</span>` : ''
  let text = `<div class="component-breadcrumb"><a href="./">${chinese ? '组件' : 'Components'}</a><span>/</span><a href="./#${item.group.id}">${item.group[language]}</a>${introduced}</div>\n\n`
  text += `# ${item.name} <span class="component-subtitle">${escape(item[language])}</span>\n\n`
  text += '<div class="component-meta">\n\n`import EUI 1.0`\n\n</div>\n\n'
  text += `## ${chinese ? '示例与代码' : 'Examples'}\n\n`
  for (const sample of sampleList) {
    text += `### ${escape(sample.title)}\n\n<QmlPlayground initial="${item.name}" :examples="${escape(JSON.stringify([sample]))}" />\n\n`
  }
  text += `## ${chinese ? '属性' : 'Properties'}\n\n`
  text += propertyReference(documentation.properties, language)
  for (const kind of ['signals', 'methods']) text += interfaceReference(declarations[kind], kind, language)
  page(`${prefix}/components/${item.name}`, text, 'example/data/component-api.json', { pageClass: 'component-page', outline: [2, 2] })
  return sampleList.length
}

// Only generated output may be replaced; repository documentation remains the source.
assert.equal(output, path.resolve(root, 'website/.generated'))
rmSync(output, { recursive: true, force: true })
mkdirSync(path.join(output, 'public'), { recursive: true })
const runtimeDirectory = path.join(root, 'website/playground/runtime')
for (const file of ['eui-playground.js', 'eui-playground.wasm', 'qtloader.js']) {
  assert(existsSync(path.join(runtimeDirectory, file)), `Missing browser runtime: ${file}`)
  const target = path.join(output, 'public/playground', file)
  mkdirSync(path.dirname(target), { recursive: true })
  copyFileSync(path.join(runtimeDirectory, file), target)
}
const loaderPath = path.join(output, 'public/playground/qtloader.js')
const loaderSource = readFileSync(loaderPath, 'utf8')
const fetchCall = 'return fetch(fullPath).then('
assert(loaderSource.includes(fetchCall), 'Qt loader fetch hook has changed')
const loader = loaderSource.replace(fetchCall, 'return (config.fetch || fetch)(fullPath).then(')
writeFileSync(loaderPath, loader)
const loaderVersion = createHash('sha256').update(loader).digest('hex').slice(0, 12)
writeFileSync(path.join(output, 'public/playground/runtime.html'),
  readFileSync(path.join(root, 'website/playground/runtime.html'), 'utf8').replace('src="qtloader.js"', `src="qtloader.js?v=${loaderVersion}"`))
// Avoid .gz: static servers may decode that suffix before the progress reader sees the bytes.
writeFileSync(path.join(output, 'public/playground/eui-playground.wasm.gzip'),
  gzipSync(readFileSync(path.join(runtimeDirectory, 'eui-playground.wasm')), { level: 9 }))
copyFileSync(path.join(root, 'website/playground/NOTICE.txt'), path.join(output, 'public/playground/NOTICE.txt'))
mkdirSync(path.join(output, 'public/playground/licenses'), { recursive: true })
for (const [file, name] of [
  ['LICENSE', 'EUI-MIT.txt'],
  ['packaging/licenses/Qt-GPL-3.0.txt', 'Qt-GPL-3.0.txt'],
  ['website/.vitepress/theme/fonts/NotoSansSC-OFL.txt', 'NotoSansSC-OFL.txt']
]) copyFileSync(path.join(root, file), path.join(output, 'public/playground/licenses', name))
copyFileSync(path.join(root, 'assets/eui-mark.svg'), path.join(output, 'public/eui-mark.svg'))
const screenshotDirectory = path.join(root, `docs/screenshots/${version}`)
assert(existsSync(screenshotDirectory), `No screenshots for EUI ${version}; refresh them before building the site`)
for (const file of readdirSync(screenshotDirectory)) {
  if (!file.endsWith('.png')) continue
  const target = path.join(output, `public/screenshots/${version}`, file)
  mkdirSync(path.dirname(target), { recursive: true })
  copyFileSync(path.join(screenshotDirectory, file), target)
}

let exampleCount = 0
const missingExamples = []
for (const language of ['zh', 'en']) {
  const chinese = language === 'zh'
  const prefix = chinese ? '' : '/en'
  page(`${prefix}/`, '<DocsHome />', 'website/.vitepress/theme/DocsHome.vue', { layout: 'page', title: chinese ? 'Qt Quick / QML 桌面组件库' : 'Qt Quick / QML desktop components' })
  page(`${prefix}/playground`, chinese
    ? '# QML 在线体验\n\n修改代码，运行并操作真实的 EUI 组件。目前可体验按钮、输入框和浮动标签，也可以组合使用它们。\n\n<QmlPlayground />'
    : '# QML playground\n\nEdit QML, run it, and interact with real EUI components. Try buttons, inputs, and floating labels, or combine them in one example.\n\n<QmlPlayground />',
    'website/playground/main.cpp', { sidebar: false, aside: false, outline: false, pageClass: 'qml-playground-page' })
  const quickStart = chinese
    ? '# 快速开始\n\nEUI 提供 C++ / Qt Quick 和 Python / PySide6 两种实现，共用 QML 组件与接口说明，应用接入方式和运行依赖有所不同。请先[选择版本](/versions)，再按对应指南开始。\n\n## 体验 C++ Gallery\n\n从 [EUI GitHub Releases](https://github.com/lihechuan/EUI/releases) 下载适合系统和架构的 Gallery。安装或完整解压后运行，无需预先安装 Qt SDK。\n\n在 Gallery 中搜索组件，体验交互，并展开或复制示例源码。[了解 Gallery](/guide/gallery)。\n\n## 在 C++ 应用中使用\n\n1. 按[环境准备](/guide/prerequisites)配置 Qt Kit 和编译工具。\n2. 使用与应用一致的 Qt 版本、架构和配置[构建 EUI](/guide/build)。\n3. 跟随[接入指南](/guide/integration)运行完整示例，再将组件接入自己的项目。\n\n```qml\nimport EUI 1.0\nimport QtQuick 2.15\n\nEButton {\n    text: "保存"\n    styleType: EStyle.Primary\n}\n```\n\nPython / PySide6 版的安装、运行和应用接入方法见 [EUI-PySide6 使用指南](/pyside6/guide/getting-started)。需要查询接口时，打开[组件目录](/components/)。'
    : '# Quick start\n\nEUI is available as C++ / Qt Quick and Python / PySide6 implementations. They share the QML components and API descriptions, while application integration and runtime requirements differ. [Choose a version](/en/versions) before following a guide.\n\n## Try the C++ Gallery\n\nDownload a Gallery package for your system and architecture from [EUI GitHub Releases](https://github.com/lihechuan/EUI/releases). Install it or extract the complete portable package. A packaged Gallery does not require a Qt SDK.\n\nSearch for a component, try its interactions, and expand or copy its example source. [Learn about the Gallery](/en/guide/gallery).\n\n## Use EUI in a C++ application\n\n1. Set up a Qt Kit and compiler using the [prerequisites](/en/guide/prerequisites).\n2. [Build EUI](/en/guide/build) with the same Qt version, architecture, and configuration as your application.\n3. Run the complete [integration example](/en/guide/integration), then add the components to your project.\n\n```qml\nimport EUI 1.0\nimport QtQuick 2.15\n\nEButton {\n    text: "Save"\n    styleType: EStyle.Primary\n}\n```\n\nFor Python / PySide6 installation and application integration, see the [EUI-PySide6 usage guide](/en/pyside6/guide/getting-started). Use the [component directory](/en/components/) to look up APIs.'
  page(`${prefix}/guide/getting-started`, editionNavigation(language, '', '/guide/getting-started') + quickStart, 'website/scripts/generate.mjs')
  const pythonGuideFile = `website/content/pyside6/guide.${chinese ? 'zh-CN' : 'en'}.md`
  const pythonQuickStart = read(pythonGuideFile).split(/^## /m)[0].trim().split('\n\n').filter((_, index) => index !== 1).join('\n\n')
  const pythonSteps = chinese
    ? `## 开始使用\n\n当前文档版本：v${pythonVersion}。\n\n1. 按[环境准备](${prefix}/pyside6/guide/prerequisites)安装 Python 和依赖。\n2. 按[安装与运行](${prefix}/pyside6/guide/build)体验 Gallery 或安装 wheel。\n3. 跟随[接入应用](${prefix}/pyside6/guide/integration)初始化 QML 引擎并运行示例。\n\n组件功能与接口见[组件目录](${prefix}/components/)，发行变化见[Python / PySide6 更新记录](${prefix}/pyside6/changelog)。`
    : `## Get started\n\nCurrent documentation version: v${pythonVersion}.\n\n1. Install Python and dependencies using the [prerequisites](${prefix}/pyside6/guide/prerequisites).\n2. Follow [installation and running](${prefix}/pyside6/guide/build) to try the Gallery or install a wheel.\n3. Follow [application integration](${prefix}/pyside6/guide/integration) to initialize the QML engine and run the example.\n\nSee the [component directory](${prefix}/components/) for APIs and the [Python / PySide6 changelog](${prefix}/pyside6/changelog) for release changes.`
  const pythonGuideSource = `${pythonRepository}/blob/${pythonSource.revision}/docs/${path.basename(pythonGuideFile)}`
  page(`${prefix}/pyside6/guide/getting-started`, editionNavigation(language, '/pyside6', '/guide/getting-started') + pythonQuickStart + '\n\n' + pythonSteps, pythonGuideFile, { sourceUrl: pythonGuideSource })
  for (const guide of pythonGuides[language]) page(guide.route, editionNavigation(language, '/pyside6', '/guide/' + guide.route.split('/').at(-1)) + rewriteLinks(guide.text, guide.file, language, '/pyside6'), pythonGuideFile, { sourceUrl: pythonGuideSource })
  const versionPage = chinese
    ? `# 选择版本\n\nEUI 提供两种桌面应用接入方式。两版共用 QML 组件和组件接口说明；宿主语言、工程配置、运行依赖及发行包分别维护。\n\n<div class="version-options">\n<section class="version-option">\n<h2>EUI · C++ / Qt Quick</h2>\n<p>面向 C++ 桌面应用，使用 Qt Quick / QML 和 CMake。当前文档版本：v${version}。</p>\n<ul>\n<li>使用 Qt Kit 和 C++ 编译器构建组件库。</li>\n<li>提供 Windows 和 macOS 桌面 Gallery 安装包，具体架构见发行页面。</li>\n</ul>\n<p class="version-option-actions"><a href="https://github.com/lihechuan/EUI/releases">下载 C++ Gallery</a><a href="./guide/getting-started.html">C++ 使用指南</a><a href="https://github.com/lihechuan/EUI">源码仓库</a></p>\n</section>\n<section class="version-option">\n<h2>EUI-PySide6 · Python / PySide6</h2>\n<p>面向 Python 桌面应用，使用 PySide6 加载 QML。当前发行版本：v${pythonVersion}。</p>\n<ul>\n<li>使用 Python 和 pip 安装组件库，并在 Python 中初始化 QML 引擎。</li>\n<li>提供 Windows x64、macOS Intel 和 Apple Silicon 的安装包与免安装包，以及 Python wheel。</li>\n</ul>\n<p class="version-option-actions"><a href="${pythonRepository}/releases">下载 Python 版</a><a href="./pyside6/guide/getting-started.html">Python 接入指南</a><a href="${pythonRepository}">源码仓库</a></p>\n</section>\n</div>\n\n## 如何选择\n\n已有 C++ / Qt 项目或需要接入 C++ 代码时，选择 EUI。使用 Python 编写桌面应用时，选择 EUI-PySide6。组件的 QML 用法和 Gallery 中的示例保持对应，Python 版不依赖原 EUI C++ 插件。两个仓库的版本号和发行包独立维护。\n\nC++ 版请先阅读[快速开始](/guide/getting-started)，Python 版的具体安装和接入步骤见[EUI-PySide6 使用指南](${prefix}/pyside6/guide/getting-started)。组件功能与接口请查看[组件目录](/components/)；平台下载及当前发行版本以各自发行页面为准。`
    : `# Choose a version\n\nEUI offers two ways to integrate the desktop components. Both versions share the QML components and API descriptions; host language, project setup, runtime dependencies, and release packages are maintained separately.\n\n<div class="version-options">\n<section class="version-option">\n<h2>EUI · C++ / Qt Quick</h2>\n<p>For C++ desktop applications using Qt Quick / QML and CMake. Current documentation version: v${version}.</p>\n<ul>\n<li>Build the component library with a Qt Kit and C++ compiler.</li>\n<li>Desktop Gallery packages are available for Windows and macOS; see the release page for architectures.</li>\n</ul>\n<p class="version-option-actions"><a href="https://github.com/lihechuan/EUI/releases">Download C++ Gallery</a><a href="./guide/getting-started.html">C++ guide</a><a href="https://github.com/lihechuan/EUI">Source repository</a></p>\n</section>\n<section class="version-option">\n<h2>EUI-PySide6 · Python / PySide6</h2>\n<p>For Python desktop applications that load QML through PySide6. Current release: v${pythonVersion}.</p>\n<ul>\n<li>Install the component library with Python and pip, then initialize the QML engine in Python.</li>\n<li>Installer and portable packages are available for Windows x64, macOS Intel, and Apple Silicon, along with a Python wheel.</li>\n</ul>\n<p class="version-option-actions"><a href="${pythonRepository}/releases">Download Python version</a><a href="./pyside6/guide/getting-started.html">Python integration guide</a><a href="${pythonRepository}">Source repository</a></p>\n</section>\n</div>\n\n## Which version should I use?\n\nChoose EUI for an existing C++ / Qt project or when integrating with C++ code. Choose EUI-PySide6 for a Python desktop application. QML component usage and Gallery examples correspond across the versions; the Python edition does not depend on the original EUI C++ plugin. The two repositories maintain their version numbers and release packages independently.\n\nFor the C++ edition, start with the [quick start](/en/guide/getting-started). For Python installation and integration, see the [EUI-PySide6 usage guide](${prefix}/pyside6/guide/getting-started). See the [component directory](/en/components/) for component features and APIs; use each release page for current versions and supported platforms.`
  page(`${prefix}/versions`, versionPage, 'website/scripts/generate.mjs')
  for (const guide of guides[language]) page(guide.route, editionNavigation(language, '', '/guide/' + guide.route.split('/').at(-1)) + rewriteLinks(guide.text, guide.file, language), guide.file)
  page(`${prefix}/guide/screenshots`, screenshotPage(language), 'docs/screenshots/README.md')
  let extensionText = read('docs/primevue-completion.md')
  if (!chinese) extensionText = extensionText.replace(/^(# .+)\n/, '$1\n\n::: info\nThis additional interaction reference is maintained in Chinese. Component properties and examples are available in English in the component directory.\n:::\n')
  page(`${prefix}/guide/extensions`, rewriteLinks(extensionText, 'docs/primevue-completion.md', language), 'docs/primevue-completion.md')
  page(`${prefix}/changelog`, changelogPage(language), 'CHANGELOG.md')
  page(`${prefix}/pyside6/changelog`, changelogPage(language, '/pyside6'), 'website/content/pyside6/CHANGELOG.md', { sourceUrl: `${pythonRepository}/blob/${pythonSource.revision}/CHANGELOG.md` })
  let index = chinese ? '# 组件目录\n\n按 Gallery 分类查找组件，点击名称查看示例、属性及信号和方法。\n\n' : '# Components\n\nBrowse the Gallery categories, then open a component for examples, properties, signals, and methods.\n\n'
  for (const group of groups) {
    index += `## ${group[language]} <span class="group-count">${group.items.filter(item => !item.demo).length}</span> {#${group.id}}\n\n<div class="component-links">\n`
    for (const item of group.items.filter(item => !item.demo)) index += `<a href="./${item.name}.html"><span>${item.name}</span><small>${escape(item[language])}</small></a>\n`
    index += '</div>\n\n'
  }
  page(`${prefix}/components/`, index, 'example/src/ExampleCatalog.cpp', { pageClass: 'component-index' })
  for (const item of components) {
    const count = componentPage(item, language)
    if (language === 'zh') {
      exampleCount += count
      if (!count) missingExamples.push(item.name)
    }
  }
}

write('site.json', JSON.stringify({ version, pythonVersion, groups, componentCount: components.length, exampleCount, missingExamples }, null, 2))
console.log(`Generated EUI ${version}: ${components.length} component pages per language, ${exampleCount} examples, ${guideNames.length} shared guide chapters.`)
if (missingExamples.length) console.log(`Source links provided for components without standalone snippets: ${missingExamples.join(', ')}`)
