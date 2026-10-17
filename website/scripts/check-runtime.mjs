import assert from 'node:assert/strict'
import { readFileSync } from 'node:fs'
import { runInNewContext } from 'node:vm'
import { gzipSync, gunzipSync } from 'node:zlib'

const root = new URL('../', import.meta.url)
const component = readFileSync(new URL('.vitepress/theme/QmlPlayground.vue', root), 'utf8')
  .match(/<script setup>([\s\S]*?)<\/script>/)[1].replace(/^import .+$/gm, '')
const runtime = readFileSync(new URL('playground/runtime.html', root), 'utf8')
  .match(/<script>([\s\S]*?)<\/script>/)[1]

function editor() {
  let now = 0
  let id = 0
  const timers = new Map()
  const child = { postMessage() {} }
  const context = {
    defineProps: () => ({ initial: 'EButton', examples: [] }),
    useData: () => ({ lang: { value: 'zh-CN' }, isDark: { value: false } }),
    ref: value => ({ value }), computed: get => ({ get value() { return get() } }),
    watch() {}, onMounted() {}, onBeforeUnmount() {}, withBase: path => path,
    location: { origin: 'http://localhost' }, TextEncoder, child,
    setTimeout(callback, delay) { timers.set(++id, { callback, due: now + delay }); return id },
    clearTimeout(key) { timers.delete(key) },
    requestAnimationFrame(callback) { callback() },
  }
  runInNewContext(component + '\nframe.value = {contentWindow: child}; run(); globalThis.state = {error, busy, ready, started, status, source, run, indent, restart, receive};', context)
  return {
    ...context.state,
    progress(loaded, total = 100, phase = 'download', origin = 'http://localhost', sender = child) {
      context.state.receive({ source: sender, origin, data: { channel: 'eui-qml', type: 'progress', phase, loaded, total } })
    },
    message(type, data = {}) {
      context.state.receive({ source: child, origin: 'http://localhost', data: { channel: 'eui-qml', type, ...data } })
    },
    advance(milliseconds) {
      now += milliseconds
      for (const [key, timer] of timers) if (timer.due <= now) { timers.delete(key); timer.callback() }
    },
  }
}

const keyboard = editor()
const originalSource = keyboard.source.value
for (const modifier of ['shiftKey', 'ctrlKey', 'metaKey', 'altKey']) {
  let prevented = false
  keyboard.indent({ [modifier]: true, target: { selectionStart: 0, selectionEnd: 0 }, preventDefault() { prevented = true } })
  assert.equal(keyboard.source.value, originalSource, `${modifier}+Tab must not modify the source`)
  assert.equal(prevented, false, `${modifier}+Tab must preserve native keyboard navigation`)
}
let tabPrevented = false
const caret = { selectionStart: 0, selectionEnd: 0 }
keyboard.indent({ target: caret, preventDefault() { tabPrevented = true } })
assert.equal(tabPrevented, true, 'Plain Tab should indent instead of leaving the editor')
assert.equal(keyboard.source.value, '    ' + originalSource)
assert.equal(caret.selectionStart, 4)
assert.equal(caret.selectionEnd, 4)

const slow = editor()
for (let loaded = 1; loaded <= 9; loaded += 1) {
  slow.advance(10000)
  slow.progress(loaded * 10)
  assert.equal(slow.error.value, '', 'Continuing downloads must survive longer than 60 seconds')
}
assert.match(slow.status.value, /90%/)
slow.progress(100, 100, 'compile')
assert.match(slow.status.value, /正在启动/)
slow.message('ready')
slow.message('result', { error: '' })
slow.advance(120000)
assert.equal(slow.ready.value, true)
assert.equal(slow.busy.value, false)
assert.equal(slow.error.value, '')

const stalled = editor()
stalled.progress(25)
stalled.advance(59000)
stalled.progress(25)
stalled.advance(1001)
assert.match(stalled.error.value, /没有进展/)
assert.equal(stalled.started.value, false, 'Stop the stalled iframe and its downloads')
stalled.run()
stalled.progress(75)
assert.equal(stalled.error.value, '', 'Run must create a clean retry after a timeout')
assert.equal(stalled.busy.value, true)

const invalid = editor()
invalid.advance(30000)
invalid.progress(20, 100, 'download', 'https://untrusted.invalid')
invalid.progress(20, 100, 'download', 'http://localhost', {})
invalid.progress(NaN)
invalid.progress(-1)
invalid.progress(101)
invalid.progress(20, -1)
invalid.progress(20, 100, 'unknown')
invalid.advance(30001)
assert.match(invalid.error.value, /没有进展/, 'Untrusted or invalid progress must not extend the deadline')

const unknown = editor()
unknown.progress(1234567, 0)
assert.match(unknown.status.value, /1\.2 MB/)
assert(!unknown.status.value.includes('%'), 'Do not invent percentages without a content length')
unknown.message('failed', { message: 'HTTP 503' })
assert.equal(unknown.error.value, 'HTTP 503')
unknown.advance(60001)
assert.equal(unknown.error.value, 'HTTP 503', 'Preserve the actual failure instead of replacing it with a timeout')

function loader(fetch, decompression = DecompressionStream) {
  const messages = []
  const elements = new Map()
  const context = {
    URLSearchParams, ReadableStream, Response, DecompressionStream: decompression, fetch,
    location: { search: '?lang=en', origin: 'http://localhost' },
    parent: { postMessage: message => messages.push(message) },
    window: { addEventListener() {} }, Module: {}, setTimeout() {},
    document: {
      documentElement: { classList: { toggle() {} } },
      getElementById(id) {
        if (!elements.has(id)) elements.set(id, { style: {}, textContent: '', addEventListener() {}, removeAttribute(name) { delete this[name] } })
        return elements.get(id)
      },
    },
    QtLoader(config) { context.config = config; return { loadEmscriptenModule() {} } },
  }
  runInNewContext(runtime, context)
  return { fetch: context.config.fetch, messages, elements, config: context.config }
}

const wasm = readFileSync(new URL('playground/runtime/eui-playground.wasm', root))
const compressed = readFileSync(new URL('.generated/public/playground/eui-playground.wasm.gzip', root))
assert.deepEqual(gunzipSync(compressed), wasm, 'The deployment artifact must decompress to the current runtime')
assert(compressed.length < wasm.length / 2, 'The compressed runtime should cut the transfer size by at least half')
const generatedLoader = readFileSync(new URL('.generated/public/playground/qtloader.js', root), 'utf8')
assert(generatedLoader.includes('(config.fetch || fetch)(fullPath)'), 'Qt must use the progress-aware fetch hook')
const generatedHtml = readFileSync(new URL('.generated/public/playground/runtime.html', root), 'utf8')
assert.match(generatedHtml, /qtloader\.js\?v=[a-f0-9]{12}/, 'Changed loader code must bypass the previous browser cache')

const payload = Buffer.from([0, 97, 115, 109, 1, 0, 0, 0])
const gzip = gzipSync(payload)
const paths = []
const compressedLoader = loader(async path => {
  paths.push(path)
  return new Response(new ReadableStream({ start(controller) {
    controller.enqueue(gzip.subarray(0, 10))
    controller.enqueue(gzip.subarray(10))
    controller.close()
  } }), { headers: { 'Content-Length': gzip.length } })
})
assert.equal(typeof compressedLoader.fetch, 'function')
const response = await compressedLoader.fetch('eui-playground.wasm')
assert.equal(response.headers.get('Content-Type'), 'application/wasm')
assert.deepEqual(Buffer.from(await response.arrayBuffer()), payload)
assert.deepEqual(paths, ['eui-playground.wasm.gzip'])
const updates = compressedLoader.messages.filter(message => message.type === 'progress')
assert(updates.some(message => message.loaded === 10 && message.total === gzip.length), 'Report real streamed bytes before completion')
assert.equal(updates.at(-1).phase, 'compile')
assert.equal(updates.at(-1).loaded, gzip.length)

const fallbackPaths = []
const fallback = loader(async path => {
  fallbackPaths.push(path)
  return path.endsWith('.gzip') ? new Response('', { status: 404 }) : new Response(payload)
})
assert.deepEqual(Buffer.from(await (await fallback.fetch('eui-playground.wasm')).arrayBuffer()), payload)
assert.deepEqual(fallbackPaths, ['eui-playground.wasm.gzip', 'eui-playground.wasm'])
const legacyPaths = []
const legacy = loader(async path => { legacyPaths.push(path); return new Response(payload) }, null)
assert.deepEqual(Buffer.from(await (await legacy.fetch('eui-playground.wasm')).arrayBuffer()), payload)
assert.deepEqual(legacyPaths, ['eui-playground.wasm'])

const httpFailure = loader(async () => new Response('', { status: 503 }))
assert.equal((await httpFailure.fetch('eui-playground.wasm')).status, 503)
const broken = loader(async () => new Response(new ReadableStream({ start(controller) { controller.error(new Error('Download interrupted')) } })))
await assert.rejects(async () => (await broken.fetch('eui-playground.wasm')).arrayBuffer(), /Download interrupted/)
const corrupt = loader(async () => new Response('invalid gzip'))
await assert.rejects(async () => (await corrupt.fetch('eui-playground.wasm')).arrayBuffer())
compressedLoader.config.showError('HTTP 503')
assert.equal(compressedLoader.messages.at(-1).message, 'HTTP 503')
assert.equal(compressedLoader.elements.get('progress').hidden, true)

console.log(`PASS: keyboard navigation, indentation, slow downloads, inactivity timeout, retries, message validation, streamed gzip, legacy fallback and failures; runtime ${wasm.length} -> ${compressed.length} bytes.`)
