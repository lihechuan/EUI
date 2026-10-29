import { readFileSync } from 'node:fs'
import { defineConfig } from 'vitepress'

const { groups } = JSON.parse(readFileSync(new URL('../.generated/site.json', import.meta.url)))
const base = process.env.EUI_DOCS_BASE || '/EUI/'
if (!base.startsWith('/') || !base.endsWith('/')) throw new Error('EUI_DOCS_BASE must start and end with /')

function navigation(language) {
  const chinese = language === 'zh'
  const prefix = chinese ? '' : '/en'
  return {
    label: chinese ? '简体中文' : 'English',
    lang: chinese ? 'zh-CN' : 'en',
    description: chinese ? 'EUI Qt Quick / QML 桌面组件库文档' : 'Documentation for the EUI Qt Quick / QML desktop component library',
    themeConfig: {
      nav: [
        { text: chinese ? '指南' : 'Guide', link: `${prefix}/guide/getting-started` },
        { text: chinese ? '组件' : 'Components', link: `${prefix}/components/` },
        { text: chinese ? '更新记录' : 'Changelog', link: `${prefix}/changelog` },
        { text: chinese ? '版本下载' : 'Versions', link: `${prefix}/versions` }
      ],
      sidebar: {
        ...Object.fromEntries(['', '/pyside6'].map(edition => [`${prefix}${edition}/guide/`, [
          {
            text: edition ? 'Python / PySide6' : 'C++ / Qt Quick',
            items: [
              { text: chinese ? '选择版本' : 'Choose a version', link: `${prefix}/versions` },
              { text: chinese ? '快速开始' : 'Quick start', link: `${prefix}${edition}/guide/getting-started` },
              { text: chinese ? '环境准备' : 'Prerequisites', link: `${prefix}${edition}/guide/prerequisites` },
              { text: edition ? (chinese ? '安装与运行' : 'Install and run') : (chinese ? '构建 EUI' : 'Build EUI'), link: `${prefix}${edition}/guide/build` },
              { text: chinese ? '接入应用' : 'Application integration', link: `${prefix}${edition}/guide/integration` }
            ]
          },
          {
            text: chinese ? '开发指南' : 'Development',
            items: [
              { text: chinese ? '主题、表单与常用功能' : 'Themes, forms, and common features', link: `${prefix}${edition}/guide/features` },
              { text: chinese ? '使用 Gallery' : 'Use the Gallery', link: `${prefix}${edition}/guide/gallery` },
              { text: chinese ? '常见问题' : 'Troubleshooting', link: `${prefix}${edition}/guide/troubleshooting` }
            ]
          }
        ]])),
        [`${prefix}/components/`]: [
          { text: chinese ? '全部组件' : 'All components', link: `${prefix}/components/` },
          ...groups.map(group => ({
            text: group[language],
            collapsed: true,
            items: group.items.filter(item => !item.demo).map(item => ({
              text: `<span class="sidebar-code">${item.name}</span><span class="sidebar-label">${item[language]}</span>`,
              link: `${prefix}/components/${item.name}`
            }))
          }))
        ]
      },
      outline: { level: [2, 3], label: chinese ? '本页目录' : 'On this page' },
      docFooter: { prev: chinese ? '上一页' : 'Previous page', next: chinese ? '下一页' : 'Next page' },
      darkModeSwitchLabel: chinese ? '外观' : 'Appearance',
      darkModeSwitchTitle: chinese ? '切换到深色模式' : 'Switch to dark theme',
      lightModeSwitchTitle: chinese ? '切换到浅色模式' : 'Switch to light theme',
      langMenuLabel: chinese ? '切换语言' : 'Change language',
      sidebarMenuLabel: chinese ? '菜单' : 'Menu',
      returnToTopLabel: chinese ? '返回顶部' : 'Return to top',
      editLink: {
        text: chinese ? '查看文档来源' : 'View documentation source',
        pattern: ({ frontmatter }) => frontmatter.sourceUrl || `https://github.com/lihechuan/EUI/blob/master/${frontmatter.source || 'website/README.md'}`
      },
      footer: {
        message: chinese ? '基于 MIT 许可发布 · 第三方资源遵循其各自许可' : 'Released under the MIT License. Third-party assets retain their own licenses.',
        copyright: 'EUI · Qt Quick / QML'
      }
    }
  }
}

export default defineConfig({
  title: 'EUI',
  base,
  srcDir: '.generated',
  cleanUrls: false,
  lastUpdated: false,
  locales: { root: navigation('zh'), en: navigation('en') },
  head: [['link', { rel: 'icon', type: 'image/svg+xml', href: `${base}eui-mark.svg` }]],
  markdown: {
    theme: { light: 'github-light', dark: 'github-dark' },
    languages: ['qml', 'powershell', 'cmake', 'cpp']
  },
  themeConfig: {
    logo: '/eui-mark.svg',
    socialLinks: [{ icon: 'github', link: 'https://github.com/lihechuan/EUI' }],
    search: {
      provider: 'local',
      options: {
        miniSearch: {
          options: {
            tokenize: text => [...new Intl.Segmenter('zh', { granularity: 'word' }).segment(text)]
              .filter(part => part.isWordLike).map(part => part.segment)
          }
        },
        locales: {
          root: {
            translations: {
              button: { buttonText: '搜索文档', buttonAriaLabel: '搜索文档' },
              modal: {
                noResultsText: '没有找到相关内容',
                resetButtonTitle: '清除搜索',
                footer: { selectText: '选择', navigateText: '切换', closeText: '关闭' }
              }
            }
          }
        }
      }
    }
  }
})
