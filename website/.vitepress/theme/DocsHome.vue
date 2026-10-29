<script setup>
import { ref, computed } from 'vue'
import { useData, withBase } from 'vitepress'
import site from '../../.generated/site.json'

const { lang } = useData()
const selectedGroupId = ref('basic')
const groupIcons = {
  basic: 'M3 3h6v6H3zM15 3h6v6h-6zM3 15h6v6H3zM15 15h6v6h-6z',
  config: 'M4 7h7m4 0h5M4 17h13m4 0h-1M11 4v6M17 14v6',
  form: 'M4 3h16v18H4zM8 8h8M8 12h8M8 16h4',
  data: 'M3 3v18h18M7 16v-5M12 16V7M17 16v-8',
  navigation: 'M4 4h16v16H4zM4 9h16M9 9v11',
  feedback: 'M4 4h16v12h-9l-5 4v-4H4zM8 8h8M8 12h5',
  other: 'M4 5h16M4 12h7m2 0h7M4 19h16',
  visual: 'M5 5h12v12H5zM9 17v4h12V9h-4',
  extensions: 'M4 4h16v16H4zM8 12h8M12 8v8'
}
const chinese = computed(() => lang.value === 'zh-CN')
const language = computed(() => chinese.value ? 'zh' : 'en')
const prefix = computed(() => chinese.value ? '' : '/en')
const selectedGroup = computed(() => site.groups.find(group => group.id === selectedGroupId.value))
const selectedComponents = computed(() => selectedGroup.value.items.filter(item => !item.demo))
const screenshot = theme => withBase('/screenshots/' + site.version + '/' + (theme === 'dark' ? 'overview-dark' : 'home') + '-' + language.value + '.png')
const url = path => {
  const [page, anchor] = path.split('#')
  return withBase(prefix.value + (page.endsWith('/') ? page : page + '.html') + (anchor ? '#' + anchor : ''))
}
</script>

<template>
  <main class="docs-home">
    <section class="intro" aria-labelledby="intro-title">
      <div class="intro-topline">
        <a :href="url('/changelog')">{{ chinese ? '更新记录' : 'Changelog' }}</a>
      </div>
      <div class="intro-copy">
        <h1 id="intro-title">EUI <span>{{ chinese ? '文档' : 'documentation' }}</span></h1>
        <p class="intro-subtitle">{{ chinese ? '为桌面应用，构建好界面。' : 'Build better desktop interfaces.' }}</p>
        <p class="intro-description">{{ chinese ? '为 Qt Quick 应用准备的组件库。从一个按钮，到完整的工作台，使用统一的组件、主题与交互。' : 'A component library for Qt Quick. From a single button to a complete workspace, with consistent components, themes, and interactions.' }}</p>
        <div class="intro-actions">
          <a class="primary-action" :href="url('/versions')">{{ chinese ? '选择实现版本' : 'Choose an edition' }}</a>
          <a class="secondary-action" :href="url('/guide/getting-started')">{{ chinese ? '开始使用' : 'Get started' }}</a>
          <a class="secondary-action" :href="url('/components/')">{{ chinese ? '浏览组件' : 'Explore components' }}</a>
        </div>
        <p class="intro-note">{{ chinese ? '网页示例的显示效果和部分交互可能与桌面程序不一致。建议下载桌面示例程序，并以桌面程序的实际效果为准。' : 'Browser examples may differ from the desktop app in appearance and interaction. Download the desktop Gallery and use it as the reference for the full component experience.' }}</p>
      </div>
      <div class="intro-art" aria-hidden="true">
        <svg class="brand-structure" viewBox="0 0 1440 660" fill="none" preserveAspectRatio="xMidYMid slice">
          <g class="brand-grid">
            <path d="m-180 460 420-242 420 242-420 242zM-110 420l420 242M-40 380l420 242M30 340l420 242M100 300l420 242M170 260l420 242M-110 500l420-242M-40 540l420-242M30 580l420-242M100 620l420-242M170 660l420-242" />
            <path d="m940 388 310-179 310 179-310 179zM1002 352l310 179M1064 316l310 179M1126 280l310 179M1188 244l310 179M1002 424l310-179M1064 460l310-179M1126 496l310-179M1188 532l310-179" />
          </g>
          <g class="brand-plane">
            <path d="m80 412 160-92 160 92v14l-160 92-160-92zM80 412l160 92 160-92M240 504v14" />
            <path d="m80 374 160-92 160 92-160 92zM80 374v38M240 466v38M400 374v38" />
            <path d="m80 336 160-92 160 92-160 92zM80 336v38M240 428v38M400 336v38" />
          </g>
          <g class="brand-base">
            <path d="m1090 364 160-92 160 92v18l-160 92-160-92zM1090 364l160 92 160-92M1250 456v18" />
            <path d="m1120 326 130-75 130 75-130 75z" />
          </g>
          <g class="brand-emblem">
            <svg x="1140" y="134" width="220" height="220" viewBox="0 0 128 128">
              <path class="brand-edge" d="M16 24 46 8V58L68 45L80 52L46 72V81L64 92L88 78V8L112 22V90L64 118L16 90Z" transform="translate(0 5)" />
              <path fill="#93c5ff" d="M16 24 46 8V58L68 45L80 52L46 72V81L64 92L88 78V8L112 22V90L64 118L16 90Z" />
              <path fill="#c2e4ff" d="M16 24 46 8V58L68 45L80 52L46 72V81L64 92V118L16 90Z" />
              <path fill="#fff" d="M16 24 46 8V30L16 46ZM46 58 68 45 80 52 46 72Z" />
            </svg>
          </g>
          <g class="brand-connections">
            <path d="M240 244v-76l90-52h170M1250 474v64l-70 40H996M80 336l-52-30v-90M1410 364l52-30v-90" />
            <circle cx="500" cy="116" r="4" /><circle cx="996" cy="578" r="4" /><circle cx="240" cy="244" r="4" />
          </g>
          <g class="brand-accents">
            <path d="m80 336 60 35M340 371l60-35M1090 364l60 35M1350 399l60-35" />
            <path d="M344 116h68M1080 578h66" />
          </g>
        </svg>
      </div>
      <div class="intro-foot">
        <a :href="url('/components/')"><strong>{{ site.componentCount }}</strong><span>{{ chinese ? '个组件' : 'components' }}</span></a>
        <a :href="url('/guide/gallery')"><strong>{{ site.exampleCount }}</strong><span>{{ chinese ? '份代码示例' : 'code examples' }}</span></a>
        <a class="intro-gallery" :href="url('/guide/gallery')">{{ chinese ? '体验 Gallery 示例程序' : 'Try the native Gallery' }}<svg viewBox="0 0 24 24" aria-hidden="true"><path d="M5 12h14m-6-6 6 6-6 6" /></svg></a>
      </div>
    </section>

    <div class="home-content">
      <section class="version-selection" aria-labelledby="version-selection-title">
        <div class="section-heading"><div><h2 id="version-selection-title">{{ chinese ? '选择实现版本' : 'Choose an implementation' }}</h2><p>{{ chinese ? '两版共享 QML 组件和接口说明，开发语言、应用接入方式与发行包各自对应。' : 'Both editions share the QML components and API references, with separate host languages, integration guides, and releases.' }}</p></div><a :href="url('/versions')">{{ chinese ? '版本对比' : 'Compare editions' }}<span aria-hidden="true">↗</span></a></div>
        <div class="version-cards">
          <article class="version-card">
            <div class="version-card-heading"><h3>EUI</h3><span>C++ · Qt Quick</span><small>v{{ site.version }}</small></div>
            <p>{{ chinese ? '适用于 C++ 桌面应用，使用 Qt Quick / QML 与 CMake 构建和接入。' : 'For C++ desktop applications, built and integrated with Qt Quick / QML and CMake.' }}</p>
            <div class="version-card-actions"><a class="version-card-primary" href="https://github.com/lihechuan/EUI/releases">{{ chinese ? '下载 C++ Gallery' : 'Download C++ Gallery' }}</a><a :href="url('/guide/getting-started')">{{ chinese ? 'C++ 使用指南' : 'C++ guide' }}<span aria-hidden="true">↗</span></a></div>
          </article>
          <article class="version-card">
            <div class="version-card-heading"><h3>EUI-PySide6</h3><span>Python · PySide6</span><small>v{{ site.pythonVersion }}</small></div>
            <p>{{ chinese ? '适用于 Python 桌面应用。提供 Windows x64、macOS Intel 和 Apple Silicon 安装包、免安装包及 Python wheel。' : 'For Python desktop applications. Installer, portable, and Python wheel packages are available for Windows x64, macOS Intel, and Apple Silicon.' }}</p>
            <div class="version-card-actions"><a class="version-card-primary" href="https://github.com/lihechuan/EUI-PySide6/releases">{{ chinese ? '下载 Python 版' : 'Download Python edition' }}</a><a :href="url('/pyside6/guide/getting-started')">{{ chinese ? 'Python 使用指南' : 'Python guide' }}<span aria-hidden="true">↗</span></a></div>
          </article>
        </div>
      </section>

      <section class="component-directory" aria-labelledby="directory-title">
        <div class="section-heading">
          <div><h2 id="directory-title">{{ chinese ? '组件索引' : 'Component index' }}</h2><p>{{ chinese ? '选择分类，查找组件的用法与接口。' : 'Choose a category to find usage examples and API references.' }}</p></div>
          <a :href="url('/components/')">{{ chinese ? '全部组件' : 'All components' }}<span aria-hidden="true">↗</span></a>
        </div>
        <div class="component-browser">
          <fieldset class="category-selector">
            <legend class="visually-hidden">{{ chinese ? '组件分类' : 'Component categories' }}</legend>
            <label v-for="group in site.groups" :key="group.id" class="category-option">
              <input v-model="selectedGroupId" type="radio" name="component-category" :value="group.id" aria-controls="component-list">
              <span><svg viewBox="0 0 24 24" aria-hidden="true"><path :d="groupIcons[group.id]" /></svg><strong>{{ group[language] }}</strong><small>{{ group.items.filter(item => !item.demo).length }}</small></span>
            </label>
          </fieldset>
          <div class="component-results">
            <div class="component-results-heading">
              <h3 id="selected-group-title">{{ selectedGroup[language] }}<span>{{ selectedComponents.length }}</span></h3>
              <a :href="url('/components/#' + selectedGroup.id)">{{ chinese ? '分类文档' : 'Category reference' }}<span aria-hidden="true">↗</span></a>
            </div>
            <ul id="component-list" class="home-component-list" aria-labelledby="selected-group-title" tabindex="0">
              <li v-for="item in selectedComponents" :key="item.name"><a :href="url('/components/' + item.name)"><strong>{{ item[language] }}</strong><span>{{ item.name }}</span><svg viewBox="0 0 24 24" aria-hidden="true"><path d="M5 12h14m-6-6 6 6-6 6" /></svg></a></li>
            </ul>
          </div>
        </div>
      </section>

      <section class="getting-started" aria-labelledby="start-title">
        <div class="section-heading"><div><h2 id="start-title">{{ chinese ? 'C++ 项目接入' : 'C++ project setup' }}</h2><p>{{ chinese ? '从环境准备到 QML 导入，按步骤完成配置。' : 'Set up your environment, build EUI, and import the QML module.' }}</p></div><a :href="url('/guide/getting-started')">{{ chinese ? '快速开始' : 'Quick start' }}<span aria-hidden="true">↗</span></a></div>
        <nav class="integration-path" :aria-label="chinese ? '接入流程' : 'Integration steps'">
          <a :href="url('/guide/prerequisites')"><span class="step-number" aria-hidden="true">1</span><strong>{{ chinese ? '环境准备' : 'Prerequisites' }}</strong><small>{{ chinese ? '选择 Qt Kit 与编译工具' : 'Choose a Qt Kit and compiler' }}</small></a>
          <a :href="url('/guide/build')"><span class="step-number" aria-hidden="true">2</span><strong>{{ chinese ? '构建 EUI' : 'Build EUI' }}</strong><small>{{ chinese ? '配置、编译与安装组件库' : 'Configure, build, and install EUI' }}</small></a>
          <a :href="url('/guide/integration')"><span class="step-number" aria-hidden="true">3</span><strong>{{ chinese ? '接入应用' : 'Integrate' }}</strong><small>{{ chinese ? '配置 CMake 并导入 QML 模块' : 'Set up CMake and import the QML module' }}</small></a>
        </nav>
      </section>

      <section class="showcase" aria-labelledby="showcase-title">
        <div class="gallery-copy">
          <svg class="gallery-symbol" viewBox="0 0 128 128" aria-hidden="true"><path fill="#255BD8" d="M16 24 46 8V58L68 45L80 52L46 72V81L64 92L88 78V8L112 22V90L64 118L16 90Z" /><path fill="#3A95F0" d="M16 24 46 8V58L68 45L80 52L46 72V81L64 92V118L16 90Z" /><path fill="#69C9FF" d="M16 24 46 8V30L16 46ZM46 58 68 45 80 52 46 72Z" /></svg>
          <h2 id="showcase-title">{{ chinese ? 'Gallery 示例程序' : 'Gallery application' }}</h2>
          <p>{{ chinese ? '在桌面端体验组件交互、切换主题，查看和复制 QML 源码。' : 'Try native interactions and themes, and view or copy the QML source.' }}</p>
          <div class="gallery-links">
            <a class="gallery-download" :href="url('/versions')">{{ chinese ? '选择 Gallery 版本' : 'Choose a Gallery edition' }}<svg viewBox="0 0 24 24" aria-hidden="true"><path d="M12 3v12m-5-5 5 5 5-5M5 16v5h14v-5" /></svg></a>
            <a class="gallery-guide" :href="url('/guide/gallery')">{{ chinese ? '使用指南' : 'User guide' }}</a>
          </div>
        </div>
        <a class="gallery-image" :href="url('/guide/gallery')" :aria-label="chinese ? '查看 Gallery 使用指南' : 'View the Gallery guide'">
          <img v-for="theme in ['light', 'dark']" :key="theme" :data-preview-theme="theme" :src="screenshot(theme)" :alt="chinese ? 'EUI Gallery 原生应用的组件总览' : 'Component overview in the native EUI Gallery'" loading="lazy" width="1440" :height="theme === 'dark' ? 1000 : 1440">
        </a>
      </section>
    </div>
  </main>
</template>
