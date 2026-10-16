<script setup>
import { computed, onBeforeUnmount, onMounted, ref, watch } from 'vue'
import { useData, withBase } from 'vitepress'

const props = defineProps({
  initial: { type: String, default: 'EButton' },
  examples: { type: Array, default: () => [] },
})
const { lang, isDark } = useData()
const chinese = computed(() => lang.value === 'zh-CN')
const frame = ref(null)
const embedded = computed(() => props.examples.length > 0)
const currentExample = computed(() => props.examples[0])
const copied = ref(false)
const source = ref('')
const highlighted = ref('')
const codeLayer = ref(null)
const started = ref(false)
const ready = ref(false)
const busy = ref(false)
const error = ref('')
const logs = ref([])
const revision = ref(0)
const initialDark = ref(isDark.value)
const initialLanguage = ref(chinese.value ? 'zh' : 'en')
let observer
let copyTimeout
let timeout
let highlightCode
let highlightRevision = 0
const frameUrl = computed(() => withBase('/playground/runtime.html') + `?lang=${initialLanguage.value}&dark=${initialDark.value ? 1 : 0}&v=${revision.value}`)
const labels = computed(() => chinese.value ? {
  run: '运行', running: '正在运行…',
  copy: '复制代码', copied: '已复制', desktop: '在 Gallery 中体验',
  reset: '恢复示例', restart: '重启预览', code: 'QML 代码', preview: '效果', console: '输出',
  idle: '运行代码，体验组件交互', hint: '可修改文字、属性和事件，再点击运行。',
  loading: '首次运行需要加载组件运行器…', rendered: '已运行', failed: '运行失败',
  loadError: '运行器加载超时，请检查网络后重启预览。',
} : {
  run: 'Run', running: 'Running…',
  copy: 'Copy code', copied: 'Copied', desktop: 'Try in the Gallery',
  reset: 'Reset example', restart: 'Restart preview', code: 'QML source', preview: 'Preview', console: 'Output',
  idle: 'Run the code to try the component', hint: 'Edit text, properties, or events, then select Run.',
  loading: 'Loading the component runtime for the first run…', rendered: 'Running', failed: 'Run failed',
  loadError: 'Runtime loading timed out. Check your connection and restart the preview.',
})
const status = computed(() => busy.value ? labels.value.loading : error.value ? labels.value.failed : ready.value ? labels.value.rendered : '')

function example(name) {
  const zh = chinese.value
  const imports = 'import EUI 1.0\nimport QtQuick 2.15\n\n'
  if (name === 'EInput') return imports + `Column {
    spacing: 16
    anchors.centerIn: parent

    EInput {
        id: field
        width: 260
        clearable: true
        placeholderText: "${zh ? '请输入姓名' : 'Enter your name'}"
    }
    EText {
        text: field.text.length ? "${zh ? '你好，' : 'Hello, '}" + field.text : "${zh ? '在输入框中输入文字' : 'Type in the field above'}"
    }
}`
  if (name === 'EFloatLabel') return imports + `Column {
    spacing: 20
    anchors.centerIn: parent

    EFloatLabel {
        width: 260
        text: "${zh ? '姓名' : 'Name'}"

        EInput {
            id: nameField
            width: parent.width
            clearable: true
        }
    }
    EText {
        text: nameField.text.length ? "${zh ? '你好，' : 'Hello, '}" + nameField.text : "${zh ? '点击输入框，标签会浮起' : 'Focus the input to raise its label'}"
    }
}`
  return imports + `Column {
    property int count: 0

    spacing: 18
    anchors.centerIn: parent

    EText {
        font.pixelSize: 20
        text: "${zh ? '点击次数：' : 'Clicks: '}" + parent.count
    }
    EButton {
        text: "${zh ? '点击加一' : 'Add one'}"
        styleType: EStyle.Primary

        onClicked: parent.count += 1
    }
}`
}

function reset() {
  source.value = currentExample.value?.code || example(props.initial)
  error.value = ''
  logs.value = []
}

function send(type = 'run') {
  frame.value?.contentWindow?.postMessage({channel: 'eui-qml', type, code: source.value, dark: isDark.value, language: chinese.value ? 'zh' : 'en'}, location.origin)
}

function run() {
  if (currentExample.value?.reason) return
  error.value = ''
  logs.value = []
  if (!source.value.trim()) {
    error.value = chinese.value ? '请输入 QML 代码。' : 'Enter some QML code.'
    return
  }
  if (new TextEncoder().encode(source.value).length > 65536) {
    error.value = chinese.value ? '代码不能超过 64 KB。' : 'QML source must be smaller than 64 KB.'
    return
  }
  if (ready.value) send()
  else {
    initialDark.value = isDark.value
    initialLanguage.value = chinese.value ? 'zh' : 'en'
    started.value = true
    busy.value = true
    clearTimeout(timeout)
    timeout = setTimeout(() => { busy.value = false; error.value = labels.value.loadError }, 60000)
  }
}

async function copy() {
  try {
    await navigator.clipboard.writeText(source.value)
    copied.value = true
    clearTimeout(copyTimeout)
    copyTimeout = setTimeout(() => { copied.value = false }, 1800)
  } catch {
    error.value = chinese.value ? '复制失败，请选中代码后复制。' : 'Select the source and copy it manually.'
  }
}

function restart() {
  clearTimeout(timeout)
  ready.value = false
  started.value = false
  busy.value = false
  revision.value += 1
  run()
}

function receive(event) {
  if (event.source !== frame.value?.contentWindow || event.origin !== location.origin || event.data?.channel !== 'eui-qml') return
  const data = event.data
  if (data.type === 'ready') {
    clearTimeout(timeout)
    ready.value = true
    send()
  } else if (data.type === 'result') {
    busy.value = false
    error.value = data.error || ''
  } else if (data.type === 'failed') {
    clearTimeout(timeout)
    busy.value = false
    ready.value = false
    error.value = data.message
  } else if (data.type === 'log' && typeof data.message === 'string') {
    logs.value = [...logs.value.slice(-49), data.message.slice(0, 2000)]
  }
}

function indent(event) {
  const textarea = event.target
  const start = textarea.selectionStart
  const end = textarea.selectionEnd
  source.value = source.value.slice(0, start) + '    ' + source.value.slice(end)
  requestAnimationFrame(() => { textarea.selectionStart = textarea.selectionEnd = start + 4 })
}

function scrollCode(event) {
  if (!codeLayer.value) return
  codeLayer.value.style.setProperty('--qml-scroll-y', `${-event.target.scrollTop}px`)
  codeLayer.value.style.setProperty('--qml-scroll-x', `${-event.target.scrollLeft}px`)
}

async function updateHighlight() {
  if (!highlightCode) return
  const current = ++highlightRevision
  const html = await highlightCode(source.value, isDark.value)
  if (current === highlightRevision) highlighted.value = html
}

reset()
watch([source, isDark], updateHighlight)
watch(() => props.examples, () => { reset(); if (started.value) run() })
watch(isDark, () => { if (ready.value) send('appearance') })
watch(() => props.initial, () => { reset(); if (started.value) run() })
watch(chinese, () => { reset(); if (ready.value) send() })
onMounted(async () => {
  window.addEventListener('message', receive)
  if (embedded.value && !currentExample.value?.reason) {
    observer = new IntersectionObserver(entries => {
      if (entries.some(entry => entry.isIntersecting)) run()
      else {
        clearTimeout(timeout)
        ready.value = false
        started.value = false
        busy.value = false
        revision.value += 1
      }
    })
    observer.observe(codeLayer.value.closest('.qml-playground'))
  }
  try {
    const module = await import('./qml-highlight.mjs')
    highlightCode = module.highlight
    await updateHighlight()
  } catch {
    highlighted.value = ''
  }
})
onBeforeUnmount(() => { clearTimeout(timeout); clearTimeout(copyTimeout); observer?.disconnect(); window.removeEventListener('message', receive) })
</script>

<template>
  <div class="qml-playground" :class="{ 'qml-embedded': embedded, 'qml-desktop-only': currentExample?.reason }">
    <div class="qml-toolbar">
      <div class="qml-actions"><button type="button" @click="copy">{{ copied ? labels.copied : labels.copy }}</button><button type="button" @click="reset">{{ labels.reset }}</button><button v-if="!currentExample?.reason" type="button" class="qml-run" :disabled="busy" @click="run">{{ busy ? labels.running : labels.run }}<kbd>Ctrl ↵</kbd></button></div>
    </div>
    <div v-if="currentExample?.reason" class="qml-native-note"><p>{{ currentExample.reason }}</p><a :href="withBase(chinese ? '/guide/gallery' : '/en/guide/gallery')">{{ labels.desktop }} ↗</a></div>
    <div class="qml-workspace-scroll"><div class="qml-workspace">
      <div class="qml-editor"><div class="qml-pane-heading">{{ labels.code }}<span>QML</span></div><div class="qml-editor-body" :class="{ highlighted }"><div ref="codeLayer" class="qml-highlight" aria-hidden="true" v-html="highlighted" /><textarea v-model="source" :aria-label="labels.code" spellcheck="false" autocapitalize="off" autocomplete="off" autocorrect="off" @scroll="scrollCode" @keydown.tab.prevent="indent" @keydown.ctrl.enter.prevent="run" @keydown.meta.enter.prevent="run" /></div></div>
      <div v-if="!currentExample?.reason" class="qml-preview"><div class="qml-pane-heading">{{ labels.preview }}<button v-if="started" type="button" @click="restart">{{ labels.restart }}</button></div><iframe v-if="started" :key="revision" ref="frame" :src="frameUrl" :title="labels.preview" sandbox="allow-scripts allow-same-origin" /><div v-else class="qml-empty"><svg viewBox="0 0 32 32" aria-hidden="true"><path d="m12 8 12 8-12 8z" /></svg><strong>{{ labels.idle }}</strong><p>{{ labels.hint }}</p><button type="button" @click="run">{{ labels.run }}</button></div></div>
    </div></div>
    <div v-if="!currentExample?.reason" class="qml-status" role="status" aria-live="polite"><span :class="{ 'qml-error-label': error }">{{ status }}</span><span>{{ chinese ? '代码在当前浏览器中运行' : 'Code runs in this browser' }} <a :href="withBase('/playground/NOTICE.txt')" target="_blank" rel="noreferrer">{{ chinese ? '许可' : 'Licenses' }}</a></span></div>
    <pre v-if="error" class="qml-error" role="alert">{{ error }}</pre>
    <details v-if="logs.length" class="qml-console" open><summary>{{ labels.console }}</summary><pre>{{ logs.join('\n') }}</pre></details>
  </div>
</template>

<style scoped>
.qml-playground { container-type: inline-size; margin: 24px 0; border: 1px solid var(--vp-c-divider); border-radius: 6px; overflow: hidden; }
.qml-toolbar, .qml-actions { display: flex; align-items: center; gap: 12px; }
.qml-toolbar { justify-content: flex-end; padding: 10px 16px; background: var(--vp-c-bg); border-bottom: 1px solid var(--vp-c-divider); }
.qml-actions button { font-size: 12px; color: var(--vp-c-text-2); }
button { cursor: pointer; font: inherit; }
.qml-actions .qml-run, .qml-empty button { background: #255bd8; color: white; border-radius: 4px; padding: 6px 14px; font-size: 13px; }
.qml-run:disabled { opacity: .6; cursor: wait; }
.qml-run kbd { margin-left: 14px; opacity: .7; font: inherit; font-size: 11px; }
.qml-workspace-scroll { overflow-x: auto; }
.qml-workspace { display: grid; min-width: 680px; grid-template-columns: minmax(0, 1fr) minmax(0, 1fr); }
.qml-pane-heading { height: 40px; padding: 0 16px; display: flex; align-items: center; justify-content: space-between; font-size: 12px; color: var(--vp-c-text-2); border-bottom: 1px solid var(--vp-c-divider); }
.qml-pane-heading span { font-size: 11px; }
.qml-pane-heading button { font-size: 12px; color: var(--vp-c-text-2); }
.qml-editor { min-width: 0; border-right: 1px solid var(--vp-c-divider); background: var(--vp-code-block-bg); }
.qml-editor-body { position: relative; height: 380px; }
.qml-editor textarea, .qml-highlight :deep(pre) { font: 12px/1.85 var(--vp-font-family-mono); font-variant-ligatures: none; tab-size: 4; }
.qml-editor textarea { position: absolute; inset: 0; display: block; width: 100%; height: 100%; resize: none; padding: 20px; color: var(--vp-c-text-1); background: transparent; border: 0; outline-offset: -2px; white-space: pre; overflow-wrap: normal; }
.qml-editor-body.highlighted textarea { color: transparent; -webkit-text-fill-color: transparent; caret-color: var(--vp-c-text-1); }
.qml-editor-body textarea::selection { background: var(--vp-c-brand-soft); }
.qml-highlight { position: absolute; inset: 0; overflow: hidden; pointer-events: none; }
.qml-highlight :deep(pre) { margin: 0; padding: 20px; width: max-content; min-width: 100%; border-radius: 0; background: transparent !important; transform: translate(var(--qml-scroll-x, 0px), var(--qml-scroll-y, 0px)); }
.qml-highlight :deep(code) { padding: 0; background: none; font: inherit; display: block; }
.qml-preview { min-width: 0; display: flex; flex-direction: column; }
.qml-preview iframe { display: block; width: 100%; height: 380px; border: 0; }
.qml-empty { flex: 1; min-height: 380px; display: flex; flex-direction: column; align-items: center; justify-content: center; text-align: center; padding: 24px; gap: 12px; }
.qml-empty svg { width: 32px; fill: none; stroke: var(--vp-c-brand-1); stroke-width: 1.5; }
.qml-empty strong { font-weight: 500; font-size: 14px; }
.qml-empty p { margin: 0; font-size: 12px; color: var(--vp-c-text-2); }
.qml-status { display: flex; gap: 16px; justify-content: space-between; min-height: 36px; align-items: center; padding: 6px 16px; border-top: 1px solid var(--vp-c-divider); font-size: 11px; color: var(--vp-c-text-2); }
.qml-status a { margin-left: 8px; font-size: inherit; color: inherit; }
.qml-error-label { color: var(--vp-c-danger-1); }
.qml-error, .qml-console pre { margin: 0; padding: 12px 16px; font: 12px/1.7 var(--vp-font-family-mono); white-space: pre-wrap; overflow-wrap: anywhere; }
.qml-error { color: var(--vp-c-danger-1); background: var(--vp-c-danger-soft); }
.qml-console { border-top: 1px solid var(--vp-c-divider); }
.qml-console summary { padding: 8px 16px; font-size: 12px; cursor: pointer; color: var(--vp-c-text-2); }
button:focus-visible, textarea:focus-visible { outline: 2px solid var(--vp-c-brand-1); outline-offset: 2px; }
.qml-desktop-only .qml-workspace { min-width: 0; grid-template-columns: minmax(0, 1fr); }
.qml-desktop-only .qml-editor { border-right: 0; }
.qml-native-note { padding: 16px 20px; font-size: 13px; color: var(--vp-c-text-2); }
.qml-native-note p { margin: 0 0 8px; }
.qml-native-note a { color: var(--vp-c-brand-1); }
@container (max-width: 700px) { .qml-run kbd { display: none; } }
@media (max-width: 560px) { .qml-toolbar { padding: 10px 12px; } .qml-status { flex-wrap: wrap; gap: 4px; } }
</style>
