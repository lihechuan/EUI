import { createHighlighterCore } from 'shiki/core'
import { createOnigurumaEngine } from 'shiki/engine/oniguruma'
import qml from 'shiki/langs/qml.mjs'
import javascript from 'shiki/langs/javascript.mjs'
import light from 'shiki/themes/github-light.mjs'
import dark from 'shiki/themes/github-dark.mjs'

const highlighter = createHighlighterCore({
  themes: [light, dark],
  langs: [qml, javascript],
  engine: createOnigurumaEngine(import('shiki/wasm'))
})

export async function highlight(source, dark) {
  return (await highlighter).codeToHtml(source + '\n', { lang: 'qml', theme: dark ? 'github-dark' : 'github-light' })
}
