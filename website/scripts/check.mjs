import assert from 'node:assert/strict'
import { existsSync, readFileSync, readdirSync } from 'node:fs'
import path from 'node:path'
import { fileURLToPath } from 'node:url'
import { createMarkdownRenderer } from 'vitepress'

const source = fileURLToPath(new URL('../.generated/', import.meta.url))
const repository = path.resolve(source, '../..')
const site = JSON.parse(readFileSync(path.join(source, 'site.json')))
const api = JSON.parse(readFileSync(path.join(repository, 'example/data/component-api.json')))
const renderer = await createMarkdownRenderer(source)
const pages = new Map()

function collect(directory) {
  for (const entry of readdirSync(directory, { withFileTypes: true })) {
    const absolute = path.join(directory, entry.name)
    if (entry.isDirectory() && entry.name !== 'public') collect(absolute)
    if (!entry.isFile() || !entry.name.endsWith('.md')) continue
    const relative = path.relative(source, absolute).replaceAll('\\', '/')
    const text = readFileSync(absolute, 'utf8')
    const html = renderer.render(text, { path: absolute, relativePath: relative })
    const url = '/' + relative.replace(/\.md$/, '.html')
    const page = { relative, text, html, ids: new Set([...html.matchAll(/\bid="([^"]+)"/g)].map(match => match[1])) }
    pages.set(url, page)
    if (url.endsWith('/index.html')) pages.set(url.replace(/index\.html$/, ''), page)
  }
}

collect(source)
for (const prefix of ['', '/en']) {
  assert(!pages.has(`${prefix}/guide/deployment.html`), 'Deployment instructions must not be published in the user guide')
  assert.match(pages.get(`${prefix}/guide/troubleshooting.html`).text, /^# 6\. /m, 'Troubleshooting must follow the Gallery chapter')
}
for (const [url, heading, language] of [['/versions.html', '选择版本', 'zh-CN'], ['/en/versions.html', 'Choose a version', 'en']]) {
  const page = pages.get(url)
  assert(page, `Missing version selector: ${url}`)
  assert.match(page.text, new RegExp(`^# ${heading}$`, 'm'))
  assert(page.text.includes('EUI-PySide6') && page.text.includes('C++ / Qt Quick'))
  assert(page.text.includes('https://github.com/lihechuan/EUI/releases'))
  assert(page.text.includes('https://github.com/lihechuan/EUI-PySide6/releases'))
  assert(page.text.includes('Windows x64') && page.text.includes('macOS Intel') && page.text.includes('Apple Silicon'))
  const prefix = language === 'en' ? '/en' : ''
  assert(page.text.includes(`${prefix}/pyside6/guide/getting-started`), 'Python guide must stay on the documentation site in the matching language')
}
const home = readFileSync(path.join(repository, 'website/.vitepress/theme/DocsHome.vue'), 'utf8')
assert(home.includes("url('/pyside6/guide/getting-started')"), 'The Python home card must open the on-site Python guide')
assert(!home.includes("url('/changelog')") && !home.includes("url('/pyside6/changelog')"), 'The home content must not duplicate the navigation changelog entry')
assert(!home.includes('EUI-PySide6/blob/master/docs/guide.'), 'Home guide links must not leave the documentation site')
for (const prefix of ['', '/en']) {
  for (const route of ['/guide/getting-started', '/guide/prerequisites', '/guide/build', '/guide/integration', '/guide/features', '/guide/gallery', '/guide/troubleshooting', '/changelog']) {
    for (const edition of ['', '/pyside6']) {
      const page = pages.get(`${prefix}${edition}${route}.html`)
      assert(page, `Missing edition page: ${prefix}${edition}${route}`)
      assert.match(page.html, /<nav class="edition-switch"/, 'Guides and changelogs must identify the selected edition')
      const navigation = page.html.match(/<nav class="edition-switch"[^>]*>(.*?)<\/nav>/)[1]
      const links = [...navigation.matchAll(/href="([^"]+)"/g)].map(([, target]) => target)
      assert.equal(links.length, 2)
      for (const base of ['/', '/EUI/', '/preview/docs/']) {
        const current = `https://eui.invalid${base}${page.relative.replace(/\.md$/, '.html')}`
        for (const [index, target] of ['', '/pyside6'].entries()) assert.equal(new URL(links[index], current).pathname, `${base.slice(0, -1)}${prefix}${target}${route}.html`, `Edition switching must preserve chapter, language and deployment directory: ${page.relative}`)
      }
      assert(page.html.includes(`aria-current="page">${edition ? 'Python / PySide6' : 'C++ / Qt Quick'}</a>`), `Wrong active edition: ${page.relative}`)
    }
  }
  const pythonStart = pages.get(`${prefix}/pyside6/guide/getting-started.html`).text
  const pythonIntegration = pages.get(`${prefix}/pyside6/guide/integration.html`).text
  const pythonLog = pages.get(`${prefix}/pyside6/changelog.html`).text
  assert(pythonStart.includes('EUI-PySide6') && pythonStart.includes('Python'))
  assert(pythonIntegration.includes('from eui import initialize') && pythonIntegration.includes('initialize(engine,'))
  assert(!pythonIntegration.includes('CMakeLists.txt'), 'Python integration must not show the C++ setup')
  assert(pythonLog.includes('1.3.2') && pythonLog.includes('Python/PySide6'))
  assert(!pythonLog.includes('2026-10-20') && !pythonLog.includes('asynchronous form validation'), 'Python changelog must not reuse C++ release notes or dates')
}
assert(pages.get('/guide/getting-started.html').text.includes('/versions'), 'Chinese quick start must link to version selection')
assert(pages.get('/en/guide/getting-started.html').text.includes('/en/versions'), 'English quick start must link to English version selection')
for (const page of new Set(pages.values())) {
  assert(!/本机验证环境限制|本轮验证结果|保留原有工作区修改|未提交或推送|构建产物保留 14 天|Build artifacts are retained for 14 days/.test(page.text), `Internal maintenance notes in ${page.relative}`)
  for (const [, target] of page.html.matchAll(/<img\b[^>]*\bsrc="([^"]*\/screenshots\/[^"/]+\/[^"/]+-(?:zh|en)\.png)"/g)) {
    if (/-dark-(zh|en)\.png$/.test(target)) continue
    assert(page.html.includes(target.replace(/-(zh|en)\.png$/, '-dark-$1.png')), `Missing dark screenshot in ${page.relative}: ${target}`)
  }
}
const errors = []
let links = 0
for (const [url, page] of pages) {
  if (url.endsWith('/index.html')) continue
  for (const [, target] of page.html.matchAll(/\b(?:href|src)="([^"]+)"/g)) {
    if (/^(?:[\w+.-]+:|\/\/)/.test(target)) continue
    const resolved = new URL(target.replaceAll('&amp;', '&'), `https://eui.invalid${url}`)
    const pathname = decodeURIComponent(resolved.pathname)
    if (/\/screenshots\/.*-(zh|en)\.png$/.test(pathname)) {
      const expected = url.startsWith('/en/') ? 'en' : 'zh'
      if (!pathname.endsWith(`-${expected}.png`)) errors.push(`${page.relative}: screenshot language mismatch ${target}`)
    }
    const targetPage = pages.get(pathname) || pages.get(pathname + '.html')
    if (!targetPage && !existsSync(path.join(source, 'public', pathname))) {
      errors.push(`${page.relative}: missing target ${target}`)
    } else if (targetPage && resolved.hash && !targetPage.ids.has(decodeURIComponent(resolved.hash.slice(1)))) {
      errors.push(`${page.relative}: missing anchor ${target}`)
    }
    links += 1
  }
}

let properties = 0
let examples = 0
let interfaces = 0
for (const item of site.groups.flatMap(group => group.items).filter(item => !item.demo)) {
  for (const prefix of ['', '/en']) {
    const page = pages.get(`${prefix}/components/${item.name}.html`)
    assert(page, `Missing component page: ${prefix}/${item.name}`)
    const breadcrumb = page.html.match(/<div class="component-breadcrumb">([\s\S]*?)<\/div>/)?.[1]
    assert(breadcrumb, `Missing component breadcrumb: ${prefix}/${item.name}`)
    const targets = [...breadcrumb.matchAll(/\bhref="([^"]+)"/g)].map(([, target]) => target)
    assert.equal(targets.length, 2, `Incomplete component breadcrumb: ${prefix}/${item.name}`)
    for (const base of ['/', '/EUI/', '/preview/docs/']) {
      const deployed = `https://eui.invalid${base}${page.relative.replace(/\.md$/, '.html')}`
      for (const target of targets) {
        assert.equal(new URL(target, deployed).pathname, `${base.slice(0, -1)}${prefix}/components/`, `Component breadcrumb leaves the deployed directory: ${page.relative} (${base})`)
      }
    }
    assert(!page.html.includes('component-source-links') && !/组件源码|示例源码|Component source|Example source/.test(page.text), `Source links left in ${prefix}/${item.name}`)
    assert(page.text.includes(`<QmlPlayground initial="${item.name}" :examples=`), `Missing component playground: ${prefix}/${item.name}`)
    assert(!page.html.includes('class="component-demo') && !/View full image|查看原图/.test(page.text), `Static preview left in ${prefix}/${item.name}`)
    assert(!page.html.includes('/screenshots/'), `Component screenshot left in ${prefix}/${item.name}`)
    const panels = [...page.text.matchAll(/:examples="([^"]+)"/g)].map(([, serialized]) => JSON.parse(serialized.replaceAll('&quot;', '"').replaceAll('&lt;', '<').replaceAll('&gt;', '>').replaceAll('&amp;', '&')))
    assert(panels.every(samples => samples.length === 1), `Each example must have its own editor and preview: ${prefix}/${item.name}`)
    const cases = panels.flat()
    assert(cases.length > 0, `No examples: ${prefix}/${item.name}`)
    assert(cases.some(sample => sample.code.includes(item.name)), `Wrong component examples: ${prefix}/${item.name}`)
    assert(cases.every(sample => sample.title && sample.code.startsWith('import EUI 1.0')), `Incomplete example: ${prefix}/${item.name}`)
    examples += cases.length
    assert(!/点击签名|Follow a signature|查看实现/.test(page.text), `Implementation instructions in ${prefix}/${item.name}`)
    for (const [, content] of page.html.matchAll(/<section class="api-interface"[^>]*>([\s\S]*?)<\/section>/g)) {
      assert(!content.includes('<a '), `Interface signatures must not link away: ${prefix}/${item.name}`)
      assert.match(content, /^<code>.+<\/code><p>.+<\/p>$/, `Missing interface purpose: ${prefix}/${item.name}`)
      if (prefix) assert(!/\p{Script=Han}/u.test(content), `Chinese interface description on English page: ${item.name}`)
      interfaces += 1
    }
    assert.equal((page.html.match(/id="property-/g) || []).length, api[item.name].properties.length, `Incomplete API: ${item.name}`)
    properties += api[item.name].properties.length
  }
}
assert.deepEqual(errors, [], errors.join('\n'))
assert.equal(site.missingExamples.length, 0, 'Every component should have a code example')
assert(pages.get('/guide/features.html').text.includes('EAnimation.play'))
assert(pages.get('/en/guide/integration.html').text.includes('CMakeLists.txt'))
assert(pages.get('/components/ETaskBoard.html').text.includes('taskMoved('))
assert(pages.get('/components/EAnimation.html').text.includes('play(target, effect, options)'))
assert(pages.get('/components/EList.html').text.includes('page 为新的页码，从 1 开始'))
assert(pages.get('/components/EList.html').text.includes('禁用或页码无效时返回 false'))
assert(!pages.get('/components/EFlex.html').text.includes('arrange()'), 'Internal layout helpers must not be published as public methods')
assert(pages.get('/components/ETable.html').text.includes('method-getSelectionRows'))
assert(pages.get('/components/ETree.html').text.includes('method-setCheckedKeys'))
assert(pages.get('/components/EMessage.html').text.includes('open(message, key, duration)'), 'Every public parameter must be preserved, including parameters after nested default expressions')
assert(!pages.get('/components/EMessage.html').text.includes('RandomUtils.nanoid()'), 'Internal default expressions must not appear in method signatures')
const changelog = readFileSync(path.join(repository, 'CHANGELOG.md'), 'utf8').replace(/\r\n/g, '\n')
const chineseChangelog = pages.get('/changelog.html').text
const englishChangelog = pages.get('/en/changelog.html').text
assert.match(chineseChangelog, /^# 更新记录$/m, 'Chinese changelog must have a Chinese title')
assert.match(englishChangelog, /^# Changelog$/m, 'English changelog must have an English title')
assert(!chineseChangelog.includes('English:'), 'Chinese changelog must not include English release notes')
assert(!chineseChangelog.includes('Features, fixes, and compatibility changes'), 'Chinese changelog must not include the English introduction')
assert(!/\p{Script=Han}/u.test(englishChangelog), 'English changelog must not include Chinese text')
for (const text of [chineseChangelog, englishChangelog]) {
  assert.deepEqual(text.match(/^## .+$/gm), changelog.match(/^## .+$/gm), 'Every changelog version and date must be preserved')
}
assert.equal((chineseChangelog.match(/^(?:\d+\. |[-*] )/gm) || []).length, changelog.match(/^(?:\d+\. |[-*] )/gm).length, 'Every Chinese release note must be preserved')
for (const [, text] of changelog.matchAll(/^English: (.+)$/gm)) assert(englishChangelog.includes(text), 'Every English release note must be preserved')
console.log(`PASS: ${new Set(pages.values()).size} pages, ${links} internal links, ${properties} localized properties, ${examples} independent examples, ${interfaces} described interfaces, matching screenshot languages and key interfaces.`)
