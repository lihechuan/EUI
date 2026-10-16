import DefaultTheme from 'vitepress/theme-without-fonts'
import DocsHome from './DocsHome.vue'
import QmlPlayground from './QmlPlayground.vue'
import './style.css'

export default {
  extends: DefaultTheme,
  enhanceApp({ app }) {
    app.component('DocsHome', DocsHome)
    app.component('QmlPlayground', QmlPlayground)
  }
}
