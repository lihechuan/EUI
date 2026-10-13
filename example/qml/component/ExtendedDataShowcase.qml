import EUI 1.0
import QtQuick 2.15

Column {
    spacing: 24
    width: parent.width

    ExampleSection {
        componentName: "EChart"

        ExampleCase {
            objectName: "showcaseEChart0"
            title: ELocale.texts.AreaChart
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EChart {
                    type: 2
                    height: 220
                    color: "#21B6A8"
                    width: Math.min(640, parent.width)
                    model: [{label: "Mon", value: 30}, {label: "Tue", value: 55}, {label: "Wed", value: 42}, {label: "Thu", value: 80}]
                }
            `)

            EChart {
                type: 2
                height: 220
                color: "#21B6A8"
                width: Math.min(640, parent.width)
                model: [{label: "Mon", value: 30}, {label: "Tue", value: 55}, {label: "Wed", value: 42}, {label: "Thu", value: 80}]
            }
        }
    }
    ExampleSection {
        componentName: "EChart"

        ExampleCase {
            objectName: "showcaseEChart1"
            title: ELocale.texts.BarChart2
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EChart {
                    type: 1
                    height: 220
                    color: "#A78BFA"
                    width: Math.min(640, parent.width)
                    model: [{label: "Mon", value: 30}, {label: "Tue", value: 55}, {label: "Wed", value: 42}, {label: "Thu", value: 80}]
                }
            `)

            EChart {
                type: 1
                height: 220
                color: "#A78BFA"
                width: Math.min(640, parent.width)
                model: [{label: "Mon", value: 30}, {label: "Tue", value: 55}, {label: "Wed", value: 42}, {label: "Thu", value: 80}]
            }
        }
    }
    ExampleSection {
        componentName: "EChart"

        ExampleCase {
            objectName: "showcaseEChart2"
            title: ELocale.texts.EmptyChart
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EChart {
                    model: []
                    height: 220
                    width: Math.min(640, parent.width)
                }
            `)

            EChart {
                model: []
                height: 220
                width: Math.min(640, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "EChart"

        ExampleCase {
            objectName: "showcaseEChart3"
            title: ELocale.texts.ChartUpdates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EChart {
                        id: chart

                        height: 220
                        width: Math.min(640, parent.width)
                        model: [{label: "Mon", value: 30}, {label: "Tue", value: 55}, {label: "Wed", value: 42}, {label: "Thu", value: 80}]
                    }
                    EButton {
                        text: "更新数据"

                        onClicked: chart.model = [{label: "Mon", value: 80}, {label: "Tue", value: 30}, {label: "Wed", value: 65}, {label: "Thu", value: 45}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EChart {
                    id: chart

                    height: 220
                    width: Math.min(640, parent.width)
                    model: [{label: "Mon", value: 30}, {label: "Tue", value: 55}, {label: "Wed", value: 42}, {label: "Thu", value: 80}]
                }
                EButton {
                    text: "更新数据"

                    onClicked: chart.model = [{label: "Mon", value: 80}, {label: "Tue", value: 30}, {label: "Wed", value: 65}, {label: "Thu", value: 45}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EImage"

        ExampleCase {
            objectName: "showcaseEImage0"
            title: ELocale.texts.ImageFitModes
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EImage {
                        width: 140
                        height: 100
                        fillMode: Image.PreserveAspectFit
                        source: "qrc:/example/eui-mark.svg"
                    }
                    EImage {
                        width: 140
                        height: 100
                        fillMode: Image.PreserveAspectCrop
                        source: "qrc:/example/eui-mark.svg"
                    }
                    EImage {
                        width: 140
                        height: 100
                        fillMode: Image.Stretch
                        source: "qrc:/example/eui-mark.svg"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EImage {
                    width: 140
                    height: 100
                    fillMode: Image.PreserveAspectFit
                    source: "qrc:/example/eui-mark.svg"
                }
                EImage {
                    width: 140
                    height: 100
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/example/eui-mark.svg"
                }
                EImage {
                    width: 140
                    height: 100
                    fillMode: Image.Stretch
                    source: "qrc:/example/eui-mark.svg"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EImage"

        ExampleCase {
            objectName: "showcaseEImage1"
            title: ELocale.texts.ImageError
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EImage {
                    width: 240
                    height: 140
                    errorText: "图片不可用"
                    source: "qrc:/example/missing-image.png"
                }
            `)

            EImage {
                width: 240
                height: 140
                errorText: "图片不可用"
                source: "qrc:/example/missing-image.png"
            }
        }
    }
    ExampleSection {
        componentName: "EImage"

        ExampleCase {
            objectName: "showcaseEImage2"
            title: ELocale.texts.ImagePreview
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EImage {
                    width: 180
                    height: 140
                    maxScale: 3
                    minScale: 0.5
                    preview: true
                    zoomRate: 1.2
                    source: "qrc:/example/eui-mark.svg"
                    previewSrcList: ["qrc:/example/eui-mark.svg", "qrc:/example/eui-mark.svg"]
                }
            `)

            EImage {
                width: 180
                height: 140
                maxScale: 3
                minScale: 0.5
                preview: true
                zoomRate: 1.2
                source: "qrc:/example/eui-mark.svg"
                previewSrcList: ["qrc:/example/eui-mark.svg", "qrc:/example/eui-mark.svg"]
            }
        }
    }
    ExampleSection {
        componentName: "ETour"

        ExampleCase {
            objectName: "showcaseETour0"
            title: ELocale.texts.GuidedSteps
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "开始引导"

                        onClicked: tour.open()
                    }
                    ETour {
                        id: tour

                        gap: ({offset: 8, radius: 12})
                        steps: [{target: target, title: "创建项目", description: "从这里开始工作。"}, {title: "准备就绪", description: "可以继续浏览组件。"}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "开始引导"

                    onClicked: tour.open()
                }
                ETour {
                    id: tour

                    gap: ({offset: 8, radius: 12})
                    steps: [{target: target, title: "创建项目", description: "从这里开始工作。"}, {title: "准备就绪", description: "可以继续浏览组件。"}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETour"

        ExampleCase {
            objectName: "showcaseETour1"
            title: ELocale.texts.UnmaskedTour
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "显示提示"

                        onClicked: tour.open()
                    }
                    ETour {
                        id: tour

                        mask: false
                        type: "primary"
                        placement: "right"
                        steps: [{target: target, title: "操作提示", description: "轻量引导保留页面上下文。"}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "显示提示"

                    onClicked: tour.open()
                }
                ETour {
                    id: tour

                    mask: false
                    type: "primary"
                    placement: "right"
                    steps: [{target: target, title: "操作提示", description: "轻量引导保留页面上下文。"}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETour"

        ExampleCase {
            objectName: "showcaseETour2"
            title: ELocale.texts.TourMaskOptions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        id: target

                        text: "开始引导"

                        onClicked: tour.open()
                    }
                    ETour {
                        id: tour

                        showClose: true
                        showArrow: false
                        maskColor: "#B30B1423"
                        closeOnPressEscape: true
                        steps: [{target: target, title: "工作台", description: "按 Escape 可结束引导。"}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    id: target

                    text: "开始引导"

                    onClicked: tour.open()
                }
                ETour {
                    id: tour

                    showClose: true
                    showArrow: false
                    maskColor: "#B30B1423"
                    closeOnPressEscape: true
                    steps: [{target: target, title: "工作台", description: "按 Escape 可结束引导。"}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EResult"

        ExampleCase {
            objectName: "showcaseEResult0"
            title: ELocale.texts.SuccessAction
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EResult {
                    title: "保存成功"
                    subTitle: "项目已经更新。"
                    styleType: EStyle.Success
                    width: Math.min(480, parent.width)

                    EButton {
                        text: "返回项目"
                    }
                }
            `)

            EResult {
                title: "保存成功"
                subTitle: "项目已经更新。"
                styleType: EStyle.Success
                width: Math.min(480, parent.width)

                EButton {
                    text: "返回项目"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EResult"

        ExampleCase {
            objectName: "showcaseEResult1"
            title: ELocale.texts.ResultStates
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EResult {
                        title: "操作结果"
                        subTitle: "请查看详细说明。"
                        styleType: EStyle.Warning
                        width: Math.min(300, parent.width)
                    }
                    EResult {
                        title: "操作结果"
                        subTitle: "请查看详细说明。"
                        styleType: EStyle.Danger
                        width: Math.min(300, parent.width)
                    }
                    EResult {
                        title: "操作结果"
                        subTitle: "请查看详细说明。"
                        styleType: EStyle.Info
                        width: Math.min(300, parent.width)
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EResult {
                    title: "操作结果"
                    subTitle: "请查看详细说明。"
                    styleType: EStyle.Warning
                    width: Math.min(300, parent.width)
                }
                EResult {
                    title: "操作结果"
                    subTitle: "请查看详细说明。"
                    styleType: EStyle.Danger
                    width: Math.min(300, parent.width)
                }
                EResult {
                    title: "操作结果"
                    subTitle: "请查看详细说明。"
                    styleType: EStyle.Info
                    width: Math.min(300, parent.width)
                }
            }
        }
    }
    ExampleSection {
        componentName: "EResult"

        ExampleCase {
            objectName: "showcaseEResult2"
            title: ELocale.texts.CustomResultIcon
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EResult {
                    title: "准备就绪"
                    subTitle: "自定义图标和说明。"
                    width: Math.min(480, parent.width)
                    iconDelegate: Component {
                        EIcon {
                            iconSize: 56
                            color: "#21B6A8"
                            iconSource: EIcons.PALETTE
                        }
                    }
                }
            `)

            EResult {
                title: "准备就绪"
                subTitle: "自定义图标和说明。"
                width: Math.min(480, parent.width)
                iconDelegate: Component {
                    EIcon {
                        iconSize: 56
                        color: "#21B6A8"
                        iconSource: EIcons.PALETTE
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "EQRCode"

        ExampleCase {
            title: ELocale.texts.QRSizes
            objectName: "showcaseEQRCode0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EQRCode {
                        width: 100
                        height: 100
                        text: "https://example.com/eui"
                    }
                    EQRCode {
                        width: 140
                        height: 140
                        text: "https://example.com/eui"
                    }
                    EQRCode {
                        width: 180
                        height: 180
                        text: "https://example.com/eui"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EQRCode {
                    width: 100
                    height: 100
                    text: "https://example.com/eui"
                }
                EQRCode {
                    width: 140
                    height: 140
                    text: "https://example.com/eui"
                }
                EQRCode {
                    width: 180
                    height: 180
                    text: "https://example.com/eui"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EQRCode"

        ExampleCase {
            objectName: "showcaseEQRCode1"
            title: ELocale.texts.QRColorsAndQuietZone
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EQRCode {
                    width: 180
                    height: 180
                    quietZone: 6
                    color: "#173958"
                    text: "EUI Design System"
                    backgroundColor: "#E7F0FF"
                }
            `)

            EQRCode {
                width: 180
                height: 180
                quietZone: 6
                color: "#173958"
                text: "EUI Design System"
                backgroundColor: "#E7F0FF"
            }
        }
    }
    ExampleSection {
        componentName: "EQRCode"

        ExampleCase {
            objectName: "showcaseEQRCode2"
            title: ELocale.texts.ErrorCorrection
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Flow {
                    spacing: 16
                    width: parent.width

                    EQRCode {
                        width: 140
                        height: 140
                        errorCorrection: 0
                        text: "EUI / Qt Quick"
                    }
                    EQRCode {
                        width: 140
                        height: 140
                        errorCorrection: 1
                        text: "EUI / Qt Quick"
                    }
                    EQRCode {
                        width: 140
                        height: 140
                        errorCorrection: 2
                        text: "EUI / Qt Quick"
                    }
                    EQRCode {
                        width: 140
                        height: 140
                        errorCorrection: 3
                        text: "EUI / Qt Quick"
                    }
                }
            `)

            Flow {
                spacing: 16
                width: parent.width

                EQRCode {
                    width: 140
                    height: 140
                    errorCorrection: 0
                    text: "EUI / Qt Quick"
                }
                EQRCode {
                    width: 140
                    height: 140
                    errorCorrection: 1
                    text: "EUI / Qt Quick"
                }
                EQRCode {
                    width: 140
                    height: 140
                    errorCorrection: 2
                    text: "EUI / Qt Quick"
                }
                EQRCode {
                    width: 140
                    height: 140
                    errorCorrection: 3
                    text: "EUI / Qt Quick"
                }
            }
        }
    }
    ExampleSection {
        componentName: "EQRCode"

        ExampleCase {
            objectName: "showcaseEQRCode3"
            title: ELocale.texts.LiveQRContent
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EInput {
                        id: content

                        text: "EUI"
                        width: Math.min(480, parent.width)
                    }
                    EQRCode {
                        width: 180
                        height: 180
                        text: content.text
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EInput {
                    id: content

                    text: "EUI"
                    width: Math.min(480, parent.width)
                }
                EQRCode {
                    width: 180
                    height: 180
                    text: content.text
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECalendar"

        ExampleCase {
            objectName: "showcaseECalendar0"
            title: ELocale.texts.CalendarRange
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECalendar {
                    width: Math.min(640, parent.width)
                    range: [new Date(2026, 9, 1), new Date(2026, 9, 31)]
                }
            `)

            ECalendar {
                width: Math.min(640, parent.width)
                range: [new Date(2026, 9, 1), new Date(2026, 9, 31)]
            }
        }
    }
    ExampleSection {
        componentName: "ECalendar"

        ExampleCase {
            objectName: "showcaseECalendar1"
            title: ELocale.texts.SelectController
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECalendar {
                    controllerType: "select"
                    width: Math.min(640, parent.width)
                }
            `)

            ECalendar {
                controllerType: "select"
                width: Math.min(640, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "ECalendar"

        ExampleCase {
            objectName: "showcaseECalendar2"
            title: ELocale.texts.CalendarCellContent
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECalendar {
                    width: Math.min(640, parent.width)
                    dateCellDelegate: Component {
                        EText {
                            text: parent.date.getDate()
                            horizontalAlignment: Text.AlignHCenter
                            color: parent.isSelected ? "#21B6A8" : EPaletteManager.ThemePalette.textRegular(ETheme.dark)
                        }
                    }
                }
            `)

            ECalendar {
                width: Math.min(640, parent.width)
                dateCellDelegate: Component {
                    EText {
                        text: parent.date.getDate()
                        horizontalAlignment: Text.AlignHCenter
                        color: parent.isSelected ? "#21B6A8" : EPaletteManager.ThemePalette.textRegular(ETheme.dark)
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECarousel"

        ExampleCase {
            objectName: "showcaseECarousel0"
            title: ELocale.texts.CardCarousel
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECarousel {
                    height: 220
                    type: "card"
                    autoplay: false
                    indicatorPosition: "outside"
                    width: Math.min(480, parent.width)
                    model: [{title: "设计", color: "#226DCA"}, {title: "开发", color: "#007D83"}, {title: "发布", color: "#6F51C5"}]
                    delegate: Component {
                        ERectangle {
                            radius: 12
                            color: parent.modelData.color || "#226DCA"

                            EText {
                                color: "white"
                                font.pixelSize: 24
                                anchors.centerIn: parent
                                text: parent.parent.modelData.title
                            }
                        }
                    }
                }
            `)

            ECarousel {
                height: 220
                type: "card"
                autoplay: false
                indicatorPosition: "outside"
                width: Math.min(480, parent.width)
                model: [{title: "设计", color: "#226DCA"}, {title: "开发", color: "#007D83"}, {title: "发布", color: "#6F51C5"}]
                delegate: Component {
                    ERectangle {
                        radius: 12
                        color: parent.modelData.color || "#226DCA"

                        EText {
                            color: "white"
                            font.pixelSize: 24
                            anchors.centerIn: parent
                            text: parent.parent.modelData.title
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECarousel"

        ExampleCase {
            objectName: "showcaseECarousel1"
            title: ELocale.texts.VerticalCarousel
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECarousel {
                    height: 220
                    loop: false
                    autoplay: false
                    initialIndex: 1
                    direction: "vertical"
                    width: Math.min(480, parent.width)
                    model: [{title: "第一屏"}, {title: "第二屏"}, {title: "第三屏"}]
                    delegate: Component {
                        ERectangle {
                            radius: 12
                            color: parent.modelData.color || "#226DCA"

                            EText {
                                color: "white"
                                font.pixelSize: 24
                                anchors.centerIn: parent
                                text: parent.parent.modelData.title
                            }
                        }
                    }
                }
            `)

            ECarousel {
                height: 220
                loop: false
                autoplay: false
                initialIndex: 1
                direction: "vertical"
                width: Math.min(480, parent.width)
                model: [{title: "第一屏"}, {title: "第二屏"}, {title: "第三屏"}]
                delegate: Component {
                    ERectangle {
                        radius: 12
                        color: parent.modelData.color || "#226DCA"

                        EText {
                            color: "white"
                            font.pixelSize: 24
                            anchors.centerIn: parent
                            text: parent.parent.modelData.title
                        }
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECarousel"

        ExampleCase {
            objectName: "showcaseECarousel2"
            title: ELocale.texts.CarouselNavigation
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ECarousel {
                        id: carousel

                        height: 220
                        autoplay: false
                        width: Math.min(480, parent.width)
                        model: [{title: "设计"}, {title: "开发"}, {title: "发布"}]
                        delegate: Component {
                            ERectangle {
                                radius: 12
                                color: parent.modelData.color || "#226DCA"

                                EText {
                                    color: "white"
                                    font.pixelSize: 24
                                    anchors.centerIn: parent
                                    text: parent.parent.modelData.title
                                }
                            }
                        }
                    }
                    Flow {
                        spacing: 16
                        width: parent.width

                        EButton {
                            text: "上一项"

                            onClicked: carousel.previous()
                        }
                        EButton {
                            text: "下一项"

                            onClicked: carousel.next()
                        }
                        EText {
                            text: "当前项：" + (carousel.currentIndex + 1)
                        }
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ECarousel {
                    id: carousel

                    height: 220
                    autoplay: false
                    width: Math.min(480, parent.width)
                    model: [{title: "设计"}, {title: "开发"}, {title: "发布"}]
                    delegate: Component {
                        ERectangle {
                            radius: 12
                            color: parent.modelData.color || "#226DCA"

                            EText {
                                color: "white"
                                font.pixelSize: 24
                                anchors.centerIn: parent
                                text: parent.parent.modelData.title
                            }
                        }
                    }
                }
                Flow {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "上一项"

                        onClicked: carousel.previous()
                    }
                    EButton {
                        text: "下一项"

                        onClicked: carousel.next()
                    }
                    EText {
                        text: "当前项：" + (carousel.currentIndex + 1)
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ETimeline"

        ExampleCase {
            objectName: "showcaseETimeline0"
            title: ELocale.texts.AlternatingTimeline
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ETimeline {
                    mode: "alternate"
                    width: Math.min(480, parent.width)
                    model: [{timestamp: "09:00", content: "创建项目", styleType: EStyle.Primary}, {timestamp: "10:30", content: "完成设计", styleType: EStyle.Success}, {timestamp: "12:00", content: "等待审核", styleType: EStyle.Warning}]
                }
            `)

            ETimeline {
                mode: "alternate"
                width: Math.min(480, parent.width)
                model: [{timestamp: "09:00", content: "创建项目", styleType: EStyle.Primary}, {timestamp: "10:30", content: "完成设计", styleType: EStyle.Success}, {timestamp: "12:00", content: "等待审核", styleType: EStyle.Warning}]
            }
        }
    }
    ExampleSection {
        componentName: "ETimeline"

        ExampleCase {
            objectName: "showcaseETimeline1"
            title: ELocale.texts.ReverseTimeline
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ETimeline {
                    reverse: true
                    width: Math.min(480, parent.width)
                    model: [{content: "待审核", timestamp: "12:00", hollow: true, size: "large", styleType: EStyle.Warning}, {content: "已保存", timestamp: "11:00", icon: EIcons.CHECK, styleType: EStyle.Success}]
                }
            `)

            ETimeline {
                reverse: true
                width: Math.min(480, parent.width)
                model: [{content: "待审核", timestamp: "12:00", hollow: true, size: "large", styleType: EStyle.Warning}, {content: "已保存", timestamp: "11:00", icon: EIcons.CHECK, styleType: EStyle.Success}]
            }
        }
    }
    ExampleSection {
        componentName: "ETimeline"

        ExampleCase {
            objectName: "showcaseETimeline2"
            title: ELocale.texts.TimelineDots
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ETimeline {
                    width: Math.min(480, parent.width)
                    dotDelegate: Component {
                        EIcon {
                            iconSize: 20
                            color: "#21B6A8"
                            iconSource: EIcons.CHECK
                        }
                    }
                    model: [{timestamp: "09:00", content: "创建项目", styleType: EStyle.Primary}, {timestamp: "10:30", content: "完成设计", styleType: EStyle.Success}, {timestamp: "12:00", content: "等待审核", styleType: EStyle.Warning}]
                }
            `)

            ETimeline {
                width: Math.min(480, parent.width)
                dotDelegate: Component {
                    EIcon {
                        iconSize: 20
                        color: "#21B6A8"
                        iconSource: EIcons.CHECK
                    }
                }
                model: [{timestamp: "09:00", content: "创建项目", styleType: EStyle.Primary}, {timestamp: "10:30", content: "完成设计", styleType: EStyle.Success}, {timestamp: "12:00", content: "等待审核", styleType: EStyle.Warning}]
            }
        }
    }
    ExampleSection {
        componentName: "ECollapse"

        ExampleCase {
            objectName: "showcaseECollapse0"
            title: ELocale.texts.AccordionCollapse
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECollapse {
                    accordion: true
                    activeNames: ["design"]
                    width: Math.min(480, parent.width)
                    model: [{name: "design", title: "设计规范", content: "字体、颜色和间距"}, {name: "dev", title: "开发规范", content: "QML 与 C++"}, {name: "locked", title: "归档", disabled: true}]
                }
            `)

            ECollapse {
                accordion: true
                activeNames: ["design"]
                width: Math.min(480, parent.width)
                model: [{name: "design", title: "设计规范", content: "字体、颜色和间距"}, {name: "dev", title: "开发规范", content: "QML 与 C++"}, {name: "locked", title: "归档", disabled: true}]
            }
        }
    }
    ExampleSection {
        componentName: "ECollapse"

        ExampleCase {
            objectName: "showcaseECollapse1"
            title: ELocale.texts.MultipleExpandedPanels
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECollapse {
                    activeNames: ["a", "b"]
                    expandIconPosition: "left"
                    width: Math.min(480, parent.width)
                    model: [{name: "a", title: "基本信息", content: "项目名称与描述"}, {name: "b", title: "高级设置", content: "组件主题与配置"}]
                }
            `)

            ECollapse {
                activeNames: ["a", "b"]
                expandIconPosition: "left"
                width: Math.min(480, parent.width)
                model: [{name: "a", title: "基本信息", content: "项目名称与描述"}, {name: "b", title: "高级设置", content: "组件主题与配置"}]
            }
        }
    }
    ExampleSection {
        componentName: "ECollapse"

        ExampleCase {
            objectName: "showcaseECollapse2"
            title: ELocale.texts.CollapseGuard
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESwitch {
                        id: allow

                        text: "允许展开"
                        checked: true
                    }
                    ECollapse {
                        width: Math.min(480, parent.width)
                        beforeCollapse: function() { return allow.checked }
                        model: [{name: "a", title: "高级设置", content: "展开前先验证权限。"}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESwitch {
                    id: allow

                    text: "允许展开"
                    checked: true
                }
                ECollapse {
                    width: Math.min(480, parent.width)
                    beforeCollapse: function() { return allow.checked }
                    model: [{name: "a", title: "高级设置", content: "展开前先验证权限。"}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStatistic"

        ExampleCase {
            objectName: "showcaseEStatistic0"
            title: ELocale.texts.CurrencyStatistic
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatistic {
                    prefix: "$"
                    precision: 2
                    title: "累计金额"
                    value: 1234567.89
                    groupSeparator: ","
                }
            `)

            EStatistic {
                prefix: "$"
                precision: 2
                title: "累计金额"
                value: 1234567.89
                groupSeparator: ","
            }
        }
    }
    ExampleSection {
        componentName: "EStatistic"

        ExampleCase {
            objectName: "showcaseEStatistic1"
            title: ELocale.texts.FormattedStatistic
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatistic {
                    title: "总访问量"
                    value: 2560000
                    formatter: function(value) { return (value / 1000000).toFixed(2) + " M" }
                }
            `)

            EStatistic {
                title: "总访问量"
                value: 2560000
                formatter: function(value) { return (value / 1000000).toFixed(2) + " M" }
            }
        }
    }
    ExampleSection {
        componentName: "EStatistic"

        ExampleCase {
            objectName: "showcaseEStatistic2"
            title: ELocale.texts.StatisticSuffix
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatistic {
                    suffix: "%"
                    value: 98.6
                    precision: 1
                    title: "完成率"
                    prefixDelegate: Component {
                        EIcon {
                            iconSize: 24
                            color: "#21B6A8"
                            iconSource: EIcons.CHECK
                        }
                    }
                }
            `)

            EStatistic {
                suffix: "%"
                value: 98.6
                precision: 1
                title: "完成率"
                prefixDelegate: Component {
                    EIcon {
                        iconSize: 24
                        color: "#21B6A8"
                        iconSource: EIcons.CHECK
                    }
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECountdown"

        ExampleCase {
            objectName: "showcaseECountdown0"
            title: ELocale.texts.MinuteCountdown
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECountdown {
                    title: "倒计时"
                    format: "mm:ss"
                    value: Date.now() + 120000
                }
            `)

            ECountdown {
                title: "倒计时"
                format: "mm:ss"
                value: Date.now() + 120000
            }
        }
    }
    ExampleSection {
        componentName: "ECountdown"

        ExampleCase {
            objectName: "showcaseECountdown1"
            title: ELocale.texts.CountdownCompletion
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ECountdown {
                        id: timer

                        format: "ss"
                        title: "等待时间"
                        value: Date.now() + 15000

                        onFinish: result.text = "已到期"
                    }
                    EText {
                        id: result

                        text: "等待计时结束"
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ECountdown {
                    id: timer

                    format: "ss"
                    title: "等待时间"
                    value: Date.now() + 15000

                    onFinish: result.text = "已到期"
                }
                EText {
                    id: result

                    text: "等待计时结束"
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECountdown"

        ExampleCase {
            objectName: "showcaseECountdown2"
            title: ELocale.texts.RestartCountdown
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ECountdown {
                        id: timer

                        format: "mm:ss"
                        value: Date.now() + 60000
                    }
                    EButton {
                        text: "重新计时"

                        onClicked: timer.value = Date.now() + 60000
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ECountdown {
                    id: timer

                    format: "mm:ss"
                    value: Date.now() + 60000
                }
                EButton {
                    text: "重新计时"

                    onClicked: timer.value = Date.now() + 60000
                }
            }
        }
    }
    ExampleSection {
        componentName: "ECountdown"

        ExampleCase {
            objectName: "showcaseECountdown3"
            title: ELocale.texts.DayCountdown
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ECountdown {
                    title: "距发布还有"
                    format: "DD [天] HH:mm:ss"
                    value: Date.now() + 93600000
                }
            `)

            ECountdown {
                title: "距发布还有"
                format: "DD [天] HH:mm:ss"
                value: Date.now() + 93600000
            }
        }
    }
    ExampleSection {
        componentName: "ESegmented"

        ExampleCase {
            objectName: "showcaseESegmented0"
            title: ELocale.texts.BlockSegmentedControl
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESegmented {
                    block: true
                    width: Math.min(480, parent.width)
                    model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                }
            `)

            ESegmented {
                block: true
                width: Math.min(480, parent.width)
                model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
            }
        }
    }
    ExampleSection {
        componentName: "ESegmented"

        ExampleCase {
            objectName: "showcaseESegmented1"
            title: ELocale.texts.VerticalSegmentedControl
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                ESegmented {
                    currentIndex: 1
                    direction: "vertical"
                    model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                }
            `)

            ESegmented {
                currentIndex: 1
                direction: "vertical"
                model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
            }
        }
    }
    ExampleSection {
        componentName: "ESegmented"

        ExampleCase {
            objectName: "showcaseESegmented2"
            title: ELocale.texts.SegmentSizes
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    ESegmented {
                        size: EStyle.Small
                        model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                    }
                    ESegmented {
                        size: EStyle.Medium
                        model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                    }
                    ESegmented {
                        size: EStyle.Large
                        model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                ESegmented {
                    size: EStyle.Small
                    model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                }
                ESegmented {
                    size: EStyle.Medium
                    model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                }
                ESegmented {
                    size: EStyle.Large
                    model: [{label: "QML", value: "qml"}, {label: "Qt", value: "qt"}, {label: "C++", value: "cpp", disabled: true}]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStatusView"

        ExampleCase {
            title: ELocale.texts.NormalState
            objectName: "showcaseEStatusView0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatusView {
                    height: 220
                    state: "normal"
                    errorTitle: "暂时无法加载"
                    errorText: "请检查数据后重试。"
                    width: Math.min(480, parent.width)

                    EText {
                        text: "项目内容已经就绪。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            `)

            EStatusView {
                height: 220
                state: "normal"
                errorTitle: "暂时无法加载"
                errorText: "请检查数据后重试。"
                width: Math.min(480, parent.width)

                EText {
                    text: "项目内容已经就绪。"
                    width: parent.width
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStatusView"

        ExampleCase {
            title: ELocale.texts.LoadingState
            objectName: "showcaseEStatusView1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatusView {
                    height: 220
                    state: "loading"
                    errorTitle: "暂时无法加载"
                    errorText: "请检查数据后重试。"
                    width: Math.min(480, parent.width)

                    EText {
                        text: "项目内容已经就绪。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            `)

            EStatusView {
                height: 220
                state: "loading"
                errorTitle: "暂时无法加载"
                errorText: "请检查数据后重试。"
                width: Math.min(480, parent.width)

                EText {
                    text: "项目内容已经就绪。"
                    width: parent.width
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStatusView"

        ExampleCase {
            title: ELocale.texts.EmptyState
            objectName: "showcaseEStatusView2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatusView {
                    height: 220
                    state: "empty"
                    errorTitle: "暂时无法加载"
                    errorText: "请检查数据后重试。"
                    width: Math.min(480, parent.width)

                    EText {
                        text: "项目内容已经就绪。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            `)

            EStatusView {
                height: 220
                state: "empty"
                errorTitle: "暂时无法加载"
                errorText: "请检查数据后重试。"
                width: Math.min(480, parent.width)

                EText {
                    text: "项目内容已经就绪。"
                    width: parent.width
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    ExampleSection {
        componentName: "EStatusView"

        ExampleCase {
            objectName: "showcaseEStatusView3"
            title: ELocale.texts.ErrorAndRetry
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EStatusView {
                    height: 220
                    state: "error"
                    errorTitle: "暂时无法加载"
                    errorText: "请检查数据后重试。"
                    width: Math.min(480, parent.width)

                    onRetry: state = "normal"

                    EText {
                        text: "项目内容已经就绪。"
                        width: parent.width
                        wrapMode: Text.WordWrap
                    }
                }
            `)

            EStatusView {
                height: 220
                state: "error"
                errorTitle: "暂时无法加载"
                errorText: "请检查数据后重试。"
                width: Math.min(480, parent.width)

                onRetry: state = "normal"

                EText {
                    text: "项目内容已经就绪。"
                    width: parent.width
                    wrapMode: Text.WordWrap
                }
            }
        }
    }
    ExampleSection {
        componentName: "EImageViewer"

        ExampleCase {
            objectName: "showcaseEImageViewer0"
            title: ELocale.texts.SingleImageViewer
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "查看图片"

                        onClicked: viewer.openPreview(viewer.initialIndex)
                    }
                    EImageViewer {
                        id: viewer

                        visible: false
                        urlList: ["qrc:/example/eui-mark.svg"]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "查看图片"

                    onClicked: viewer.openPreview(viewer.initialIndex)
                }
                EImageViewer {
                    id: viewer

                    visible: false
                    urlList: ["qrc:/example/eui-mark.svg"]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EImageViewer"

        ExampleCase {
            objectName: "showcaseEImageViewer1"
            title: ELocale.texts.GalleryStartIndex
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "从第二张开始"

                        onClicked: viewer.openPreview(viewer.initialIndex)
                    }
                    EImageViewer {
                        id: viewer

                        visible: false
                        infinite: false
                        initialIndex: 1
                        urlList: ["qrc:/example/eui-mark.svg", "qrc:/example/eui-mark.svg"]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "从第二张开始"

                    onClicked: viewer.openPreview(viewer.initialIndex)
                }
                EImageViewer {
                    id: viewer

                    visible: false
                    infinite: false
                    initialIndex: 1
                    urlList: ["qrc:/example/eui-mark.svg", "qrc:/example/eui-mark.svg"]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EImageViewer"

        ExampleCase {
            title: ELocale.texts.ViewerOptions
            objectName: "showcaseEImageViewer2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "查看预览"

                        onClicked: viewer.openPreview(viewer.initialIndex)
                    }
                    EImageViewer {
                        id: viewer

                        maxScale: 2
                        minScale: 0.5
                        visible: false
                        showProgress: false
                        hideOnClickModal: true
                        urlList: ["qrc:/example/eui-mark.svg"]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "查看预览"

                    onClicked: viewer.openPreview(viewer.initialIndex)
                }
                EImageViewer {
                    id: viewer

                    maxScale: 2
                    minScale: 0.5
                    visible: false
                    showProgress: false
                    hideOnClickModal: true
                    urlList: ["qrc:/example/eui-mark.svg"]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EImageViewer"

        ExampleCase {
            objectName: "showcaseEImageViewer3"
            title: ELocale.texts.ViewerZoomSteps
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                Column {
                    spacing: 16
                    width: parent.width

                    EButton {
                        text: "查看图片"

                        onClicked: viewer.openPreview(viewer.initialIndex)
                    }
                    EImageViewer {
                        id: viewer

                        zoomRate: 1.5
                        visible: false
                        closeOnPressEscape: true
                        urlList: ["qrc:/example/eui-mark.svg"]
                    }
                }
            `)

            Column {
                spacing: 16
                width: parent.width

                EButton {
                    text: "查看图片"

                    onClicked: viewer.openPreview(viewer.initialIndex)
                }
                EImageViewer {
                    id: viewer

                    zoomRate: 1.5
                    visible: false
                    closeOnPressEscape: true
                    urlList: ["qrc:/example/eui-mark.svg"]
                }
            }
        }
    }
    ExampleSection {
        componentName: "EDescriptions"

        ExampleCase {
            objectName: "showcaseEDescriptions0"
            title: ELocale.texts.VerticalDescriptions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EDescriptions {
                    columns: 2
                    title: "项目资料"
                    width: parent.width
                    direction: "vertical"
                    model: [{label: "名称", value: "EUI"}, {label: "版本", value: "1.0"}, {label: "平台", value: "Windows / macOS", span: 2}]
                }
            `)

            EDescriptions {
                columns: 2
                title: "项目资料"
                width: parent.width
                direction: "vertical"
                model: [{label: "名称", value: "EUI"}, {label: "版本", value: "1.0"}, {label: "平台", value: "Windows / macOS", span: 2}]
            }
        }
    }
    ExampleSection {
        componentName: "EDescriptions"

        ExampleCase {
            objectName: "showcaseEDescriptions1"
            title: ELocale.texts.CompactDescriptions
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EDescriptions {
                    columns: 1
                    border: false
                    labelWidth: 80
                    size: EStyle.Small
                    width: parent.width
                    model: [{label: "名称", value: "EUI"}, {label: "版本", value: "1.0"}, {label: "平台", value: "Windows / macOS", span: 2}]
                }
            `)

            EDescriptions {
                columns: 1
                border: false
                labelWidth: 80
                size: EStyle.Small
                width: parent.width
                model: [{label: "名称", value: "EUI"}, {label: "版本", value: "1.0"}, {label: "平台", value: "Windows / macOS", span: 2}]
            }
        }
    }
    ExampleSection {
        componentName: "EDescriptions"

        ExampleCase {
            objectName: "showcaseEDescriptions2"
            title: ELocale.texts.DescriptionSpans
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EDescriptions {
                    columns: 2
                    extra: "已发布"
                    title: "项目资料"
                    width: parent.width
                    model: [{label: "项目", value: "EUI", rowspan: 2}, {label: "版本", value: "1.0"}, {label: "平台", value: "Qt Quick"}]
                }
            `)

            EDescriptions {
                columns: 2
                extra: "已发布"
                title: "项目资料"
                width: parent.width
                model: [{label: "项目", value: "EUI", rowspan: 2}, {label: "版本", value: "1.0"}, {label: "平台", value: "Qt Quick"}]
            }
        }
    }
    ExampleSection {
        componentName: "EInfiniteScroll"

        ExampleCase {
            title: ELocale.texts.LoadOnScroll
            objectName: "showcaseEInfiniteScroll0"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EInfiniteScroll {
                    height: 200
                    distance: 32
                    immediate: false
                    width: Math.min(480, parent.width)
                    model: ["项目 1", "项目 2", "项目 3", "项目 4", "项目 5", "项目 6"]

                    onLoadMore: { model = model.concat(["新增项目 " + (count + 1)])
                        finishLoad(count >= 12) }
                }
            `)

            EInfiniteScroll {
                height: 200
                distance: 32
                immediate: false
                width: Math.min(480, parent.width)
                model: ["项目 1", "项目 2", "项目 3", "项目 4", "项目 5", "项目 6"]

                onLoadMore: { model = model.concat(["新增项目 " + (count + 1)])
                    finishLoad(count >= 12) }
            }
        }
    }
    ExampleSection {
        componentName: "EInfiniteScroll"

        ExampleCase {
            title: ELocale.texts.FinishedList
            objectName: "showcaseEInfiniteScroll1"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EInfiniteScroll {
                    height: 200
                    finished: true
                    model: ["项目 1", "项目 2", "项目 3"]
                    width: Math.min(480, parent.width)
                }
            `)

            EInfiniteScroll {
                height: 200
                finished: true
                model: ["项目 1", "项目 2", "项目 3"]
                width: Math.min(480, parent.width)
            }
        }
    }
    ExampleSection {
        componentName: "EInfiniteScroll"

        ExampleCase {
            title: ELocale.texts.LoadingList
            objectName: "showcaseEInfiniteScroll2"
            code: GalleryStyle.sourceCode(`
                import EUI 1.0
                import QtQuick 2.15

                EInfiniteScroll {
                    delay: 500
                    height: 180
                    distance: 64
                    loading: true
                    width: Math.min(480, parent.width)
                    model: ["项目 1", "项目 2", "项目 3", "项目 4"]
                }
            `)

            EInfiniteScroll {
                delay: 500
                height: 180
                distance: 64
                loading: true
                width: Math.min(480, parent.width)
                model: ["项目 1", "项目 2", "项目 3", "项目 4"]
            }
        }
    }
}
