import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EConfigProvider"

        ExampleCase {
            objectName: "showcaseEConfigProvider0"
            title: ELocale.texts.InheritedSizeAndDisabledState
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EConfigProvider {
                    disabled: true
                    size: EStyle.Small
                    width: parent.width
                    height: content.implicitHeight

                    Column {
                        id: content

                        spacing: 12
                        width: parent.width

                        EButton {
                            text: "项目操作"
                        }
                        EInput {
                            text: "继承禁用状态"
                            width: Math.min(480, parent.width)
                        }
                        ESelect {
                            model: ["QML", "Qt"]
                            width: Math.min(480, parent.width)
                        }
                    }
                }
            `)

            EConfigProvider {
                disabled: true
                size: EStyle.Small
                width: parent.width
                height: content.implicitHeight

                Column {
                    id: content

                    spacing: 12
                    width: parent.width

                    EButton {
                        text: "项目操作"
                    }
                    EInput {
                        text: "继承禁用状态"
                        width: Math.min(480, parent.width)
                    }
                    ESelect {
                        model: ["QML", "Qt"]
                        width: Math.min(480, parent.width)
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EConfigProvider"

        ExampleCase {
            objectName: "showcaseEConfigProvider1"
            title: ELocale.texts.LocalComponentDefaults
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EConfigProvider {
                    width: parent.width
                    card: ({shadow: "never"})
                    height: content.implicitHeight
                    button: ({plain: true, round: true, type: "primary"})

                    Column {
                        id: content

                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "局部按钮默认值"
                        }
                        ECard {
                            header: "局部卡片"
                            width: Math.min(480, parent.width)

                            EText {
                                text: "配置仅作用于当前子树。"
                                width: parent.width
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                }
            `)

            EConfigProvider {
                width: parent.width
                card: ({shadow: "never"})
                height: content.implicitHeight
                button: ({plain: true, round: true, type: "primary"})

                Column {
                    id: content

                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "局部按钮默认值"
                    }
                    ECard {
                        header: "局部卡片"
                        width: Math.min(480, parent.width)

                        EText {
                            text: "配置仅作用于当前子树。"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EConfigProvider"

        ExampleCase {
            title: ELocale.texts.LocalLanguage
            objectName: "showcaseEConfigProvider2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EConfigProvider {
                    width: parent.width
                    height: content.implicitHeight
                    locale: ({name: "en", texts: {NoData: "No projects yet"}})

                    Column {
                        id: content

                        width: parent.width

                        EEmpty {
                            width: parent.width
                        }
                    }
                }
            `)

            EConfigProvider {
                width: parent.width
                height: content.implicitHeight
                locale: ({name: "en", texts: {NoData: "No projects yet"}})

                Column {
                    id: content

                    width: parent.width

                    EEmpty {
                        width: parent.width
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ELink"

        ExampleCase {
            objectName: "showcaseELink0"
            title: ELocale.texts.SemanticLinks
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ELink {
                        text: "查看文档"
                        styleType: EStyle.Primary
                    }
                    ELink {
                        text: "查看文档"
                        styleType: EStyle.Success
                    }
                    ELink {
                        text: "查看文档"
                        styleType: EStyle.Warning
                    }
                    ELink {
                        text: "查看文档"
                        styleType: EStyle.Danger
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ELink {
                    text: "查看文档"
                    styleType: EStyle.Primary
                }
                ELink {
                    text: "查看文档"
                    styleType: EStyle.Success
                }
                ELink {
                    text: "查看文档"
                    styleType: EStyle.Warning
                }
                ELink {
                    text: "查看文档"
                    styleType: EStyle.Danger
                }
            }
        }
    }
    ExampleSection {
        componentName: "ELink"

        ExampleCase {
            objectName: "showcaseELink1"
            title: ELocale.texts.DisabledLinks
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ELink {
                        text: "项目详情"
                        enabled: true
                        underline: true
                    }
                    ELink {
                        text: "项目详情"
                        enabled: false
                        underline: true
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ELink {
                    text: "项目详情"
                    enabled: true
                    underline: true
                }
                ELink {
                    text: "项目详情"
                    enabled: false
                    underline: true
                }
            }
        }
    }
    ExampleSection {
        componentName: "ELink"

        ExampleCase {
            objectName: "showcaseELink2"
            title: ELocale.texts.IconLink
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ELink {
                    text: "打开设置"
                    leftIcon: true
                    iconSource: EIcons.PENCIL
                    styleType: EStyle.Primary
                }
            `)

            ELink {
                text: "打开设置"
                leftIcon: true
                iconSource: EIcons.PENCIL
                styleType: EStyle.Primary
            }
        }
    }
    ExampleSection {
        componentName: "ELink"

        ExampleCase {
            objectName: "showcaseELink3"
            title: ELocale.texts.ClickFeedback
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ELink {
                        text: "查看最新版本"

                        onClicked: result.text = "当前版本：1.0"
                    }
                    EText {
                        id: result

                        text: "点击链接查看信息"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ELink {
                    text: "查看最新版本"

                    onClicked: result.text = "当前版本：1.0"
                }
                EText {
                    id: result

                    text: "点击链接查看信息"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERow"

        ExampleCase {
            objectName: "showcaseERow0"
            title: ELocale.texts.GridOffsets
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERow {
                    gutter: [16, 16]
                    width: parent.width

                    ECol {
                        span: 8
                        offset: 4
                        height: 64

                        ERectangle {
                            radius: 8
                            color: "#226DCA"
                            anchors.fill: parent

                            EText {
                                color: "white"
                                text: "8 / offset 4"
                                anchors.centerIn: parent
                            }
                        }
                    }
                    ECol {
                        span: 8
                        height: 64

                        ERectangle {
                            radius: 8
                            color: "#007D83"
                            anchors.fill: parent

                            EText {
                                color: "white"
                                text: "8 / 24"
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            `)

            ERow {
                gutter: [16, 16]
                width: parent.width

                ECol {
                    span: 8
                    offset: 4
                    height: 64

                    ERectangle {
                        radius: 8
                        color: "#226DCA"
                        anchors.fill: parent

                        EText {
                            color: "white"
                            text: "8 / offset 4"
                            anchors.centerIn: parent
                        }
                    }
                }
                ECol {
                    span: 8
                    height: 64

                    ERectangle {
                        radius: 8
                        color: "#007D83"
                        anchors.fill: parent

                        EText {
                            color: "white"
                            text: "8 / 24"
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERow"

        ExampleCase {
            objectName: "showcaseERow1"
            title: ELocale.texts.RowAlignment
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERow {
                    gutter: 16
                    align: "middle"
                    width: parent.width
                    justify: "space-between"

                    EButton {
                        text: "操作 A"
                    }
                    EButton {
                        text: "主要操作"
                        size: EStyle.Large
                        styleType: EStyle.Primary
                    }
                    EButton {
                        text: "操作 B"
                    }
                }
            `)

            ERow {
                gutter: 16
                align: "middle"
                width: parent.width
                justify: "space-between"

                EButton {
                    text: "操作 A"
                }
                EButton {
                    text: "主要操作"
                    size: EStyle.Large
                    styleType: EStyle.Primary
                }
                EButton {
                    text: "操作 B"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ERow"

        ExampleCase {
            objectName: "showcaseERow2"
            title: ELocale.texts.ResponsiveRow
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ERow {
                    gutter: [12, 12]
                    width: parent.width
                    viewportWidth: width

                    ECol {
                        md: 8
                        sm: 12
                        xs: 24
                        height: 60

                        ERectangle {
                            radius: 8
                            color: "#226DCA"
                            anchors.fill: parent

                            EText {
                                color: "white"
                                text: "Responsive"
                                anchors.centerIn: parent
                            }
                        }
                    }
                    ECol {
                        md: 8
                        sm: 12
                        xs: 24
                        height: 60

                        ERectangle {
                            radius: 8
                            color: "#007D83"
                            anchors.fill: parent

                            EText {
                                color: "white"
                                text: "Responsive"
                                anchors.centerIn: parent
                            }
                        }
                    }
                    ECol {
                        md: 8
                        sm: 12
                        xs: 24
                        height: 60

                        ERectangle {
                            radius: 8
                            color: "#6F51C5"
                            anchors.fill: parent

                            EText {
                                color: "white"
                                text: "Responsive"
                                anchors.centerIn: parent
                            }
                        }
                    }
                }
            `)

            ERow {
                gutter: [12, 12]
                width: parent.width
                viewportWidth: width

                ECol {
                    md: 8
                    sm: 12
                    xs: 24
                    height: 60

                    ERectangle {
                        radius: 8
                        color: "#226DCA"
                        anchors.fill: parent

                        EText {
                            color: "white"
                            text: "Responsive"
                            anchors.centerIn: parent
                        }
                    }
                }
                ECol {
                    md: 8
                    sm: 12
                    xs: 24
                    height: 60

                    ERectangle {
                        radius: 8
                        color: "#007D83"
                        anchors.fill: parent

                        EText {
                            color: "white"
                            text: "Responsive"
                            anchors.centerIn: parent
                        }
                    }
                }
                ECol {
                    md: 8
                    sm: 12
                    xs: 24
                    height: 60

                    ERectangle {
                        radius: 8
                        color: "#6F51C5"
                        anchors.fill: parent

                        EText {
                            color: "white"
                            text: "Responsive"
                            anchors.centerIn: parent
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESpace"

        ExampleCase {
            objectName: "showcaseESpace0"
            title: ELocale.texts.VerticalSpacing
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESpace {
                    size: 20
                    alignment: "start"
                    direction: "vertical"
                    width: Math.min(480, parent.width)

                    EButton {
                        text: "设计"
                    }
                    EButton {
                        text: "开发"
                    }
                    EButton {
                        text: "发布"
                    }
                }
            `)

            ESpace {
                size: 20
                alignment: "start"
                direction: "vertical"
                width: Math.min(480, parent.width)

                EButton {
                    text: "设计"
                }
                EButton {
                    text: "开发"
                }
                EButton {
                    text: "发布"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESpace"

        ExampleCase {
            objectName: "showcaseESpace1"
            title: ELocale.texts.WrappingAndSeparators
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESpace {
                    wrap: true
                    spacer: "·"
                    size: [12, 20]
                    width: Math.min(360, parent.width)

                    ETag {
                        text: "QML"
                    }
                    ETag {
                        text: "Qt Quick"
                    }
                    ETag {
                        text: "C++"
                    }
                    ETag {
                        text: "Design"
                    }
                    ETag {
                        text: "Windows"
                    }
                    ETag {
                        text: "macOS"
                    }
                }
            `)

            ESpace {
                wrap: true
                spacer: "·"
                size: [12, 20]
                width: Math.min(360, parent.width)

                ETag {
                    text: "QML"
                }
                ETag {
                    text: "Qt Quick"
                }
                ETag {
                    text: "C++"
                }
                ETag {
                    text: "Design"
                }
                ETag {
                    text: "Windows"
                }
                ETag {
                    text: "macOS"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESpace"

        ExampleCase {
            objectName: "showcaseESpace2"
            title: ELocale.texts.EqualWidthSpace
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESpace {
                    size: 12
                    fill: true
                    fillRatio: 100
                    width: Math.min(480, parent.width)

                    EButton {
                        text: "设计"
                    }
                    EButton {
                        text: "开发"
                    }
                    EButton {
                        text: "发布"
                    }
                }
            `)

            ESpace {
                size: 12
                fill: true
                fillRatio: 100
                width: Math.min(480, parent.width)

                EButton {
                    text: "设计"
                }
                EButton {
                    text: "开发"
                }
                EButton {
                    text: "发布"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAppBar"

        ExampleCase {
            objectName: "showcaseEAppBar0"
            title: ELocale.texts.TitleIcon
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAppBar {
                    showClose: false
                    title: "EUI 工作台"
                    showMaximize: false
                    showMinimize: false
                    width: parent.width
                    iconSource: EIcons.PALETTE
                }
            `)

            EAppBar {
                showClose: false
                title: "EUI 工作台"
                showMaximize: false
                showMinimize: false
                width: parent.width
                iconSource: EIcons.PALETTE
            }
        }
    }
    ExampleSection {
        componentName: "EAppBar"

        ExampleCase {
            objectName: "showcaseEAppBar1"
            title: ELocale.texts.AppBarColors
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAppBar {
                    showClose: false
                    title: "EUI 工作台"
                    showMaximize: false
                    showMinimize: false
                    width: parent.width
                    textColor: "#E7F0FF"
                    backgroundColor: "#173958"
                }
            `)

            EAppBar {
                showClose: false
                title: "EUI 工作台"
                showMaximize: false
                showMinimize: false
                width: parent.width
                textColor: "#E7F0FF"
                backgroundColor: "#173958"
            }
        }
    }
    ExampleSection {
        componentName: "EAppBar"

        ExampleCase {
            objectName: "showcaseEAppBar2"
            title: ELocale.texts.AppBarActions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAppBar {
                    showClose: false
                    title: "EUI 工作台"
                    showMaximize: false
                    showMinimize: false
                    width: parent.width

                    ETextButton {
                        text: "帮助"
                    }
                }
            `)

            EAppBar {
                showClose: false
                title: "EUI 工作台"
                showMaximize: false
                showMinimize: false
                width: parent.width

                ETextButton {
                    text: "帮助"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EAppBar"

        ExampleCase {
            objectName: "showcaseEAppBar3"
            title: ELocale.texts.AppBarTruncation
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EAppBar {
                    showClose: false
                    showMaximize: false
                    showMinimize: false
                    width: parent.width
                    title: "EUI Design System / 一个需要在窄窗口下正确省略的项目名称"
                }
            `)

            EAppBar {
                showClose: false
                showMaximize: false
                showMinimize: false
                width: parent.width
                title: "EUI Design System / 一个需要在窄窗口下正确省略的项目名称"
            }
        }
    }
    ExampleSection {
        componentName: "ESplitter"

        ExampleCase {
            objectName: "showcaseESplitter0"
            title: ELocale.texts.VerticalSplitter
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESplitter {
                    height: 200
                    width: parent.width
                    orientation: Qt.Vertical

                    ESplitterPanel {
                        size: "40%"

                        ECard {
                            header: "资源"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "工作区"
                            anchors.fill: parent
                        }
                    }
                }
            `)

            ESplitter {
                height: 200
                width: parent.width
                orientation: Qt.Vertical

                ESplitterPanel {
                    size: "40%"

                    ECard {
                        header: "资源"
                        anchors.fill: parent
                    }
                }
                ESplitterPanel {
                    min: 80

                    ECard {
                        header: "工作区"
                        anchors.fill: parent
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESplitter"

        ExampleCase {
            objectName: "showcaseESplitter1"
            title: ELocale.texts.PanelConstraints
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESplitter {
                    height: 200
                    width: parent.width

                    ESplitterPanel {
                        min: 100
                        max: "70%"
                        size: "35%"

                        ECard {
                            header: "资源"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "工作区"
                            anchors.fill: parent
                        }
                    }
                }
            `)

            ESplitter {
                height: 200
                width: parent.width

                ESplitterPanel {
                    min: 100
                    max: "70%"
                    size: "35%"

                    ECard {
                        header: "资源"
                        anchors.fill: parent
                    }
                }
                ESplitterPanel {
                    min: 80

                    ECard {
                        header: "工作区"
                        anchors.fill: parent
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESplitter"

        ExampleCase {
            objectName: "showcaseESplitter2"
            title: ELocale.texts.PanelCollapse
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "折叠 / 恢复"

                        onClicked: firstPanel.setCollapsed(!firstPanel.collapsed)
                    }
                    ESplitter {
                        height: 200
                        width: parent.width

                        ESplitterPanel {
                            id: firstPanel

                            size: "35%"
                            collapsible: true

                            ECard {
                                header: "资源"
                                anchors.fill: parent
                            }
                        }
                        ESplitterPanel {
                            min: 80

                            ECard {
                                header: "工作区"
                                anchors.fill: parent
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "折叠 / 恢复"

                    onClicked: firstPanel.setCollapsed(!firstPanel.collapsed)
                }
                ESplitter {
                    height: 200
                    width: parent.width

                    ESplitterPanel {
                        id: firstPanel

                        size: "35%"
                        collapsible: true

                        ECard {
                            header: "资源"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "工作区"
                            anchors.fill: parent
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EScrollbar"

        ExampleCase {
            objectName: "showcaseEScrollbar0"
            title: ELocale.texts.PersistentScrollbar
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EScrollbar {
                        id: scroll

                        height: 200
                        always: true
                        contentHeight: 600
                        contentWidth: width
                        width: Math.min(480, parent.width)

                        EText {
                            height: 600
                            text: "滚动内容\\n\\n\\n\\n\\n\\n底部"
                        }
                    }
                    EButton {
                        text: "滚到末尾"

                        onClicked: scroll.setScrollTop(400)
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EScrollbar {
                    id: scroll

                    height: 200
                    always: true
                    contentHeight: 600
                    contentWidth: width
                    width: Math.min(480, parent.width)

                    EText {
                        height: 600
                        text: "滚动内容\n\n\n\n\n\n底部"
                    }
                }
                EButton {
                    text: "滚到末尾"

                    onClicked: scroll.setScrollTop(400)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EScrollbar"

        ExampleCase {
            objectName: "showcaseEScrollbar1"
            title: ELocale.texts.HorizontalScrolling
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EScrollbar {
                    mask: true
                    height: 140
                    maskSize: 32
                    contentWidth: 900
                    contentHeight: 100
                    width: Math.min(480, parent.width)

                    EText {
                        width: 900
                        font.pixelSize: 24
                        text: "Design                         Develop                         Deploy"
                    }
                }
            `)

            EScrollbar {
                mask: true
                height: 140
                maskSize: 32
                contentWidth: 900
                contentHeight: 100
                width: Math.min(480, parent.width)

                EText {
                    width: 900
                    font.pixelSize: 24
                    text: "Design                         Develop                         Deploy"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EScrollbar"

        ExampleCase {
            objectName: "showcaseEScrollbar2"
            title: ELocale.texts.ScrollPosition
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EScrollbar {
                        id: scroll

                        height: 180
                        contentHeight: 500
                        contentWidth: width
                        width: Math.min(480, parent.width)

                        EText {
                            height: 500
                            text: "滚动查看当前位置"
                        }
                    }
                    EText {
                        text: "滚动位置：" + Math.round(scroll.scrollTop)
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EScrollbar {
                    id: scroll

                    height: 180
                    contentHeight: 500
                    contentWidth: width
                    width: Math.min(480, parent.width)

                    EText {
                        height: 500
                        text: "滚动查看当前位置"
                    }
                }
                EText {
                    text: "滚动位置：" + Math.round(scroll.scrollTop)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EContainer"

        ExampleCase {
            objectName: "showcaseEContainer0"
            title: ELocale.texts.HeaderAndMain
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EContainer {
                    height: 200
                    width: parent.width

                    EHeader {
                        EText {
                            text: "工作台顶栏"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EMain {
                        EText {
                            text: "主内容区域"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            `)

            EContainer {
                height: 200
                width: parent.width

                EHeader {
                    EText {
                        text: "工作台顶栏"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EMain {
                    EText {
                        text: "主内容区域"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EContainer"

        ExampleCase {
            objectName: "showcaseEContainer1"
            title: ELocale.texts.AsideAndMain
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EContainer {
                    height: 200
                    width: parent.width

                    EAside {
                        implicitWidth: 120

                        EText {
                            text: "资源目录"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EMain {
                        EText {
                            text: "项目内容"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            `)

            EContainer {
                height: 200
                width: parent.width

                EAside {
                    implicitWidth: 120

                    EText {
                        text: "资源目录"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EMain {
                    EText {
                        text: "项目内容"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EContainer"

        ExampleCase {
            objectName: "showcaseEContainer2"
            title: ELocale.texts.MainAndFooter
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EContainer {
                    height: 200
                    width: parent.width

                    EMain {
                        EText {
                            text: "主工作区"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EFooter {
                        EText {
                            text: "状态栏"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            `)

            EContainer {
                height: 200
                width: parent.width

                EMain {
                    EText {
                        text: "主工作区"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EFooter {
                    EText {
                        text: "状态栏"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EContainer"

        ExampleCase {
            objectName: "showcaseEContainer3"
            title: ELocale.texts.NestedWorkspace
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EContainer {
                    height: 200
                    width: parent.width

                    EHeader {
                        EText {
                            text: "项目名称"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EContainer {
                        direction: "horizontal"

                        EAside {
                            implicitWidth: 120

                            EText {
                                text: "目录"
                                width: parent.width
                                wrapMode: Text.WordWrap
                            }
                        }
                        EMain {
                            EText {
                                text: "画布"
                                width: parent.width
                                wrapMode: Text.WordWrap
                            }
                        }
                    }
                    EFooter {
                        EText {
                            text: "已保存"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
            `)

            EContainer {
                height: 200
                width: parent.width

                EHeader {
                    EText {
                        text: "项目名称"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
                EContainer {
                    direction: "horizontal"

                    EAside {
                        implicitWidth: 120

                        EText {
                            text: "目录"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                    EMain {
                        EText {
                            text: "画布"
                            width: parent.width
                            wrapMode: Text.WordWrap
                        }
                    }
                }
                EFooter {
                    EText {
                        text: "已保存"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETypography"

        ExampleCase {
            objectName: "showcaseETypography0"
            title: ELocale.texts.HeadingHierarchy
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    ETypography {
                        level: 1
                        text: "标题"
                    }
                    ETypography {
                        level: 2
                        text: "标题"
                    }
                    ETypography {
                        level: 3
                        text: "标题"
                    }
                    ETypography {
                        level: 4
                        text: "标题"
                    }
                    ETypography {
                        level: 5
                        text: "标题"
                    }
                    ETypography {
                        level: 6
                        text: "标题"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                ETypography {
                    level: 1
                    text: "标题"
                }
                ETypography {
                    level: 2
                    text: "标题"
                }
                ETypography {
                    level: 3
                    text: "标题"
                }
                ETypography {
                    level: 4
                    text: "标题"
                }
                ETypography {
                    level: 5
                    text: "标题"
                }
                ETypography {
                    level: 6
                    text: "标题"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETypography"

        ExampleCase {
            objectName: "showcaseETypography1"
            title: ELocale.texts.ParagraphSpacing
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ETypography {
                    level: 0
                    lineHeight: 1.65
                    width: Math.min(480, parent.width)
                    text: "为组件提供清晰的阅读层级。正文保持舒适的行距，在窄窗口下自动换行，让中英文内容都易于阅读。"
                }
            `)

            ETypography {
                level: 0
                lineHeight: 1.65
                width: Math.min(480, parent.width)
                text: "为组件提供清晰的阅读层级。正文保持舒适的行距，在窄窗口下自动换行，让中英文内容都易于阅读。"
            }
        }
    }
    ExampleSection {
        componentName: "ETypography"

        ExampleCase {
            objectName: "showcaseETypography2"
            title: ELocale.texts.HeadingAndCaption
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ETypography {
                        level: 3
                        text: "项目详情"
                    }
                    ETypography {
                        text: "更新时间：今天 · 状态：已发布"
                        width: Math.min(480, parent.width)
                        color: EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ETypography {
                    level: 3
                    text: "项目详情"
                }
                ETypography {
                    text: "更新时间：今天 · 状态：已发布"
                    width: Math.min(480, parent.width)
                    color: EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETypography"

        ExampleCase {
            objectName: "showcaseETypography3"
            title: ELocale.texts.MixedLanguageHeading
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ETypography {
                    level: 2
                    width: Math.min(480, parent.width)
                    text: "EUI Design System：跨平台组件演示工作台"
                }
            `)

            ETypography {
                level: 2
                width: Math.min(480, parent.width)
                text: "EUI Design System：跨平台组件演示工作台"
            }
        }
    }
    ExampleSection {
        componentName: "ESplitterPanel"

        ExampleCase {
            objectName: "showcaseESplitterPanel0"
            title: ELocale.texts.VerticalSplitter
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESplitter {
                    height: 200
                    width: parent.width
                    orientation: Qt.Vertical

                    ESplitterPanel {
                        size: "40%"

                        ECard {
                            header: "资源"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "工作区"
                            anchors.fill: parent
                        }
                    }
                }
            `)

            ESplitter {
                height: 200
                width: parent.width
                orientation: Qt.Vertical

                ESplitterPanel {
                    size: "40%"

                    ECard {
                        header: "资源"
                        anchors.fill: parent
                    }
                }
                ESplitterPanel {
                    min: 80

                    ECard {
                        header: "工作区"
                        anchors.fill: parent
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESplitterPanel"

        ExampleCase {
            objectName: "showcaseESplitterPanel1"
            title: ELocale.texts.PanelConstraints
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESplitter {
                    height: 200
                    width: parent.width

                    ESplitterPanel {
                        min: 100
                        max: "70%"
                        size: "35%"

                        ECard {
                            header: "资源"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "工作区"
                            anchors.fill: parent
                        }
                    }
                }
            `)

            ESplitter {
                height: 200
                width: parent.width

                ESplitterPanel {
                    min: 100
                    max: "70%"
                    size: "35%"

                    ECard {
                        header: "资源"
                        anchors.fill: parent
                    }
                }
                ESplitterPanel {
                    min: 80

                    ECard {
                        header: "工作区"
                        anchors.fill: parent
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESplitterPanel"

        ExampleCase {
            title: ELocale.texts.PanelCollapse
            objectName: "showcaseESplitterPanel2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "折叠 / 恢复"

                        onClicked: firstPanel.setCollapsed(!firstPanel.collapsed)
                    }
                    ESplitter {
                        height: 200
                        width: parent.width

                        ESplitterPanel {
                            id: firstPanel

                            size: "35%"
                            collapsible: true

                            ECard {
                                header: "资源"
                                anchors.fill: parent
                            }
                        }
                        ESplitterPanel {
                            min: 80

                            ECard {
                                header: "工作区"
                                anchors.fill: parent
                            }
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "折叠 / 恢复"

                    onClicked: firstPanel.setCollapsed(!firstPanel.collapsed)
                }
                ESplitter {
                    height: 200
                    width: parent.width

                    ESplitterPanel {
                        id: firstPanel

                        size: "35%"
                        collapsible: true

                        ECard {
                            header: "资源"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "工作区"
                            anchors.fill: parent
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ESplitterPanel"

        ExampleCase {
            title: ELocale.texts.FixedPanel
            objectName: "showcaseESplitterPanel3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESplitter {
                    height: 180
                    width: parent.width

                    ESplitterPanel {
                        size: 160
                        resizable: false

                        ECard {
                            header: "固定资源栏"
                            anchors.fill: parent
                        }
                    }
                    ESplitterPanel {
                        min: 80

                        ECard {
                            header: "自适应工作区"
                            anchors.fill: parent
                        }
                    }
                }
            `)

            ESplitter {
                height: 180
                width: parent.width

                ESplitterPanel {
                    size: 160
                    resizable: false

                    ECard {
                        header: "固定资源栏"
                        anchors.fill: parent
                    }
                }
                ESplitterPanel {
                    min: 80

                    ECard {
                        header: "自适应工作区"
                        anchors.fill: parent
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStaggeredLayout"

        ExampleCase {
            title: ELocale.texts.TwoColumns
            objectName: "showcaseEStaggeredLayout0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStaggeredLayout {
                    columns: 2
                    spacing: 12
                    width: parent.width

                    Repeater {
                        model: [80, 120, 100, 64, 110, 90]

                        ERectangle {
                            radius: 12
                            height: modelData
                            color: index % 2 ? "#007D83" : "#226DCA"

                            EText {
                                color: "white"
                                anchors.centerIn: parent
                                text: "Panel " + (index + 1)
                            }
                        }
                    }
                }
            `)

            EStaggeredLayout {
                columns: 2
                spacing: 12
                width: parent.width

                Repeater {
                    model: [80, 120, 100, 64, 110, 90]

                    ERectangle {
                        radius: 12
                        height: modelData
                        color: index % 2 ? "#007D83" : "#226DCA"

                        EText {
                            color: "white"
                            anchors.centerIn: parent
                            text: "Panel " + (index + 1)
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStaggeredLayout"

        ExampleCase {
            title: ELocale.texts.CompactMasonry
            objectName: "showcaseEStaggeredLayout1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStaggeredLayout {
                    columns: 3
                    spacing: 8
                    width: parent.width

                    Repeater {
                        model: [80, 120, 100, 64, 110, 90]

                        ERectangle {
                            radius: 12
                            height: modelData
                            color: index % 2 ? "#007D83" : "#226DCA"

                            EText {
                                color: "white"
                                anchors.centerIn: parent
                                text: "Panel " + (index + 1)
                            }
                        }
                    }
                }
            `)

            EStaggeredLayout {
                columns: 3
                spacing: 8
                width: parent.width

                Repeater {
                    model: [80, 120, 100, 64, 110, 90]

                    ERectangle {
                        radius: 12
                        height: modelData
                        color: index % 2 ? "#007D83" : "#226DCA"

                        EText {
                            color: "white"
                            anchors.centerIn: parent
                            text: "Panel " + (index + 1)
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStaggeredLayout"

        ExampleCase {
            title: ELocale.texts.ResponsiveColumns
            objectName: "showcaseEStaggeredLayout2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStaggeredLayout {
                    spacing: 16
                    width: parent.width
                    columns: width < 500 ? 2 : 4

                    Repeater {
                        model: [80, 120, 100, 64, 110, 90]

                        ERectangle {
                            radius: 12
                            height: modelData
                            color: index % 2 ? "#007D83" : "#226DCA"

                            EText {
                                color: "white"
                                anchors.centerIn: parent
                                text: "Panel " + (index + 1)
                            }
                        }
                    }
                }
            `)

            EStaggeredLayout {
                spacing: 16
                width: parent.width
                columns: width < 500 ? 2 : 4

                Repeater {
                    model: [80, 120, 100, 64, 110, 90]

                    ERectangle {
                        radius: 12
                        height: modelData
                        color: index % 2 ? "#007D83" : "#226DCA"

                        EText {
                            color: "white"
                            anchors.centerIn: parent
                            text: "Panel " + (index + 1)
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStaggeredLayout"

        ExampleCase {
            title: ELocale.texts.SingleColumn
            objectName: "showcaseEStaggeredLayout3"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStaggeredLayout {
                    columns: 1
                    spacing: 20
                    width: parent.width

                    Repeater {
                        model: [80, 120, 100, 64, 110, 90]

                        ERectangle {
                            radius: 12
                            height: modelData
                            color: index % 2 ? "#007D83" : "#226DCA"

                            EText {
                                color: "white"
                                anchors.centerIn: parent
                                text: "Panel " + (index + 1)
                            }
                        }
                    }
                }
            `)

            EStaggeredLayout {
                columns: 1
                spacing: 20
                width: parent.width

                Repeater {
                    model: [80, 120, 100, 64, 110, 90]

                    ERectangle {
                        radius: 12
                        height: modelData
                        color: index % 2 ? "#007D83" : "#226DCA"

                        EText {
                            color: "white"
                            anchors.centerIn: parent
                            text: "Panel " + (index + 1)
                        }
                    }
                }
            }
        }
    }
}
