#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMetaEnum>
#include <QDirIterator>
#include <QImageReader>
#include <QJsonDocument>

#include "ExampleData.h"
#include "../../source/enum/EIcons.h"
#include "../../source/enum/EStyle.h"
#include "../../source/utils/ECodeTools.h"

namespace
{
struct Sample
{
    const char *key;
    const char *chinese;
    const char *english;
};

const Sample ModelTexts[] = {
    {"Qt", "Qt", "Qt"},
    {"C", "C++", "C++"},
    {"Day", "日", "Day"},
    {"Age", "年龄", "Age"},
    {"Bob", "李四", "Bob"},
    {"Qml", "QML", "QML"},
    {"Src", "src", "src"},
    {"Week", "周", "Week"},
    {"Year", "年", "Year"},
    {"Bob2", "Bob", "Bob"},
    {"City", "城市", "City"},
    {"Copy", "复制", "Copy"},
    {"Date", "日期", "Date"},
    {"Done", "完成", "Done"},
    {"Edit", "编辑", "Edit"},
    {"Grid", "网格", "Grid"},
    {"Home", "首页", "Home"},
    {"List", "列表", "List"},
    {"Name", "姓名", "Name"},
    {"Role", "角色", "Role"},
    {"Month", "月", "Month"},
    {"Name2", "名称", "Name"},
    {"Alice", "张三", "Alice"},
    {"Carol", "王五", "Carol"},
    {"Docs", "docs", "docs"},
    {"Files", "文件", "Files"},
    {"Main", "main", "main"},
    {"Value", "数值", "Value"},
    {"City2", "city", "city"},
    {"Delete", "删除", "Delete"},
    {"Status", "状态", "Status"},
    {"David", "David", "David"},
    {"Style", "style", "style"},
    {"Alice2", "Alice", "Alice"},
    {"Archive", "归档", "Archive"},
    {"Beijing", "北京", "Beijing"},
    {"Reports", "报表", "Reports"},
    {"Support", "支持", "Support"},
    {"Value2", "value", "value"},
    {"Welcome", "欢迎", "Welcome"},
    {"Enabled", "已启用", "Enabled"},
    {"Node1", "节点 %1", "Node %1"},
    {"Value2026", "2026", "2026"},
    {"Activity", "活动", "Activity"},
    {"Feedback", "反馈", "Feedback"},
    {"Hangzhou", "杭州", "Hangzhou"},
    {"Overview", "概览", "Overview"},
    {"Products", "产品", "Products"},
    {"Projects", "项目", "Projects"},
    {"Recent", "recent", "recent"},
    {"Settings", "设置", "Settings"},
    {"Shanghai", "上海", "Shanghai"},
    {"Shenzhen", "深圳", "Shenzhen"},
    {"Disabled2", "禁用", "Disabled"},
    {"Continue", "继续操作", "Continue"},
    {"Disabled4", "已禁用", "Disabled"},
    {"Favorites", "收藏", "Favorites"},
    {"Guangzhou", "广州", "Guangzhou"},
    {"Resources", "资源", "Resources"},
    {"Charlie", "Charlie", "Charlie"},
    {"EastChina", "华东", "East China"},
    {"Option1", "选项 %1", "Option %1"},
    {"Record1", "记录 %1", "Record %1"},
    {"Workspace", "工作区", "Workspace"},
    {"Components", "组件", "Components"},
    {"Navigation", "导航", "Navigation"},
    {"Solutions", "解决方案", "Solutions"},
    {"Support2", "support", "support"},
    {"Workspace2", "工作台", "Workspace"},
    {"MainQml", "main.qml", "main.qml"},
    {"QtQuick", "Qt Quick", "Qt Quick"},
    {"SouthChina", "华南", "South China"},
    {"SourceCode", "源代码", "Source code"},
    {"Consistency", "一致性", "Consistency"},
    {"Disabled3", "disabled", "disabled"},
    {"Products2", "products", "products"},
    {"AgentsMd", "AGENTS.md", "AGENTS.md"},
    {"DisabledRow", "禁用行", "Disabled row"},
    {"Year2026", "year-2026", "year-2026"},
    {"InternalUse", "内部资料", "Internal use"},
    {"Configuration", "配置", "Configuration"},
    {"Documentation", "文档", "Documentation"},
    {"Favorites2", "favorites", "favorites"},
    {"Notifications", "通知", "Notifications"},
    {"Solutions2", "solutions", "solutions"},
    {"Administrator", "管理员", "Administrator"},
    {"DisabledMenu", "禁用菜单", "Disabled menu"},
    {"QtCreator", "Qt Creator", "Qt Creator"},
    {"QtWidgets", "Qt Widgets", "Qt Widgets"},
    {"AddComponents", "添加组件", "Add components"},
    {"ArchivedFiles", "最近文件", "Archived files"},
    {"DisabledPanel", "禁用面板", "Disabled panel"},
    {"ProjectCreated", "创建项目", "Project created"},
    {"Value20261007", "2026-10-07", "2026-10-07"},
    {"Value20261008", "2026-10-08", "2026-10-08"},
    {"Value20261009", "2026-10-09", "2026-10-09"},
    {"Value20261010", "2026-10-10", "2026-10-10"},
    {"Consistency2", "consistency", "consistency"},
    {"BuildInterfaces", "构建界面", "Build interfaces"},
    {"NameIsRequired", "请输入名称", "Name is required"},
    {"EmailIsRequired", "邮箱不能为空", "Email is required"},
    {"InteractWithEase", "轻松交互", "Interact with ease"},
    {"Notifications2", "notifications", "notifications"},
    {"ConfigureResources", "配置资源", "Configure resources"},
    {"ConfirmAndPublish", "确认并发布", "Confirm and publish"},
    {"DiscoverComponents", "发现组件", "Discover components"},
    {"NameAndDescription", "命名与描述", "Name and description"},
    {"CodeReviewCompleted", "完成代码检查", "Code review completed"},
    {"PublishedSuccessfully", "发布成功", "Published successfully"},
    {"Value202610090900", "2026-10-09 09:00", "2026-10-09 09:00"},
    {"Value202610101030", "2026-10-10 10:30", "2026-10-10 10:30"},
    {"Value202610101200", "2026-10-10 12:00", "2026-10-10 12:00"},
    {"EnterAValidEmailAddress", "请输入有效邮箱", "Enter a valid email address"},
    {"ThisPanelCannotBeExpanded", "此项不可展开。", "This panel cannot be expanded."},
    {"ThisButtonStartsTheGuidedTour", "此按钮可以开始界面引导。", "This button starts the guided tour."},
    {"ProvideClearVisualFeedbackForEveryAction",
     "每次操作均提供明确的视觉反馈。",
     "Provide clear visual feedback for every action."},
    {"YouHaveLearnedTheMainActionsClick",
     "您已经了解主要操作，点击完成结束引导。",
     "You have learned the main actions. Click Finish to end the tour."},
    {"KeepColorsSizesTypographyAndInteractionRules",
     "颜色、尺寸、排版和交互规则保持一致，让用户快速理解界面。",
     "Keep colors, sizes, typography, and interaction rules consistent so users can understand the interface quickly."},
};

const Sample Samples[] = {
    {"EInputOtpExample",
     R"qml(EInputOtp {
    length: 6
})qml",
     R"qml(EInputOtp {
    length: 6
})qml"},
    {"EColorPickerExample",
     R"qml(EColorPicker {
    showAlpha: true
})qml",
     R"qml(EColorPicker {
    showAlpha: true
})qml"},
    {"SelectionExamplesSample4",
     R"qml(ETimePicker {
    value: "09:30:00"
})qml",
     R"qml(ETimePicker {
    value: "09:30:00"
})qml"},
    {"EColorPickerPanelExample",
     R"qml(EColorPickerPanel {
    showAlpha: true
})qml",
     R"qml(EColorPickerPanel {
    showAlpha: true
})qml"},
    {"EDatePickerPanelExample",
     R"qml(EDatePickerPanel {
    value: "2026-10-10"
})qml",
     R"qml(EDatePickerPanel {
    value: "2026-10-10"
})qml"},
    {"LayoutExamplesSample8",
     R"qml(ETypography {
    level: 2
    text: qsTr("标题")
})qml",
     R"qml(ETypography {
    level: 2
    text: qsTr("Heading")
})qml"},
    {"LayoutExamplesSample2",
     R"qml(EContainer {
    EHeader {
    }
    EMain {
    }
    EFooter {
    }
})qml",
     R"qml(EContainer {
    EHeader {
    }
    EMain {
    }
    EFooter {
    }
})qml"},
    {"DataExamplesSample1",
     R"qml(EAvatar {
    size: 48
    text: "EU"
    shape: "circle"
})qml",
     R"qml(EAvatar {
    size: 48
    text: "EU"
    shape: "circle"
})qml"},
    {"EDateTimePickerExample",
     R"qml(EDateTimePicker {
    value: "2026-10-10 09:30:00"
})qml",
     R"qml(EDateTimePicker {
    value: "2026-10-10 09:30:00"
})qml"},
    {"ETimeSelectExample",
     R"qml(ETimeSelect {
    step: 30
    end: "18:00"
    start: "09:00"
})qml",
     R"qml(ETimeSelect {
    step: 30
    end: "18:00"
    start: "09:00"
})qml"},
    {"EUploadExample",
     R"qml(EUpload {
    limit: 5
    width: 520
    action: uploadUrl.text
})qml",
     R"qml(EUpload {
    limit: 5
    width: 520
    action: uploadUrl.text
})qml"},
    {"OverlayExamplesSample2",
     R"qml(EDrawer {
    title: "项目详情"
    edge: Qt.RightEdge
})qml",
     R"qml(EDrawer {
    edge: Qt.RightEdge
    title: "Project details"
})qml"},
    {"ShortcutPicker",
     R"qml(EShortcutPicker {
    width: 360
    sequence: "Ctrl+Shift+P"
})qml",
     R"qml(EShortcutPicker {
    width: 360
    sequence: "Ctrl+Shift+P"
})qml"},
    {"SelectionExamplesSample2",
     R"qml(ERate {
    value: 3.5
    allowHalf: true
    clearable: true
})qml",
     R"qml(ERate {
    value: 3.5
    allowHalf: true
    clearable: true
})qml"},
    {"NavigationExamplesSample0",
     R"qml(ETabs {
    currentIndex: 0
    model: ["概览", "设置"]
})qml",
     R"qml(ETabs {
    currentIndex: 0
    model: ["Overview", "Settings"]
})qml"},
    {"NavigationExamplesSample1",
     R"qml(EBreadcrumb {
    model: ["首页", "组件", "导航"]
})qml",
     R"qml(EBreadcrumb {
    model: ["Home", "Components", "Navigation"]
})qml"},
    {"QrCode",
     R"qml(EQRCode {
    width: 180
    height: 180
    text: "https://example.com"
})qml",
     R"qml(EQRCode {
    width: 180
    height: 180
    text: "https://example.com"
})qml"},
    {"ExtendedNavigationExamplesSample3",
     R"qml(EBacktop {
    target: scroll
    visibilityHeight: 100
})qml",
     R"qml(EBacktop {
    target: scroll
    visibilityHeight: 100
})qml"},
    {"ExtendedNavigationExamplesSample0",
     R"qml(EPageHeader {
    title: "返回"
    content: "项目详情"
})qml",
     R"qml(EPageHeader {
    title: "Back"
    content: "Project details"
})qml"},
    {"EMentionExample",
     R"qml(EMention {
    width: 360
    options: exampleData.models.Members
})qml",
     R"qml(EMention {
    width: 360
    options: exampleData.models.Members
})qml"},
    {"OverlayExamplesSample1",
     R"qml(EDialog {
    title: "保存修改"
    text: "是否保存当前修改？"
})qml",
     R"qml(EDialog {
    title: "Save changes"
    text: "Save your current changes?"
})qml"},
    {"NavigationExamplesSample2",
     R"qml(ESteps {
    active: 1
    model: ["创建", "配置", "完成"]
})qml",
     R"qml(ESteps {
    active: 1
    model: ["Create", "Configuration", "Done"]
})qml"},
    {"DataExamplesSample2",
     R"qml(EBadge {
    max: 99
    value: 120

    EButton {
        text: qsTr("消息")
    }
})qml",
     R"qml(EBadge {
    max: 99
    value: 120

    EButton {
        text: qsTr("Messages")
    }
})qml"},
    {"OverlayExamplesSample0",
     R"qml(EAlert {
    title: "保存成功"
    styleType: EStyle.Success
})qml",
     R"qml(EAlert {
    styleType: EStyle.Success
    title: "Saved successfully"
})qml"},
    {"OverlayExamplesSample4",
     R"qml(EPopconfirm {
    text: "确定删除？"
    reference: deleteButton
})qml",
     R"qml(EPopconfirm {
    reference: deleteButton
    text: "Delete this item?"
})qml"},
    {"LayoutExamplesSample6",
     R"qml(ESpace {
    size: 16

    EButton {
        text: "One"
    }
    EButton {
        text: "Two"
    }
})qml",
     R"qml(ESpace {
    size: 16

    EButton {
        text: "One"
    }
    EButton {
        text: "Two"
    }
})qml"},
    {"SelectLargeData",
     R"qml(ESelect {
    clearable: true
    model: exampleData.models.ManyOptions
})qml",
     R"qml(ESelect {
    clearable: true
    model: exampleData.models.ManyOptions
})qml"},
    {"ExtendedDataExamplesSample4",
     R"qml(EImage {
    preview: true
    source: "qrc:/example/images/gallery/lake.jpg"
})qml",
     R"qml(EImage {
    preview: true
    source: "qrc:/example/images/gallery/lake.jpg"
})qml"},
    {"LayoutExamplesSample9",
     R"qml(EConfigProvider {
    size: EStyle.Small

    EButton {
        text: "Button"
    }
})qml",
     R"qml(EConfigProvider {
    size: EStyle.Small

    EButton {
        text: "Button"
    }
})qml"},
    {"SelectionExamplesSample0",
     R"qml(ESelect {
    clearable: true
    model: ["北京", "上海", "深圳"]
})qml",
     R"qml(ESelect {
    clearable: true
    model: ["Beijing", "Shanghai", "Shenzhen"]
})qml"},
    {"ERadioGroupExample",
     R"qml(ERadioGroup {
    value: "QML"
    options: exampleData.models.Languages
})qml",
     R"qml(ERadioGroup {
    value: "QML"
    options: exampleData.models.Languages
})qml"},
    {"EInputTagExample",
     R"qml(EInputTag {
    max: 5
    width: 520
    value: exampleData.models.InitialTags
})qml",
     R"qml(EInputTag {
    max: 5
    width: 520
    value: exampleData.models.InitialTags
})qml"},
    {"OverlayExamplesSample3",
     R"qml(EPopover {
    title: "详情"
    text: "说明内容"
    reference: button
})qml",
     R"qml(EPopover {
    title: "Details"
    reference: button
    text: "Additional information"
})qml"},
    {"RangeSlider",
     R"qml(ERangeSlider {
    from: 0
    to: 100
    stepSize: 5
    first.value: 20
    second.value: 70
})qml",
     R"qml(ERangeSlider {
    from: 0
    to: 100
    stepSize: 5
    first.value: 20
    second.value: 70
})qml"},
    {"MediaPlayer",
     R"qml(EMediaPlayer {
    width: 480
    height: 300
    source: "file:///C:/media/video.mp4"
})qml",
     R"qml(EMediaPlayer {
    width: 480
    height: 300
    source: "file:///C:/media/video.mp4"
})qml"},
    {"ExtendedNavigationExamplesSample1",
     R"qml(EAffix {
    offset: 0
    target: flickable

    EButton {
        text: "固定操作"
    }
})qml",
     R"qml(EAffix {
    offset: 0
    target: flickable

    EButton {
        text: "Sticky action"
    }
})qml"},
    {"NavigationExamplesSample3",
     R"qml(EDropdown {
    text: "操作"
    model: [{
        "text": "编辑",
        "command": "edit"
    }]
})qml",
     R"qml(EDropdown {
    text: "Actions"
    model: [{
        "text": "Edit",
        "command": "edit"
    }]
})qml"},
    {"TreeLargeData",
     R"qml(ETree {
    height: 260
    width: parent.width
    model: exampleData.models.ManyNodes
})qml",
     R"qml(ETree {
    height: 260
    width: parent.width
    model: exampleData.models.ManyNodes
})qml"},
    {"FormExamplesSample7",
     R"qml(ESwitch {
    text: qsTr("默认")
}
ESwitch {
    checked: true
    text: qsTr("已开启")
})qml",
     R"qml(ESwitch {
    text: qsTr("Default")
}
ESwitch {
    checked: true
    text: qsTr("Enabled")
})qml"},
    {"LayoutExamplesSample5",
     R"qml(EScrollbar {
    height: 160

    Column {
        Repeater {
            model: 20

            EText {
                text: index
            }
        }
    }
})qml",
     R"qml(EScrollbar {
    height: 160

    Column {
        Repeater {
            model: 20

            EText {
                text: index
            }
        }
    }
})qml"},
    {"SelectionExamplesSample1",
     R"qml(EAutocomplete {
    clearable: true
    model: ["QML", "Qt Quick", "Qt Widgets"]
})qml",
     R"qml(EAutocomplete {
    clearable: true
    model: ["QML", "Qt Quick", "Qt Widgets"]
})qml"},
    {"Chart",
     R"qml(EChart {
    type: 1
    height: 260
    width: parent.width
    model: [12, 28, 18, 42, 32, 54, 38]
})qml",
     R"qml(EChart {
    type: 1
    height: 260
    width: parent.width
    model: [12, 28, 18, 42, 32, 54, 38]
})qml"},
    {"DataExamplesSample4",
     R"qml(EEmpty {
    description: qsTr("没有匹配的结果")

    EButton {
        text: qsTr("重新搜索")
    }
})qml",
     R"qml(EEmpty {
    description: qsTr("No matching results")

    EButton {
        text: qsTr("Search again")
    }
})qml"},
    {"LayoutExamplesSample4",
     R"qml(ELink {
    text: qsTr("EUI 首页")
    href: "https://easy-ui.cn/"
})qml",
     R"qml(ELink {
    text: qsTr("EUI Home")
    href: "https://easy-ui.cn/"
})qml"},
    {"ExtendedNavigationExamplesSample2",
     R"qml(EAnchor {
    target: scroll
    model: [{
        "text": "概览",
        "target": overview
    }]
})qml",
     R"qml(EAnchor {
    target: scroll
    model: [{
        "text": "Overview",
        "target": overview
    }]
})qml"},
    {"ETreeSelectExample",
     R"qml(ETreeSelect {
    clearable: true
    leavesOnly: true
    data: exampleData.models.Regions
})qml",
     R"qml(ETreeSelect {
    clearable: true
    leavesOnly: true
    data: exampleData.models.Regions
})qml"},
    {"ExtendedDataExamplesSample0",
     R"qml(ECalendar {
    value: "2026-10-10"
    maxDate: "2026-12-31"
    minDate: "2026-01-01"
})qml",
     R"qml(ECalendar {
    value: "2026-10-10"
    maxDate: "2026-12-31"
    minDate: "2026-01-01"
})qml"},
    {"SelectionExamplesSample3",
     R"qml(EDatePicker {
    value: "2026-10-10"
    maxDate: "2026-12-31"
    minDate: "2026-01-01"
})qml",
     R"qml(EDatePicker {
    value: "2026-10-10"
    maxDate: "2026-12-31"
    minDate: "2026-01-01"
})qml"},
    {"DataExamplesSample5",
     R"qml(ESkeleton {
    rows: 3
    loading: true
    animated: true

    EText {
        text: qsTr("加载完成")
    }
})qml",
     R"qml(ESkeleton {
    rows: 3
    loading: true
    animated: true

    EText {
        text: qsTr("Loading complete")
    }
})qml"},
    {"ExtendedDataExamplesSample11",
     R"qml(EStatistic {
    precision: 2
    prefix: "¥ "
    value: 12345.7
    title: qsTr("今日成交")
})qml",
     R"qml(EStatistic {
    precision: 2
    prefix: "¥ "
    value: 12345.7
    title: qsTr("Today's sales")
})qml"},
    {"FormExamplesSample3",
     R"qml(ERadioButton {
    checked: true
    text: qsTr("北京")
}
ERadioButton {
    text: qsTr("上海")
})qml",
     R"qml(ERadioButton {
    checked: true
    text: qsTr("Beijing")
}
ERadioButton {
    text: qsTr("Shanghai")
})qml"},
    {"ECascaderExample",
     R"qml(ECascader {
    options: exampleData.models.Regions
    value: exampleData.models.InitialRegion
})qml",
     R"qml(ECascader {
    options: exampleData.models.Regions
    value: exampleData.models.InitialRegion
})qml"},
    {"DataExamplesSample8",
     R"qml(EPagination {
    total: 256
    pageSize: 10

    onCurrentChange: function(page) {
        console.log(page)
    }
})qml",
     R"qml(EPagination {
    total: 256
    pageSize: 10

    onCurrentChange: function(page) {
        console.log(page)
    }
})qml"},
    {"DataExamplesSample0",
     R"qml(ETag {
    closable: true
    text: qsTr("成功")
    styleType: EStyle.Success

    onClosed: visible = false
})qml",
     R"qml(ETag {
    closable: true
    text: qsTr("Success")
    styleType: EStyle.Success

    onClosed: visible = false
})qml"},
    {"ExtendedFeedbackExamplesSample0",
     R"qml(EMessageBox {
    id: box
}
EButton {
    onClicked: box.prompt("输入项目名称", "新建项目", "")
})qml",
     R"qml(EMessageBox {
    id: box
}
EButton {
    onClicked: box.prompt("Enter a project name", "New project", "")
})qml"},
    {"ExtendedDataExamplesSample6",
     R"qml(EResult {
    title: qsTr("操作成功")
    subTitle: qsTr("修改已保存。")

    EButton {
        text: qsTr("返回")
    }
})qml",
     R"qml(EResult {
    title: qsTr("Success")
    subTitle: qsTr("Your changes are saved.")

    EButton {
        text: qsTr("Back")
    }
})qml"},
    {"ExtendedDataExamplesSample1",
     R"qml(ECarousel {
    model: ["第一页", "第二页"]
    delegate: Component {
        EText {
            text: parent.modelData
        }
    }
})qml",
     R"qml(ECarousel {
    model: ["First page", "Second page"]
    delegate: Component {
        EText {
            text: parent.modelData
        }
    }
})qml"},
    {"FormExamplesSample1",
     R"qml(ECheckBoxButton {
    text: qsTr("选项一")
}
ECheckBoxButton {
    radius: 8
    checked: true
    text: qsTr("选项二")
})qml",
     R"qml(ECheckBoxButton {
    text: qsTr("Option one")
}
ECheckBoxButton {
    radius: 8
    checked: true
    text: qsTr("Option two")
})qml"},
    {"Captcha",
     R"qml(ECaptcha {
    id: captcha

    length: 5
}
EButton {
    text: qsTr("验证")

    onClicked: console.log(captcha.verify(input.text))
})qml",
     R"qml(ECaptcha {
    id: captcha

    length: 5
}
EButton {
    text: qsTr("Verify")

    onClicked: console.log(captcha.verify(input.text))
})qml"},
    {"DataExamplesSample3",
     R"qml(ECard {
    width: 360
    header: qsTr("项目状态")
    footer: qsTr("更新于今天")

    EText {
        text: qsTr("所有任务已完成")
    }
})qml",
     R"qml(ECard {
    width: 360
    footer: qsTr("Updated today")
    header: qsTr("Project status")

    EText {
        text: qsTr("All tasks are complete")
    }
})qml"},
    {"ECheckBoxGroupExample",
     R"qml(ECheckBoxGroup {
    max: 2
    options: exampleData.models.Languages
    value: exampleData.models.InitialCheckedTags
})qml",
     R"qml(ECheckBoxGroup {
    max: 2
    options: exampleData.models.Languages
    value: exampleData.models.InitialCheckedTags
})qml"},
    {"ETransferExample",
     R"qml(ETransfer {
    width: 620
    data: exampleData.models.TransferItems
    value: exampleData.models.InitialTransferredKeys
})qml",
     R"qml(ETransfer {
    width: 620
    data: exampleData.models.TransferItems
    value: exampleData.models.InitialTransferredKeys
})qml"},
    {"BasicExamplesSample1",
     R"qml(ESwitch {
    id: disabledSwitch

    text: qsTr("禁用")
}
EButton {
    text: qsTr("按钮")
    enabled: !disabledSwitch.checked
})qml",
     R"qml(ESwitch {
    id: disabledSwitch

    text: qsTr("Disabled")
}
EButton {
    text: qsTr("Button")
    enabled: !disabledSwitch.checked
})qml"},
    {"ExtendedDataExamplesSample3",
     R"qml(EDescriptions {
    columns: 2
    title: qsTr("用户信息")
    model: [{
        "label": qsTr("姓名"),
        "value": qsTr("张三")
    }]
})qml",
     R"qml(EDescriptions {
    columns: 2
    title: qsTr("User information")
    model: [{
        "label": qsTr("Name"),
        "value": qsTr("Alice")
    }]
})qml"},
    {"ExtendedDataExamplesSample5",
     R"qml(EInfiniteScroll {
    model: items

    onLoadMore: {
        items = items.concat(nextItems)
        finishLoad(items.length >= 40)
    }
})qml",
     R"qml(EInfiniteScroll {
    model: items

    onLoadMore: {
        items = items.concat(nextItems)
        finishLoad(items.length >= 40)
    }
})qml"},
    {"ExtendedDataExamplesSample10",
     R"qml(ETour {
    id: tour

    steps: [{
        "target": startButton,
        "title": qsTr("开始"),
        "description": qsTr("点击此按钮。")
    }]
})qml",
     R"qml(ETour {
    id: tour

    steps: [{
        "target": startButton,
        "title": qsTr("Start"),
        "description": qsTr("Click this button.")
    }]
})qml"},
    {"ExtendedDataExamplesSample9",
     R"qml(ETimeline {
    model: [{
        "content": qsTr("发布成功"),
        "timestamp": "2026-10-10",
        "styleType": EStyle.Success
    }]
})qml",
     R"qml(ETimeline {
    model: [{
        "content": qsTr("Published successfully"),
        "timestamp": "2026-10-10",
        "styleType": EStyle.Success
    }]
})qml"},
    {"OverlayExamplesSample5",
     R"qml(ENotification {
    id: notification
}
EButton {
    onClicked: notification.open({
        "title": "完成",
        "message": "保存成功"
    })
})qml",
     R"qml(ENotification {
    id: notification
}
EButton {
    onClicked: notification.open({
        "title": "Done",
        "message": "Saved successfully"
    })
})qml"},
    {"ExtendedDataExamplesSample12",
     R"qml(ESegmented {
    model: [qsTr("日"), qsTr("周"), qsTr("月")]

    onSelected: function(value, index) {
        console.log(value)
    }
})qml",
     R"qml(ESegmented {
    model: [qsTr("Day"), qsTr("Week"), qsTr("Month")]

    onSelected: function(value, index) {
        console.log(value)
    }
})qml"},
    {"StatusView",
     R"qml(EStatusView {
    id: status

    height: 240
    state: "loading"
    width: parent.width

    onRetry: state = "loading"

    EText {
        text: qsTr("内容已就绪")
    }
})qml",
     R"qml(EStatusView {
    id: status

    height: 240
    state: "loading"
    width: parent.width

    onRetry: state = "loading"

    EText {
        text: qsTr("Content is ready")
    }
})qml"},
    {"DataExamplesSample7",
     R"qml(ETree {
    expandedKeys: ["root"]
    model: [{
        "id": "root",
        "label": qsTr("项目"),
        "children": [{
            "id": "app",
            "label": qsTr("应用")
        }]
    }]
})qml",
     R"qml(ETree {
    expandedKeys: ["root"]
    model: [{
        "id": "root",
        "label": qsTr("Projects"),
        "children": [{
            "id": "app",
            "label": qsTr("Application")
        }]
    }]
})qml"},
    {"ExtendedDataExamplesSample2",
     R"qml(ECollapse {
    accordion: true
    model: [{
        "name": "one",
        "title": qsTr("一致性"),
        "content": qsTr("保持界面和交互一致。")
    }]
})qml",
     R"qml(ECollapse {
    accordion: true
    model: [{
        "name": "one",
        "title": qsTr("Consistency"),
        "content": qsTr("Keep the interface and interactions consistent.")
    }]
})qml"},
    {"BasicExamplesSample4",
     R"qml(ESwitch {
    id: loadingSwitch

    text: qsTr("加载")
}
EButton {
    text: qsTr("加载按钮")
    styleType: EStyle.Primary
    loading: loadingSwitch.checked
})qml",
     R"qml(ESwitch {
    id: loadingSwitch

    text: qsTr("Loading")
}
EButton {
    styleType: EStyle.Primary
    text: qsTr("Loading button")
    loading: loadingSwitch.checked
})qml"},
    {"FormExamplesSample2",
     R"qml(ERadio {
    checked: true
    text: qsTr("默认")
}
ERadio {
    text: qsTr("小号")
    size: EStyle.Small
}
ERadio {
    text: qsTr("无边框")
    borderVisible: false
})qml",
     R"qml(ERadio {
    checked: true
    text: qsTr("Default")
}
ERadio {
    size: EStyle.Small
    text: qsTr("Small")
}
ERadio {
    borderVisible: false
    text: qsTr("Without border")
})qml"},
    {"FormExamplesSample0",
     R"qml(ECheckBox {
    text: qsTr("未选中")
}
ECheckBox {
    checked: true
    text: qsTr("已选中")
}
ECheckBox {
    checked: true
    text: qsTr("无边框")
    borderVisible: false
})qml",
     R"qml(ECheckBox {
    text: qsTr("Unchecked")
}
ECheckBox {
    checked: true
    text: qsTr("Checked")
}
ECheckBox {
    checked: true
    borderVisible: false
    text: qsTr("Without border")
})qml"},
    {"MenuBar",
     R"qml(EMenuBar {
    model: [{
        "text": qsTr("文件"),
        "children": [{
            "index": "new",
            "text": qsTr("新建")
        }]
    }]

    onCommand: function(index, item) {
        console.log(index, item.text)
    }
})qml",
     R"qml(EMenuBar {
    model: [{
        "text": qsTr("File"),
        "children": [{
            "index": "new",
            "text": qsTr("New")
        }]
    }]

    onCommand: function(index, item) {
        console.log(index, item.text)
    }
})qml"},
    {"BasicExamplesSample5",
     R"qml(Flow {
    spacing: 12

    EButton {
        text: qsTr("大号")
        size: EStyle.Large
    }
    EButton {
        text: qsTr("中号")
        size: EStyle.Medium
    }
    EButton {
        text: qsTr("小号")
        size: EStyle.Small
    }
})qml",
     R"qml(Flow {
    spacing: 12

    EButton {
        size: EStyle.Large
        text: qsTr("Large")
    }
    EButton {
        size: EStyle.Medium
        text: qsTr("Medium")
    }
    EButton {
        size: EStyle.Small
        text: qsTr("Small")
    }
})qml"},
    {"TableLargeData",
     R"qml(ETable {
    height: 260
    stripe: true
    rowHeight: 46
    headerHeight: 44
    width: parent.width
    model: exampleData.models.ManyRows
    columns: exampleData.models.VirtualTableColumns
})qml",
     R"qml(ETable {
    height: 260
    stripe: true
    rowHeight: 46
    headerHeight: 44
    width: parent.width
    model: exampleData.models.ManyRows
    columns: exampleData.models.VirtualTableColumns
})qml"},
    {"BasicExamplesSample8",
     R"qml(ECopyableText {
    text: qsTr("拖动鼠标选择并复制这段文字")
}
ECopyableText {
    text: qsTr("自定义颜色")
    textColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml",
     R"qml(ECopyableText {
    text: qsTr("Drag to select and copy this text")
}
ECopyableText {
    text: qsTr("Custom color")
    textColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml"},
    {"StaggeredLayout",
     R"qml(EStaggeredLayout {
    columns: 3
    spacing: 12
    width: parent.width

    Repeater {
        model: [90, 150, 110, 180, 120, 80]

        ERectangle {
            height: modelData
            color: EPaletteManager.ThemePalette.surfacePressed(ETheme.dark)
        }
    }
})qml",
     R"qml(EStaggeredLayout {
    columns: 3
    spacing: 12
    width: parent.width

    Repeater {
        model: [90, 150, 110, 180, 120, 80]

        ERectangle {
            height: modelData
            color: EPaletteManager.ThemePalette.surfacePressed(ETheme.dark)
        }
    }
})qml"},
    {"BasicExamplesSample13",
     R"qml(ETextArea {
    placeholderText: qsTr("请输入多行内容")
    width: Math.min(parent.width, 520)
}
ETextArea {
    height: 72
    size: EStyle.Small
    text: qsTr("固定高度")
    adaptiveHeight: false
    width: Math.min(parent.width, 520)
})qml",
     R"qml(ETextArea {
    width: Math.min(parent.width, 520)
    placeholderText: qsTr("Enter multiple lines")
}
ETextArea {
    height: 72
    size: EStyle.Small
    adaptiveHeight: false
    text: qsTr("Fixed height")
    width: Math.min(parent.width, 520)
})qml"},
    {"ParallaxEffect",
     R"qml(EParallaxView {
    height: 280
    speed: 0.35
    width: parent.width
    backgroundComponent: Component {
        Image {
            source: "qrc:/example/images/gallery/tree.jpg"
            fillMode: Image.PreserveAspectCrop

            ERectangle {
                color: "#66000000"
                anchors.fill: parent
            }
        }
    }

    Repeater {
        model: 10

        EText {
            height: 48
            color: "white"
            text: index + 1
        }
    }
})qml",
     R"qml(EParallaxView {
    height: 280
    speed: 0.35
    width: parent.width
    backgroundComponent: Component {
        Image {
            source: "qrc:/example/images/gallery/tree.jpg"
            fillMode: Image.PreserveAspectCrop

            ERectangle {
                color: "#66000000"
                anchors.fill: parent
            }
        }
    }

    Repeater {
        model: 10

        EText {
            height: 48
            color: "white"
            text: index + 1
        }
    }
})qml"},
    {"FeedbackExamplesSample4",
     R"qml(EButton {
    text: qsTr("上方提示")

    EToolTip {
        text: qsTr("Top")
        visible: parent.hovered
        placement: EPlacement.Top
    }
}
EButton {
    text: qsTr("左侧提示")

    EToolTip {
        text: qsTr("Left")
        visible: parent.hovered
        placement: EPlacement.Left
    }
})qml",
     R"qml(EButton {
    text: qsTr("Top tooltip")

    EToolTip {
        text: qsTr("Top")
        visible: parent.hovered
        placement: EPlacement.Top
    }
}
EButton {
    text: qsTr("Left tooltip")

    EToolTip {
        text: qsTr("Left")
        visible: parent.hovered
        placement: EPlacement.Left
    }
})qml"},
    {"ShadowEffect",
     R"qml(ERectangle {
    radius: 8
    width: 220
    height: 100
    color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)

    EShadow {
        z: -1
        elevation: 8
        anchors.fill: parent
        radius: parent.radius
    }
    EText {
        text: qsTr("控件层次")
        anchors.centerIn: parent
    }
})qml",
     R"qml(ERectangle {
    radius: 8
    width: 220
    height: 100
    color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)

    EShadow {
        z: -1
        elevation: 8
        anchors.fill: parent
        radius: parent.radius
    }
    EText {
        text: qsTr("Elevation")
        anchors.centerIn: parent
    }
})qml"},
    {"BasicExamplesSample0",
     R"qml(Flow {
    spacing: 12

    EButton {
        text: qsTr("默认")
    }
    EButton {
        text: qsTr("主要")
        styleType: EStyle.Primary
    }
    EButton {
        plain: true
        text: qsTr("朴素")
        styleType: EStyle.Primary
    }
    EButton {
        round: true
        text: qsTr("圆角")
        styleType: EStyle.Success
    }
})qml",
     R"qml(Flow {
    spacing: 12

    EButton {
        text: qsTr("Default")
    }
    EButton {
        text: qsTr("Primary")
        styleType: EStyle.Primary
    }
    EButton {
        plain: true
        text: qsTr("Plain")
        styleType: EStyle.Primary
    }
    EButton {
        round: true
        text: qsTr("Round")
        styleType: EStyle.Success
    }
})qml"},
    {"FormExamplesSample4",
     R"qml(EInput {
    placeholderText: qsTr("基础输入框")
    width: Math.min(parent.width, 520)
}
EInput {
    clearable: true
    prefixIcon: EIcons.AT
    placeholderText: qsTr("可清空")
    width: Math.min(parent.width, 520)
}
EInput {
    text: "password"
    showPassword: true
    suffixIcon: EIcons.EYE
    width: Math.min(parent.width, 520)
})qml",
     R"qml(EInput {
    width: Math.min(parent.width, 520)
    placeholderText: qsTr("Enter a password")
}
EInput {
    clearable: true
    prefixIcon: EIcons.AT
    width: Math.min(parent.width, 520)
    placeholderText: qsTr("Small input")
}
EInput {
    text: "password"
    showPassword: true
    suffixIcon: EIcons.EYE
    width: Math.min(parent.width, 520)
})qml"},
    {"FeedbackExamplesSample2",
     R"qml(EProgress {
    value: 0.25
    width: Math.min(parent.width, 560)
}
EProgress {
    value: 0.5
    styleType: EStyle.Warning
    width: Math.min(parent.width, 560)
}
EProgress {
    value: 0.76
    styleType: EStyle.Success
    width: Math.min(parent.width, 560)
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
})qml",
     R"qml(EProgress {
    value: 0.25
    width: Math.min(parent.width, 560)
}
EProgress {
    value: 0.5
    styleType: EStyle.Warning
    width: Math.min(parent.width, 560)
}
EProgress {
    value: 0.76
    styleType: EStyle.Success
    width: Math.min(parent.width, 560)
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
})qml"},
    {"OtherExamplesSample2",
     R"qml(Item {
    width: 640
    height: 260

    EWatermark {
        zIndex: 9
        rotate: -18
        tileHeight: 72
        tileWidth: 150
        anchors.fill: parent
        gap: Qt.point(42, 30)
        offset: Qt.point(16, 10)
        content: ["EUI", qsTr("内部资料")]
        font: Qt.font({
            "pixelSize": 15,
            "bold": true
        })
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
    }
})qml",
     R"qml(Item {
    width: 640
    height: 260

    EWatermark {
        zIndex: 9
        rotate: -18
        tileHeight: 72
        tileWidth: 150
        anchors.fill: parent
        gap: Qt.point(42, 30)
        offset: Qt.point(16, 10)
        content: ["EUI", qsTr("Internal use")]
        font: Qt.font({
            "pixelSize": 15,
            "bold": true
        })
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
    }
})qml"},
    {"ApacheECharts",
     R"qml(Column {
    width: 640
    spacing: 16

    ESelect {
        id: chartType

        currentIndex: 0
        textRole: "label"
        model: exampleData.models.EChartsTypes
    }
    EECharts {
        height: 400
        width: parent.width
        option: exampleData.models.EChartsOptions[
            exampleData.models.EChartsTypes[chartType.currentIndex].value]

        onClicked: function(params) {
            console.log(params.name, params.value)
        }
    }
})qml",
     R"qml(Column {
    width: 640
    spacing: 16

    ESelect {
        id: chartType

        currentIndex: 0
        textRole: "label"
        model: exampleData.models.EChartsTypes
    }
    EECharts {
        height: 400
        width: parent.width
        option: exampleData.models.EChartsOptions[
            exampleData.models.EChartsTypes[chartType.currentIndex].value]

        onClicked: function(params) {
            console.log(params.name, params.value)
        }
    }
})qml"},
    {"ButtonProgress",
     R"qml(Column {
    spacing: 12

    EButton {
        id: taskButton

        progress: -1
        text: qsTr("开始任务")
        styleType: EStyle.Primary
        enabled: !progressTimer.running

        onClicked: {
            progress = 0
            progressTimer.start()
        }
    }
    Timer {
        id: progressTimer

        repeat: true
        interval: 120

        onTriggered: {
            taskButton.progress = Math.min(1, taskButton.progress + 0.1)
            if (taskButton.progress >= 1)
                stop()
        }
    }
})qml",
     R"qml(Column {
    spacing: 12

    EButton {
        id: taskButton

        progress: -1
        text: qsTr("Start task")
        styleType: EStyle.Primary
        enabled: !progressTimer.running

        onClicked: {
            progress = 0
            progressTimer.start()
        }
    }
    Timer {
        id: progressTimer

        repeat: true
        interval: 120

        onTriggered: {
            taskButton.progress = Math.min(1, taskButton.progress + 0.1)
            if (taskButton.progress >= 1)
                stop()
        }
    }
})qml"},
    {"FeedbackExamplesSample0",
     R"qml(ECircleProgress {
    value: 0.35
    width: 92
    height: 92
}
ECircleProgress {
    value: 0.68
    width: 92
    height: 92
    strokeWidth: 10
    styleType: EStyle.Warning
}
ECircleProgress {
    value: 0.72
    width: 110
    height: 110
    dashboard: true
    startAngle: 120
    strokeWidth: 10
    clockwise: false
    showBackground: true
    styleType: EStyle.Success
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
})qml",
     R"qml(ECircleProgress {
    value: 0.35
    width: 92
    height: 92
}
ECircleProgress {
    value: 0.68
    width: 92
    height: 92
    strokeWidth: 10
    styleType: EStyle.Warning
}
ECircleProgress {
    value: 0.72
    width: 110
    height: 110
    dashboard: true
    startAngle: 120
    strokeWidth: 10
    clockwise: false
    showBackground: true
    styleType: EStyle.Success
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
})qml"},
    {"OtherExamplesSample1",
     R"qml(EDivider {
    width: parent.width
}
EDivider {
    text: qsTr("居中标题")
    width: parent.width
}
EDivider {
    text: qsTr("左侧标题")
    width: parent.width
    contentPosition: EPlacement.Left
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
EDivider {
    lineWidth: 2
    verticalHeight: 84
    direction: EOrientation.Vertical
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml",
     R"qml(EDivider {
    width: parent.width
}
EDivider {
    width: parent.width
    text: qsTr("Divider label")
}
EDivider {
    width: parent.width
    text: qsTr("Vertical divider")
    contentPosition: EPlacement.Left
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
EDivider {
    lineWidth: 2
    verticalHeight: 84
    direction: EOrientation.Vertical
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml"},
    {"BasicExamplesSample3",
     R"qml(EButton {
    iconSource: EIcons.PENCIL
    styleType: EStyle.Primary
}
EButton {
    styleType: EStyle.Primary
    iconSource: EIcons.ARROW_UP_RIGHT_FROM_SQUARE
}
EButton {
    iconSource: EIcons.DELETE
    styleType: EStyle.Primary
}
EButton {
    text: qsTr("Search")
    styleType: EStyle.Primary
    iconSource: EIcons.MAGNIFIER
}
EButton {
    leftIcon: true
    text: qsTr("Upload")
    styleType: EStyle.Primary
    iconSource: EIcons.ARROW_UP_FROM_LINE
})qml",
     R"qml(EButton {
    iconSource: EIcons.PENCIL
    styleType: EStyle.Primary
}
EButton {
    styleType: EStyle.Primary
    iconSource: EIcons.ARROW_UP_RIGHT_FROM_SQUARE
}
EButton {
    iconSource: EIcons.DELETE
    styleType: EStyle.Primary
}
EButton {
    text: qsTr("Search")
    styleType: EStyle.Primary
    iconSource: EIcons.MAGNIFIER
}
EButton {
    leftIcon: true
    text: qsTr("Upload")
    styleType: EStyle.Primary
    iconSource: EIcons.ARROW_UP_FROM_LINE
})qml"},
    {"RoundedClip",
     R"qml(EClip {
    radius: 32
    width: 440
    height: 180

    ERectangle {
        anchors.fill: parent
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)

        ERectangle {
            rotation: -20
            width: parent.width
            height: parent.height / 2
            color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
        }
        EButton {
            property int clickCount: 0

            anchors.centerIn: parent
            text: qsTr("点击次数： %1").arg(clickCount)

            onClicked: ++clickCount
        }
    }
})qml",
     R"qml(EClip {
    radius: 32
    width: 440
    height: 180

    ERectangle {
        anchors.fill: parent
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)

        ERectangle {
            rotation: -20
            width: parent.width
            height: parent.height / 2
            color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
        }
        EButton {
            property int clickCount: 0

            anchors.centerIn: parent
            text: qsTr("Clicks: %1").arg(clickCount)

            onClicked: ++clickCount
        }
    }
})qml"},
    {"BasicExamplesSample10",
     R"qml(ERotatingIcon {
    running: true
    iconSource: EIcons.ARROW_ROTATE_RIGHT
}
ERotatingIcon {
    running: false
    iconSource: EIcons.ARROW_ROTATE_RIGHT
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
}
ERotatingIcon {
    iconSize: 28
    duration: 900
    running: true
    originX: width / 2
    originY: height / 2
    iconSource: EIcons.ARROW_ROTATE_RIGHT
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml",
     R"qml(ERotatingIcon {
    running: true
    iconSource: EIcons.ARROW_ROTATE_RIGHT
}
ERotatingIcon {
    running: false
    iconSource: EIcons.ARROW_ROTATE_RIGHT
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
}
ERotatingIcon {
    iconSize: 28
    duration: 900
    running: true
    originX: width / 2
    originY: height / 2
    iconSource: EIcons.ARROW_ROTATE_RIGHT
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml"},
    {"FeedbackExamplesSample1",
     R"qml(Flow {
    spacing: 28
    width: parent.width

    ELoading {
        width: 46
        height: 46
    }
    ELoading {
        width: 52
        height: 52
        material: true
        showBackground: false
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
    }
    ELoading {
        width: 64
        height: 64
        duration: 800
        material: true
        strokeWidth: 7
        sweepAngle: 230
        clockwise: false
        showBackground: false
        styleType: EStyle.Warning
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    }
})qml",
     R"qml(Flow {
    spacing: 28
    width: parent.width

    ELoading {
        width: 46
        height: 46
    }
    ELoading {
        width: 52
        height: 52
        material: true
        showBackground: false
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
    }
    ELoading {
        width: 64
        height: 64
        duration: 800
        material: true
        strokeWidth: 7
        sweepAngle: 230
        clockwise: false
        showBackground: false
        styleType: EStyle.Warning
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    }
})qml"},
    {"LayoutExamplesSample7",
     R"qml(ESplitter {
    width: 600
    height: 160

    ERectangle {
        border.width: 1
        SplitView.minimumWidth: 100
        SplitView.preferredWidth: 200
        color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
        border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)
    }
    ERectangle {
        border.width: 1
        SplitView.fillWidth: true
        color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
        border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)
    }
})qml",
     R"qml(ESplitter {
    width: 600
    height: 160

    ERectangle {
        border.width: 1
        SplitView.minimumWidth: 100
        SplitView.preferredWidth: 200
        color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
        border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)
    }
    ERectangle {
        border.width: 1
        SplitView.fillWidth: true
        color: EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
        border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)
    }
})qml"},
    {"BasicExamplesSample6",
     R"qml(EButton {
    text: qsTr("Default")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EButton {
    plain: true
    text: qsTr("Plain")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EButton {
    enabled: false
    text: qsTr("Disabled")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EButton {
    plain: true
    enabled: false
    text: qsTr("Disabled Plain")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml",
     R"qml(EButton {
    text: qsTr("Default")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EButton {
    plain: true
    text: qsTr("Plain")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EButton {
    enabled: false
    text: qsTr("Disabled")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EButton {
    plain: true
    enabled: false
    text: qsTr("Disabled plain")
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml"},
    {"FeedbackExamplesSample3",
     R"qml(EMessage {
    id: messageCenter

    duration: 2600
    grouping: true
    showClose: true
    placement: EPlacement.Top
}
EButton {
    text: qsTr("保存")

    onClicked: messageCenter.open({
        "key": "save",
        "plain": true,
        "center": true,
        "duration": 3200,
        "showClose": true,
        "closeOnClick": true,
        "type": EStyle.Success,
        "message": qsTr("保存成功"),
        "textColor": EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success),
        "backgroundColor": EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
    })
})qml",
     R"qml(EMessage {
    id: messageCenter

    duration: 2600
    grouping: true
    showClose: true
    placement: EPlacement.Top
}
EButton {
    text: qsTr("Save")

    onClicked: messageCenter.open({
        "key": "save",
        "plain": true,
        "center": true,
        "duration": 3200,
        "showClose": true,
        "closeOnClick": true,
        "type": EStyle.Success,
        "message": qsTr("Saved successfully"),
        "textColor": EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success),
        "backgroundColor": EPaletteManager.ThemePalette.surfaceComponent(ETheme.dark)
    })
})qml"},
    {"FormExamplesSample5",
     R"qml(ESlider {
    value: 30
    width: Math.min(parent.width, 520)
}
ESlider {
    to: 10
    from: 0
    value: 6
    stepSize: 1
    styleType: EStyle.Success
    width: Math.min(parent.width, 520)
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
ESlider {
    from: 0
    to: 100
    value: 45
    stepSize: 5
    trackSize: 8
    handleSize: 24
    size: EStyle.Large
    styleType: EStyle.Primary
    orientation: Qt.Horizontal
    width: Math.min(parent.width, 520)
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml",
     R"qml(ESlider {
    value: 30
    width: Math.min(parent.width, 520)
}
ESlider {
    to: 10
    from: 0
    value: 6
    stepSize: 1
    styleType: EStyle.Success
    width: Math.min(parent.width, 520)
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
ESlider {
    from: 0
    to: 100
    value: 45
    stepSize: 5
    trackSize: 8
    handleSize: 24
    size: EStyle.Large
    styleType: EStyle.Primary
    orientation: Qt.Horizontal
    width: Math.min(parent.width, 520)
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
})qml"},
    {"BasicExamplesSample11",
     R"qml(ERectangle {
    radius: 8
    height: 64
    width: 120
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
ERectangle {
    height: 64
    radius: 20
    width: 120
    border.width: 2
    color: EPaletteManager.BasePalette.transparent
    border.color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
ERectangle {
    height: 64
    radius: 12
    width: 150
    opacity: 0.86
    border.width: 3
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    border.color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml",
     R"qml(ERectangle {
    radius: 8
    height: 64
    width: 120
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
ERectangle {
    height: 64
    radius: 20
    width: 120
    border.width: 2
    color: EPaletteManager.BasePalette.transparent
    border.color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
ERectangle {
    height: 64
    radius: 12
    width: 150
    opacity: 0.86
    border.width: 3
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    border.color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml"},
    {"FormExamplesSample10",
     R"qml(Item {
    id: page

    property var account: ({
        "name": ""
    })

    width: 620
    height: 180

    EForm {
        id: accountForm

        spacing: 16
        labelWidth: 90
        labelSuffix: "："
        size: EStyle.Large
        model: page.account
        width: parent.width
        focusFirstError: true
        validateOnChange: true
        rules: ({
            "name": [{
                "required": true,
                "trigger": "change",
                "message": qsTr("请输入名称")
            }]
        })

        EFormItem {
            prop: "name"
            required: true
            label: qsTr("名称")
            width: parent.width

            EInput {
                clearable: true
                width: parent.width
            }
        }
    }
    EButton {
        text: qsTr("验证")
        anchors.top: accountForm.bottom

        onClicked: accountForm.validate()
    }
})qml",
     R"qml(Item {
    id: page

    property var account: ({
        "name": ""
    })

    width: 620
    height: 180

    EForm {
        id: accountForm

        spacing: 16
        labelWidth: 90
        labelSuffix: ":"
        size: EStyle.Large
        model: page.account
        width: parent.width
        focusFirstError: true
        validateOnChange: true
        rules: ({
            "name": [{
                "required": true,
                "trigger": "change",
                "message": qsTr("Name is required")
            }]
        })

        EFormItem {
            prop: "name"
            required: true
            label: qsTr("Name")
            width: parent.width

            EInput {
                clearable: true
                width: parent.width
            }
        }
    }
    EButton {
        text: qsTr("Validate")
        anchors.top: accountForm.bottom

        onClicked: accountForm.validate()
    }
})qml"},
    {"FormExamplesSample8",
     R"qml(EThemeSwitch {
}
EThemeSwitch {
    duration: 300
    darkIconColor: EPaletteManager.BasePalette.white
    lightThumbColor: EPaletteManager.BasePalette.white
    darkBorderColor: EPaletteManager.ThemePalette.textRegular(false)
    darkTrackColor: EPaletteManager.ThemePalette.surfacePressed(true)
    lightTrackColor: EPaletteManager.ThemePalette.surfaceHover(false)
    darkThumbColor: EPaletteManager.ThemePalette.fetchThemeColor(true)
    lightBorderColor: EPaletteManager.ThemePalette.borderDefault(false)
    lightIconColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml",
     R"qml(EThemeSwitch {
}
EThemeSwitch {
    duration: 300
    darkIconColor: EPaletteManager.BasePalette.white
    lightThumbColor: EPaletteManager.BasePalette.white
    darkBorderColor: EPaletteManager.ThemePalette.textRegular(false)
    darkTrackColor: EPaletteManager.ThemePalette.surfacePressed(true)
    lightTrackColor: EPaletteManager.ThemePalette.surfaceHover(false)
    darkThumbColor: EPaletteManager.ThemePalette.fetchThemeColor(true)
    lightBorderColor: EPaletteManager.ThemePalette.borderDefault(false)
    lightIconColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml"},
    {"BasicExamplesSample12",
     R"qml(EAdvancedRectangle {
    height: 78
    width: 140
    radius: [28, 4, 28, 4]
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EAdvancedRectangle {
    height: 78
    width: 140
    borderWidth: 2
    radius: [6, 28, 6, 28]
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
    borderColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
EAdvancedRectangle {
    height: 78
    width: 170
    borderWidth: 3
    dashPattern: [4, 2]
    radius: [30, 8, 30, 8]
    borderStyle: Qt.DashLine
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    borderColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml",
     R"qml(EAdvancedRectangle {
    height: 78
    width: 140
    radius: [28, 4, 28, 4]
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
}
EAdvancedRectangle {
    height: 78
    width: 140
    borderWidth: 2
    radius: [6, 28, 6, 28]
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
    borderColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Success)
}
EAdvancedRectangle {
    height: 78
    width: 170
    borderWidth: 3
    dashPattern: [4, 2]
    radius: [30, 8, 30, 8]
    borderStyle: Qt.DashLine
    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    borderColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
})qml"},
    {"AcrylicEffect",
     R"qml(Column {
    width: 480
    spacing: 16

    Item {
        height: 240
        width: parent.width

        Image {
            id: backdrop

            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/example/images/gallery/lake.jpg"
        }
        EAcrylic {
            id: acrylic

            radius: 8
            width: 320
            height: 170
            blurRadius: 24
            tintOpacity: 0.65
            sourceItem: backdrop
            anchors.centerIn: parent
        }
    }
    EButton {
        text: qsTr("刷新背景")

        onClicked: acrylic.refresh()
    }
})qml",
     R"qml(Column {
    width: 480
    spacing: 16

    Item {
        height: 240
        width: parent.width

        Image {
            id: backdrop

            anchors.fill: parent
            fillMode: Image.PreserveAspectCrop
            source: "qrc:/example/images/gallery/lake.jpg"
        }
        EAcrylic {
            id: acrylic

            radius: 8
            width: 320
            height: 170
            blurRadius: 24
            tintOpacity: 0.65
            sourceItem: backdrop
            anchors.centerIn: parent
        }
    }
    EButton {
        text: qsTr("Refresh backdrop")

        onClicked: acrylic.refresh()
    }
})qml"},
    {"AppBar",
     R"qml(EApplicationWindow {
    width: 620
    height: 260
    visible: true
    flags: Qt.Window | Qt.FramelessWindowHint
    appBar: EAppBar {
        title: qsTr("应用")

        ETextButton {
            text: ELocale.language === "zh" ? ELocale.texts.English : ELocale.texts.SimplifiedChinese

            onClicked: ELocale.language = ELocale.language === "zh" ? "en" : "zh"
        }
        EThemeSwitch {
            anchors.verticalCenter: parent.verticalCenter
            accessibleName: ETheme.dark ? ELocale.texts.Light : ELocale.texts.Dark
        }
    }

    EText {
        font.pixelSize: 14
        wrapMode: Text.WordWrap
        anchors.centerIn: parent
        width: parent.width - 48
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("拖动标题栏、双击标题栏最大化，或从窗口边缘调整大小。")
        color: EPaletteManager.ThemePalette.textRegular(ETheme.dark)
    }
})qml",
     R"qml(EApplicationWindow {
    width: 620
    height: 260
    visible: true
    flags: Qt.Window | Qt.FramelessWindowHint
    appBar: EAppBar {
        title: qsTr("Application")

        ETextButton {
            text: ELocale.language === "zh" ? ELocale.texts.English : ELocale.texts.SimplifiedChinese

            onClicked: ELocale.language = ELocale.language === "zh" ? "en" : "zh"
        }
        EThemeSwitch {
            anchors.verticalCenter: parent.verticalCenter
            accessibleName: ETheme.dark ? ELocale.texts.Light : ELocale.texts.Dark
        }
    }

    EText {
        font.pixelSize: 14
        wrapMode: Text.WordWrap
        anchors.centerIn: parent
        width: parent.width - 48
        horizontalAlignment: Text.AlignHCenter
        text: qsTr("Drag the title bar, double-click to maximize, or resize from the window edges.")
        color: EPaletteManager.ThemePalette.textRegular(ETheme.dark)
    }
})qml"},
    {"LayoutExamplesSample3",
     R"qml(ERow {
    gutter: 16
    height: 64
    width: parent.width

    ECol {
        span: 8
        height: parent.height

        ERectangle {
            radius: 4
            border.width: 1
            anchors.fill: parent
            color: EPaletteManager.BasePalette.transparent
            border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)

            EText {
                font.pixelSize: 14
                anchors.centerIn: parent
                text: "8 / 24"
                color: EPaletteManager.ThemePalette.textRegular(ETheme.dark)
            }
        }
    }
    ECol {
        span: 16
        height: parent.height

        ERectangle {
            radius: 4
            border.width: 1
            anchors.fill: parent
            color: EPaletteManager.BasePalette.transparent
            border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)

            EText {
                font.pixelSize: 14
                anchors.centerIn: parent
                text: "16 / 24"
                color: EPaletteManager.ThemePalette.textRegular(ETheme.dark)
            }
        }
    }
})qml",
     R"qml(ERow {
    gutter: 16
    height: 64
    width: parent.width

    ECol {
        span: 8
        height: parent.height

        ERectangle {
            radius: 4
            border.width: 1
            anchors.fill: parent
            color: EPaletteManager.BasePalette.transparent
            border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)

            EText {
                font.pixelSize: 14
                anchors.centerIn: parent
                text: "8 / 24"
                color: EPaletteManager.ThemePalette.textRegular(ETheme.dark)
            }
        }
    }
    ECol {
        span: 16
        height: parent.height

        ERectangle {
            radius: 4
            border.width: 1
            anchors.fill: parent
            color: EPaletteManager.BasePalette.transparent
            border.color: EPaletteManager.ThemePalette.borderDefault(ETheme.dark)

            EText {
                font.pixelSize: 14
                anchors.centerIn: parent
                text: "16 / 24"
                color: EPaletteManager.ThemePalette.textRegular(ETheme.dark)
            }
        }
    }
})qml"},
    {"BasicExamplesSample7",
     R"qml(Column {
    spacing: 16
    width: Math.min(parent.width, 720)

    EText {
        font.bold: true
        font.pixelSize: 11
        font.letterSpacing: 1.2
        text: qsTr("阅读体验 / TYPOGRAPHY")
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
    }
    EText {
        font.bold: true
        lineHeight: 1.15
        font.pixelSize: 26
        width: parent.width
        text: qsTr("让信息更易读")
        wrapMode: Text.WordWrap
        color: EPaletteManager.ThemePalette.textPrimary(ETheme.dark)
    }
    EText {
        lineHeight: 1.6
        font.pixelSize: 14
        width: parent.width
        wrapMode: Text.WordWrap
        color: EPaletteManager.ThemePalette.textPrimary(ETheme.dark)
        text: qsTr("清晰的层级让内容更容易浏览。标题建立重点，正文保持舒适行距，辅助信息提供上下文。调整窗口宽度，观察文本自动换行。")
    }
    EText {
        lineHeight: 1.4
        font.pixelSize: 12
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("更新于今天 · 约 1 分钟阅读")
        color: EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
    }
})qml",
     R"qml(Column {
    spacing: 16
    width: Math.min(parent.width, 720)

    EText {
        font.bold: true
        font.pixelSize: 11
        font.letterSpacing: 1.2
        text: qsTr("READING / TYPOGRAPHY")
        color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
    }
    EText {
        font.bold: true
        lineHeight: 1.15
        font.pixelSize: 26
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("Make information easy to read")
        color: EPaletteManager.ThemePalette.textPrimary(ETheme.dark)
    }
    EText {
        lineHeight: 1.6
        font.pixelSize: 14
        width: parent.width
        wrapMode: Text.WordWrap
        color: EPaletteManager.ThemePalette.textPrimary(ETheme.dark)
        text: qsTr("Clear hierarchy makes content easier to scan. A heading sets the focus, comfortable line spacing supports reading, and secondary text adds context. Resize the window to see the text wrap.")
    }
    EText {
        lineHeight: 1.4
        font.pixelSize: 12
        width: parent.width
        wrapMode: Text.WordWrap
        text: qsTr("Updated today · 1 min read")
        color: EPaletteManager.ThemePalette.textSecondary(ETheme.dark)
    }
})qml"},
    {"BasicExamplesSample9",
     R"qml(Column {
    spacing: 16
    width: parent.width

    Row {
        spacing: 24

        Repeater {
            model: [16, 24, 32]

            Column {
                spacing: 10

                EIcon {
                    width: 36
                    height: 36
                    iconSize: modelData
                    iconSource: EIcons.FOLDER
                    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
                }
                EText {
                    font.pixelSize: 12
                    text: modelData + " px"
                }
            }
        }
    }
    Flow {
        spacing: 12
        width: parent.width

        Repeater {
            model: [
                {icon: EIcons.BELL, label: qsTr("主要"), tone: EStyle.Primary},
                {icon: EIcons.CIRCLE_CHECK, label: qsTr("成功"), tone: EStyle.Success},
                {icon: EIcons.TRIANGLE_EXCLAMATION, label: qsTr("警告"), tone: EStyle.Warning},
                {icon: EIcons.CIRCLE_XMARK, label: qsTr("危险"), tone: EStyle.Danger}
            ]

            Row {
                spacing: 12
                width: Math.max(0, (parent.width - 12) / 2)

                EIcon {
                    width: 28
                    height: 28
                    iconSize: 28
                    iconSource: modelData.icon
                    color: EPaletteManager.ThemePalette.fetchColorForStyle(modelData.tone)
                }
                EText {
                    font.pixelSize: 13
                    text: modelData.label
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
})qml",
     R"qml(Column {
    spacing: 16
    width: parent.width

    Row {
        spacing: 24

        Repeater {
            model: [16, 24, 32]

            Column {
                spacing: 10

                EIcon {
                    width: 36
                    height: 36
                    iconSize: modelData
                    iconSource: EIcons.FOLDER
                    color: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Primary)
                }
                EText {
                    font.pixelSize: 12
                    text: modelData + " px"
                }
            }
        }
    }
    Flow {
        spacing: 12
        width: parent.width

        Repeater {
            model: [
                {icon: EIcons.BELL, label: qsTr("Primary"), tone: EStyle.Primary},
                {icon: EIcons.CIRCLE_CHECK, label: qsTr("Success"), tone: EStyle.Success},
                {icon: EIcons.TRIANGLE_EXCLAMATION, label: qsTr("Warning"), tone: EStyle.Warning},
                {icon: EIcons.CIRCLE_XMARK, label: qsTr("Danger"), tone: EStyle.Danger}
            ]

            Row {
                spacing: 12
                width: Math.max(0, (parent.width - 12) / 2)

                EIcon {
                    width: 28
                    height: 28
                    iconSize: 28
                    iconSource: modelData.icon
                    color: EPaletteManager.ThemePalette.fetchColorForStyle(modelData.tone)
                }
                EText {
                    font.pixelSize: 13
                    text: modelData.label
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }
    }
})qml"},
    {"DataExamplesSample6",
     R"qml(Column {
    spacing: 12
    width: parent.width

    Flow {
        spacing: 10
        width: parent.width

        ETag {
            effect: "plain"
            size: EStyle.Small
            styleType: EStyle.Info
            text: qsTr("%1 条记录").arg(table.rowCount)
        }
        EText {
            height: 28
            font.pixelSize: 13
            verticalAlignment: Text.AlignVCenter
            text: table.currentRow ? qsTr("已选择：%1").arg(table.currentRow.name) : qsTr("尚未选择")
        }
        ETag {
            effect: "plain"
            size: EStyle.Small
            styleType: EStyle.Info
            text: table.sortOrder === "ascending" ? qsTr("升序")
                : table.sortOrder === "descending" ? qsTr("降序") : qsTr("原始顺序")
        }
        ETextButton {
            size: EStyle.Small
            text: qsTr("清除选择")
            enabled: table.currentRow !== null

            onClicked: table.clearSelection()
        }
        ETextButton {
            size: EStyle.Small
            text: qsTr("清除排序")
            enabled: table.sortOrder.length > 0

            onClicked: table.sort(table.sortProp, "")
        }
    }
    ETable {
        id: table

        stripe: true
        rowHeight: 46
        headerHeight: 44
        width: parent.width
        model: exampleData.models.TableRows
        columns: exampleData.models.TableColumns
    }
})qml",
     R"qml(Column {
    spacing: 12
    width: parent.width

    Flow {
        spacing: 10
        width: parent.width

        ETag {
            effect: "plain"
            size: EStyle.Small
            styleType: EStyle.Info
            text: qsTr("%1 records").arg(table.rowCount)
        }
        EText {
            height: 28
            font.pixelSize: 13
            verticalAlignment: Text.AlignVCenter
            text: table.currentRow ? qsTr("Selected: %1").arg(table.currentRow.name) : qsTr("No selection")
        }
        ETag {
            effect: "plain"
            size: EStyle.Small
            styleType: EStyle.Info
            text: table.sortOrder === "ascending" ? qsTr("Ascending")
                : table.sortOrder === "descending" ? qsTr("Descending") : qsTr("Original order")
        }
        ETextButton {
            size: EStyle.Small
            text: qsTr("Clear selection")
            enabled: table.currentRow !== null

            onClicked: table.clearSelection()
        }
        ETextButton {
            size: EStyle.Small
            text: qsTr("Clear sorting")
            enabled: table.sortOrder.length > 0

            onClicked: table.sort(table.sortProp, "")
        }
    }
    ETable {
        id: table

        stripe: true
        rowHeight: 46
        headerHeight: 44
        width: parent.width
        model: exampleData.models.TableRows
        columns: exampleData.models.TableColumns
    }
})qml"},
    {"FormExamplesSample6",
     R"qml(Flow {
    spacing: 24
    width: parent.width

    Column {
        spacing: 10
        width: Math.min(parent.width, 220)

        EText {
            font.bold: true
            text: qsTr("默认")
            font.pixelSize: 14
        }
        ESpinBox {
            to: 10
            from: 0
            value: 2
            width: parent.width
        }
        ETag {
            text: "0 – 10"
            effect: "plain"
            styleType: EStyle.Info
        }
    }
    Column {
        spacing: 10
        width: Math.min(parent.width, 220)

        EText {
            font.bold: true
            font.pixelSize: 14
            text: qsTr("输入") + " · ±2"
        }
        ESpinBox {
            to: 20
            value: 0
            from: -20
            stepSize: 2
            editable: true
            width: parent.width
        }
        ETag {
            effect: "plain"
            text: "−20 – 20"
            styleType: EStyle.Info
        }
    }
    Column {
        spacing: 10
        width: Math.min(parent.width, 220)

        EText {
            font.bold: true
            text: qsTr("校验")
            font.pixelSize: 14
        }
        ESpinBox {
            id: validationSpinBox

            from: 0
            to: 100
            value: 20
            stepSize: 5
            iconSize: 18
            editable: true
            indicatorWidth: 42
            size: EStyle.Large
            indicatorHeight: 38
            selectByMouse: true
            width: parent.width
            incIcon: EIcons.PLUS
            decIcon: EIcons.MINUS
            styleType: EStyle.Warning
            validationMessage: qsTr("请检查数值")
            validationState: EValidationState.Warning
            textColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
        }
        ETag {
            text: validationSpinBox.validationMessage
            styleType: validationSpinBox.hasValidationError ? EStyle.Danger : EStyle.Warning
        }
    }
})qml",
     R"qml(Flow {
    spacing: 24
    width: parent.width

    Column {
        spacing: 10
        width: Math.min(parent.width, 220)

        EText {
            font.bold: true
            font.pixelSize: 14
            text: qsTr("Default")
        }
        ESpinBox {
            to: 10
            from: 0
            value: 2
            width: parent.width
        }
        ETag {
            text: "0 – 10"
            effect: "plain"
            styleType: EStyle.Info
        }
    }
    Column {
        spacing: 10
        width: Math.min(parent.width, 220)

        EText {
            font.bold: true
            font.pixelSize: 14
            text: qsTr("Input") + " · ±2"
        }
        ESpinBox {
            to: 20
            value: 0
            from: -20
            stepSize: 2
            editable: true
            width: parent.width
        }
        ETag {
            effect: "plain"
            text: "−20 – 20"
            styleType: EStyle.Info
        }
    }
    Column {
        spacing: 10
        width: Math.min(parent.width, 220)

        EText {
            font.bold: true
            font.pixelSize: 14
            text: qsTr("Validate")
        }
        ESpinBox {
            id: validationSpinBox

            from: 0
            to: 100
            value: 20
            stepSize: 5
            iconSize: 18
            editable: true
            indicatorWidth: 42
            size: EStyle.Large
            indicatorHeight: 38
            selectByMouse: true
            width: parent.width
            incIcon: EIcons.PLUS
            decIcon: EIcons.MINUS
            styleType: EStyle.Warning
            validationMessage: qsTr("Enter a value")
            validationState: EValidationState.Warning
            textColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
        }
        ETag {
            text: validationSpinBox.validationMessage
            styleType: validationSpinBox.hasValidationError ? EStyle.Danger : EStyle.Warning
        }
    }
})qml"},
    {"OtherExamplesSample0",
     R"qml(EMenu {
    width: 260
    uniqueOpened: true
    currentIndex: "home"
    model: [{
        "index": "home",
        "text": "Home",
        "iconSource": EIcons.HOUSE
    }, {
        "index": "workspace",
        "text": "Workspace",
        "children": [{
            "index": "projects",
            "text": "Projects"
        }, {
            "index": "reports",
            "text": "Reports"
        }]
    }]

    onItemSelected: function(index, indexPath, item) {
        console.log(index, indexPath, item.text)
    }
}
EMenu {
    width: parent.width
    currentIndex: "products"
    styleType: EStyle.Success
    direction: EOrientation.Horizontal
    model: [{
        "index": "products",
        "text": qsTr("产品"),
        "iconSource": EIcons.STAR
    }, {
        "index": "solutions",
        "text": qsTr("解决方案"),
        "iconSource": EIcons.COMPASS
    }, {
        "index": "support",
        "text": qsTr("支持"),
        "iconSource": EIcons.BELL
    }]
}
EMenu {
    width: 64
    indent: 18
    iconSize: 20
    duration: 260
    collapse: true
    itemHeight: 48
    uniqueOpened: false
    currentIndex: "favorites"
    styleType: EStyle.Warning
    textColor: EPaletteManager.BasePalette.gainsboro
    backgroundColor: EPaletteManager.BasePalette.black
    hoverColor: EPaletteManager.BasePalette.charcoalGray
    activeTextColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    model: [{
        "index": "favorites",
        "text": qsTr("收藏"),
        "iconSource": EIcons.STAR
    }, {
        "index": "notifications",
        "text": qsTr("通知"),
        "iconSource": EIcons.BELL
    }]
})qml",
     R"qml(EMenu {
    width: 260
    uniqueOpened: true
    currentIndex: "home"
    model: [{
        "index": "home",
        "text": "Home",
        "iconSource": EIcons.HOUSE
    }, {
        "index": "workspace",
        "text": "Workspace",
        "children": [{
            "index": "projects",
            "text": "Projects"
        }, {
            "index": "reports",
            "text": "Reports"
        }]
    }]

    onItemSelected: function(index, indexPath, item) {
        console.log(index, indexPath, item.text)
    }
}
EMenu {
    width: parent.width
    currentIndex: "products"
    styleType: EStyle.Success
    direction: EOrientation.Horizontal
    model: [{
        "index": "products",
        "text": qsTr("Products"),
        "iconSource": EIcons.STAR
    }, {
        "index": "solutions",
        "text": qsTr("Solutions"),
        "iconSource": EIcons.COMPASS
    }, {
        "index": "support",
        "text": qsTr("Support"),
        "iconSource": EIcons.BELL
    }]
}
EMenu {
    width: 64
    indent: 18
    iconSize: 20
    duration: 260
    collapse: true
    itemHeight: 48
    uniqueOpened: false
    currentIndex: "favorites"
    styleType: EStyle.Warning
    textColor: EPaletteManager.BasePalette.gainsboro
    backgroundColor: EPaletteManager.BasePalette.black
    hoverColor: EPaletteManager.BasePalette.charcoalGray
    activeTextColor: EPaletteManager.ThemePalette.fetchColorForStyle(EStyle.Warning)
    model: [{
        "index": "favorites",
        "text": qsTr("Favorites"),
        "iconSource": EIcons.STAR
    }, {
        "index": "notifications",
        "text": qsTr("Notifications"),
        "iconSource": EIcons.BELL
    }]
})qml"},
    {"BasicExamplesSample2",
     R"qml(ETextButton {
    text: qsTr("Default")
}
ETextButton {
    text: qsTr("Primary")
    styleType: EStyle.Primary
}
ETextButton {
    text: qsTr("Success")
    styleType: EStyle.Success
}
ETextButton {
    text: qsTr("Info")
    styleType: EStyle.Info
}
ETextButton {
    text: qsTr("Warning")
    styleType: EStyle.Warning
}
ETextButton {
    text: qsTr("Danger")
    styleType: EStyle.Danger
}
ETextButton {
    text: qsTr("Default")
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Primary")
    styleType: EStyle.Primary
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Success")
    styleType: EStyle.Success
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Info")
    styleType: EStyle.Info
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Warning")
    styleType: EStyle.Warning
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Danger")
    styleType: EStyle.Danger
    isOpenBackgroundColor: true
}
ETextButton {
    enabled: false
    text: qsTr("Default")
}
ETextButton {
    enabled: false
    text: qsTr("Primary")
    styleType: EStyle.Primary
}
ETextButton {
    enabled: false
    text: qsTr("Success")
    styleType: EStyle.Success
}
ETextButton {
    enabled: false
    text: qsTr("Info")
    styleType: EStyle.Info
}
ETextButton {
    enabled: false
    text: qsTr("Warning")
    styleType: EStyle.Warning
}
ETextButton {
    enabled: false
    text: qsTr("Danger")
    styleType: EStyle.Danger
})qml",
     R"qml(ETextButton {
    text: qsTr("Default")
}
ETextButton {
    text: qsTr("Primary")
    styleType: EStyle.Primary
}
ETextButton {
    text: qsTr("Success")
    styleType: EStyle.Success
}
ETextButton {
    text: qsTr("Info")
    styleType: EStyle.Info
}
ETextButton {
    text: qsTr("Warning")
    styleType: EStyle.Warning
}
ETextButton {
    text: qsTr("Danger")
    styleType: EStyle.Danger
}
ETextButton {
    text: qsTr("Default")
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Primary")
    styleType: EStyle.Primary
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Success")
    styleType: EStyle.Success
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Info")
    styleType: EStyle.Info
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Warning")
    styleType: EStyle.Warning
    isOpenBackgroundColor: true
}
ETextButton {
    text: qsTr("Danger")
    styleType: EStyle.Danger
    isOpenBackgroundColor: true
}
ETextButton {
    enabled: false
    text: qsTr("Default")
}
ETextButton {
    enabled: false
    text: qsTr("Primary")
    styleType: EStyle.Primary
}
ETextButton {
    enabled: false
    text: qsTr("Success")
    styleType: EStyle.Success
}
ETextButton {
    enabled: false
    text: qsTr("Info")
    styleType: EStyle.Info
}
ETextButton {
    enabled: false
    text: qsTr("Warning")
    styleType: EStyle.Warning
}
ETextButton {
    enabled: false
    text: qsTr("Danger")
    styleType: EStyle.Danger
})qml"},
    {"FormExamplesSample9",
     R"qml(EFormItem {
    id: standaloneItem

    error: ""
    form: null
    prop: "email"
    rowSpacing: 8
    labelWidth: 90
    required: true
    labelSuffix: "："
    resettable: true
    label: qsTr("邮箱")
    showMessage: true
    size: EStyle.Large
    formDisabled: false
    validateEvent: true
    inlineMessage: false
    labelPosition: "left"
    propagateToField: true
    showSuccessState: true
    validateOnChange: true
    inheritFormConfig: false
    helpText: qsTr("用于接收账户通知")
    hideRequiredAsterisk: false
    objectName: "galleryFormItem"
    requiredMessage: qsTr("请输入邮箱")
    validationMessage: qsTr("格式正确")
    requiredAsteriskPosition: "left"
    width: Math.min(parent.width, 560)
    validationState: EValidationState.Success
    rules: [{ required: true, message: qsTr("邮箱不能为空") }]
    validator: function(value) {
        return String(value).indexOf("@") > 0 || qsTr("请输入有效邮箱")
    }

    EInput {
        clearable: true
        width: parent.width
        text: "demo@example.com"
    }
}
EFormItem {
    rowSpacing: 8
    labelWidth: 90
    labelSuffix: "："
    size: EStyle.Large
    label: qsTr("外部错误")
    labelPosition: "left"
    error: qsTr("服务端返回的错误信息")
    width: Math.min(parent.width, 560)

    EInput {
        text: qsTr("错误值")
        width: parent.width
    }
}
EDescriptions {
    columns: 1
    objectName: "galleryFormFeedback"
    width: Math.min(parent.width, 560)
    model: [{
        "label": qsTr("邮箱"),
        "value": standaloneItem.fieldValue
    }, {
        "label": qsTr("消息"),
        "value": standaloneItem.effectiveMessage
    }]
}
Flow {
    spacing: 10
    width: parent.width

    EButton {
        text: qsTr("校验")
        styleType: EStyle.Primary
        objectName: "galleryFormValidate"

        onClicked: standaloneItem.validate()
    }
    EButton {
        text: qsTr("重新开始")
        objectName: "galleryFormReset"

        onClicked: standaloneItem.resetField()
    }
    ETag {
        size: EStyle.Large
        styleType: standaloneItem.hasValidationError ? EStyle.Danger
            : standaloneItem.validationState === EValidationState.Success ? EStyle.Success : EStyle.Info
        text: standaloneItem.validating ? qsTr("加载中")
            : standaloneItem.hasValidationError ? qsTr("校验失败")
            : standaloneItem.validationState === EValidationState.Success ? qsTr("格式正确") : qsTr("输入")
    }
    ETag {
        text: qsTr("编辑")
        size: EStyle.Large
        visible: standaloneItem.fieldFocused
    }
})qml",
     R"qml(EFormItem {
    id: standaloneItem

    error: ""
    form: null
    prop: "email"
    rowSpacing: 8
    labelWidth: 90
    required: true
    labelSuffix: "："
    resettable: true
    showMessage: true
    size: EStyle.Large
    formDisabled: false
    validateEvent: true
    inlineMessage: false
    label: qsTr("Email")
    labelPosition: "left"
    propagateToField: true
    showSuccessState: true
    validateOnChange: true
    inheritFormConfig: false
    hideRequiredAsterisk: false
    objectName: "galleryFormItem"
    requiredAsteriskPosition: "left"
    width: Math.min(parent.width, 560)
    validationMessage: qsTr("Valid format")
    requiredMessage: qsTr("Enter your email")
    validationState: EValidationState.Success
    helpText: qsTr("Used for account notifications")
    rules: [{ required: true, message: qsTr("Email is required") }]
    validator: function(value) {
        return String(value).indexOf("@") > 0 || qsTr("Enter a valid email address")
    }

    EInput {
        clearable: true
        width: parent.width
        text: "demo@example.com"
    }
}
EFormItem {
    rowSpacing: 8
    labelWidth: 90
    labelSuffix: "："
    size: EStyle.Large
    labelPosition: "left"
    label: qsTr("External error")
    width: Math.min(parent.width, 560)
    error: qsTr("Error returned by the server")

    EInput {
        width: parent.width
        text: qsTr("Reset value")
    }
}
EDescriptions {
    columns: 1
    objectName: "galleryFormFeedback"
    width: Math.min(parent.width, 560)
    model: [{
        "label": qsTr("Email"),
        "value": standaloneItem.fieldValue
    }, {
        "label": qsTr("Messages"),
        "value": standaloneItem.effectiveMessage
    }]
}
Flow {
    spacing: 10
    width: parent.width

    EButton {
        text: qsTr("Validate")
        styleType: EStyle.Primary
        objectName: "galleryFormValidate"

        onClicked: standaloneItem.validate()
    }
    EButton {
        text: qsTr("Start over")
        objectName: "galleryFormReset"

        onClicked: standaloneItem.resetField()
    }
    ETag {
        size: EStyle.Large
        styleType: standaloneItem.hasValidationError ? EStyle.Danger
            : standaloneItem.validationState === EValidationState.Success ? EStyle.Success : EStyle.Info
        text: standaloneItem.validating ? qsTr("Loading")
            : standaloneItem.hasValidationError ? qsTr("Validation failed")
            : standaloneItem.validationState === EValidationState.Success ? qsTr("Valid format") : qsTr("Input")
    }
    ETag {
        size: EStyle.Large
        text: qsTr("Edit")
        visible: standaloneItem.fieldFocused
    }
})qml"},
};

QVariantMap buildEChartsOptions(bool chinese)
{
    QFile file(QStringLiteral(":/example/data/echarts-options.json"));
    if (!file.open(QIODevice::ReadOnly))
        return {};
    QString json = QString::fromUtf8(file.readAll());
    if (chinese)
    {
        const Sample labels[] = {
            {"Fri", "周五", "Fri"},
            {"Mon", "周一", "Mon"},
            {"Sat", "周六", "Sat"},
            {"Sun", "周日", "Sun"},
            {"Thu", "周四", "Thu"},
            {"Tue", "周二", "Tue"},
            {"Wed", "周三", "Wed"},
            {"Cost", "成本", "Cost"},
            {"Cache", "缓存", "Cache"},
            {"Speed", "速度", "Speed"},
            {"Engine", "引擎", "Engine"},
            {"Orders", "订单", "Orders"},
            {"Search", "搜索", "Search"},
            {"Social", "社交", "Social"},
            {"Trials", "试用", "Trials"},
            {"Client", "客户端", "Client"},
            {"Mobile", "移动端", "Mobile"},
            {"Direct", "直接访问", "Direct"},
            {"Gateway", "网关", "Gateway"},
            {"Product", "产品", "Product"},
            {"Quality", "质量", "Quality"},
            {"Service", "服务", "Service"},
            {"Signups", "注册", "Signups"},
            {"Storage", "存储", "Storage"},
            {"Desktop", "桌面端", "Desktop"},
            {"Controls", "控件", "Controls"},
            {"Referral", "外链", "Referral"},
            {"Renewals", "续费", "Renewals"},
            {"Visitors", "访问", "Visitors"},
            {"Coverage", "覆盖率", "Coverage"},
            {"Interface", "界面", "Interface"},
            {"Rendering", "渲染", "Rendering"},
            {"Completion", "完成率", "Completion"},
            {"Reliability", "可靠性", "Reliability"},
        };
        for (const auto &entry : labels)
            json.replace(QStringLiteral("\"%1\"").arg(QString::fromLatin1(entry.key)),
                         QStringLiteral("\"%1\"").arg(QString::fromUtf8(entry.chinese)));
    }
    return QJsonDocument::fromJson(json.toUtf8()).toVariant().toMap();
}

QVariantList buildTableAvatars()
{
    const QString root = QStringLiteral(":/example/images/48");
    QDirIterator iterator(root, QStringList{QStringLiteral("*.png")}, QDir::Files, QDirIterator::Subdirectories);
    QStringList paths;
    while (iterator.hasNext())
        paths.append(iterator.next());
    paths.sort();

    QVariantList rows;
    for (const auto &path : paths)
    {
        const QFileInfo info(path);
        const QSize size = QImageReader(path).size();
        rows.append(QVariantMap{{QStringLiteral("id"), rows.size() + 1},
                                {QStringLiteral("fileSize"), info.size()},
                                {QStringLiteral("name"), info.completeBaseName()},
                                {QStringLiteral("imageWidth"), size.width()},
                                {QStringLiteral("imageHeight"), size.height()},
                                {QStringLiteral("format"), info.suffix().toUpper()},
                                {QStringLiteral("category"), path.section('/', -2, -2)},
                                {QStringLiteral("avatar"), QStringLiteral("qrc") + path}});
    }
    return rows;
}

QVariantMap buildModels(bool chinese)
{
    QVariantMap texts;
    for (const auto &entry : ModelTexts)
        texts.insert(QString::fromLatin1(entry.key), QString::fromUtf8(chinese ? entry.chinese : entry.english));
    const auto text = [&texts](const char *key) { return texts.value(QString::fromLatin1(key)).toString(); };
    QVariantMap result;
    result.insert(QStringLiteral("TableAvatars"), buildTableAvatars());
    result.insert(QStringLiteral("Cities"), QVariantList{text("Beijing"), text("Shanghai"), text("Shenzhen")});
    result.insert(QStringLiteral("Technologies"),
                  QVariantList{text("Qml"), text("QtQuick"), text("QtWidgets"), text("QtCreator")});
    result.insert(
        QStringLiteral("Tabs"),
        QVariantList{QVariantMap{{QStringLiteral("text"), text("Overview")}},
                     QVariantMap{{QStringLiteral("text"), text("Activity")}},
                     QVariantMap{{QStringLiteral("text"), text("Settings")}},
                     QVariantMap{{QStringLiteral("disabled"), true}, {QStringLiteral("text"), text("Disabled4")}}});
    result.insert(QStringLiteral("Breadcrumbs"), QVariantList{text("Home"), text("Components"), text("Navigation")});
    result.insert(QStringLiteral("ProjectSteps"),
                  QVariantList{QVariantMap{{QStringLiteral("text"), text("ProjectCreated")},
                                           {QStringLiteral("description"), text("NameAndDescription")}},
                               QVariantMap{{QStringLiteral("text"), text("ConfigureResources")},
                                           {QStringLiteral("description"), text("AddComponents")}},
                               QVariantMap{{QStringLiteral("text"), text("Done")},
                                           {QStringLiteral("description"), text("ConfirmAndPublish")}}});
    result.insert(
        QStringLiteral("DropdownActions"),
        QVariantList{
            QVariantMap{{QStringLiteral("text"), text("Edit")}, {QStringLiteral("command"), QStringLiteral("edit")}},
            QVariantMap{{QStringLiteral("text"), text("Copy")}, {QStringLiteral("command"), QStringLiteral("copy")}},
            QVariantMap{{QStringLiteral("disabled"), true},
                        {QStringLiteral("text"), text("Delete")},
                        {QStringLiteral("command"), QStringLiteral("delete")}}});
    result.insert(
        QStringLiteral("VerticalMenu"),
        QVariantList{
            QVariantMap{{QStringLiteral("text"), text("Home")},
                        {QStringLiteral("index"), QStringLiteral("home")},
                        {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::HOUSE)}},
            QVariantMap{{QStringLiteral("text"), text("Workspace2")},
                        {QStringLiteral("index"), QStringLiteral("workspace")},
                        {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::COMPASS)},
                        {QStringLiteral("children"),
                         QVariantList{QVariantMap{{QStringLiteral("text"), text("Projects")},
                                                  {QStringLiteral("index"), QStringLiteral("projects")}},
                                      QVariantMap{{QStringLiteral("text"), text("Reports")},
                                                  {QStringLiteral("index"), QStringLiteral("reports")}}}}},
            QVariantMap{
                {QStringLiteral("text"), text("Files")},
                {QStringLiteral("index"), QStringLiteral("files")},
                {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::FOLDER)},
                {QStringLiteral("children"),
                 QVariantList{QVariantMap{{QStringLiteral("index"), text("Recent")},
                                          {QStringLiteral("text"), text("ArchivedFiles")}},
                              QVariantMap{{QStringLiteral("text"), text("Archive")},
                                          {QStringLiteral("index"), QStringLiteral("archive")},
                                          {QStringLiteral("children"),
                                           QVariantList{QVariantMap{{QStringLiteral("index"), text("Year2026")},
                                                                    {QStringLiteral("text"), text("Value2026")}}}}}}}},
            QVariantMap{{QStringLiteral("disabled"), true},
                        {QStringLiteral("index"), text("Disabled3")},
                        {QStringLiteral("text"), text("DisabledMenu")}}});
    result.insert(QStringLiteral("HorizontalMenu"),
                  QVariantList{QVariantMap{{QStringLiteral("text"), text("Products")},
                                           {QStringLiteral("index"), text("Products2")},
                                           {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::STAR)}},
                               QVariantMap{{QStringLiteral("text"), text("Solutions")},
                                           {QStringLiteral("index"), text("Solutions2")},
                                           {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::COMPASS)}},
                               QVariantMap{{QStringLiteral("text"), text("Support")},
                                           {QStringLiteral("index"), text("Support2")},
                                           {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::BELL)}}});
    result.insert(QStringLiteral("CollapsedMenu"),
                  QVariantList{QVariantMap{{QStringLiteral("text"), text("Favorites")},
                                           {QStringLiteral("index"), text("Favorites2")},
                                           {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::STAR)}},
                               QVariantMap{{QStringLiteral("text"), text("Notifications")},
                                           {QStringLiteral("index"), text("Notifications2")},
                                           {QStringLiteral("iconSource"), static_cast<int>(EIcons::IconType::BELL)}}});
    result.insert(QStringLiteral("Watermark"), QVariantList{QStringLiteral("EUI"), text("InternalUse")});
    result.insert(QStringLiteral("SemanticStyles"),
                  QVariantList{static_cast<int>(EStyle::Component::Primary),
                               static_cast<int>(EStyle::Component::Success),
                               static_cast<int>(EStyle::Component::Warning),
                               static_cast<int>(EStyle::Component::Danger),
                               static_cast<int>(EStyle::Component::Info)});
    result.insert(
        QStringLiteral("TableColumns"),
        QVariantList{QVariantMap{{QStringLiteral("sortable"), true},
                                 {QStringLiteral("label"), text("Name")},
                                 {QStringLiteral("prop"), QStringLiteral("name")}},
                     QVariantMap{{QStringLiteral("sortable"), true},
                                 {QStringLiteral("label"), text("Date")},
                                 {QStringLiteral("prop"), QStringLiteral("date")}},
                     QVariantMap{{QStringLiteral("label"), text("City")}, {QStringLiteral("prop"), text("City2")}},
                     QVariantMap{{QStringLiteral("width"), 90},
                                 {QStringLiteral("align"), QStringLiteral("right")},
                                 {QStringLiteral("sortable"), true},
                                 {QStringLiteral("label"), text("Age")},
                                 {QStringLiteral("prop"), QStringLiteral("age")}}});
    result.insert(QStringLiteral("TableRows"),
                  QVariantList{QVariantMap{{QStringLiteral("age"), 24},
                                           {QStringLiteral("name"), text("Alice")},
                                           {QStringLiteral("city"), text("Shanghai")},
                                           {QStringLiteral("date"), text("Value20261010")}},
                               QVariantMap{{QStringLiteral("age"), 31},
                                           {QStringLiteral("name"), text("Bob")},
                                           {QStringLiteral("city"), text("Beijing")},
                                           {QStringLiteral("date"), text("Value20261008")}},
                               QVariantMap{{QStringLiteral("age"), 28},
                                           {QStringLiteral("name"), text("Carol")},
                                           {QStringLiteral("city"), text("Hangzhou")},
                                           {QStringLiteral("date"), text("Value20261009")}},
                               QVariantMap{{QStringLiteral("age"), 36},
                                           {QStringLiteral("disabled"), true},
                                           {QStringLiteral("city"), text("Shenzhen")},
                                           {QStringLiteral("name"), text("DisabledRow")},
                                           {QStringLiteral("date"), text("Value20261007")}},
                               QVariantMap{{QStringLiteral("age"), 22},
                                           {QStringLiteral("name"), text("David")},
                                           {QStringLiteral("city"), text("Guangzhou")},
                                           {QStringLiteral("date"), text("Value20261008")}},
                               QVariantMap{{QStringLiteral("age"), 42},
                                           {QStringLiteral("name"), text("Charlie")},
                                           {QStringLiteral("city"), text("Shenzhen")},
                                           {QStringLiteral("date"), text("Value20261010")}}});
    result.insert(QStringLiteral("ExpandedTreeKeys"), QVariantList{QStringLiteral("workspace")});
    result.insert(
        QStringLiteral("WorkspaceTree"),
        QVariantList{
            QVariantMap{
                {QStringLiteral("label"), text("Workspace")},
                {QStringLiteral("id"), QStringLiteral("workspace")},
                {QStringLiteral("children"),
                 QVariantList{QVariantMap{{QStringLiteral("id"), text("Src")},
                                          {QStringLiteral("label"), text("SourceCode")},
                                          {QStringLiteral("children"),
                                           QVariantList{QVariantMap{{QStringLiteral("id"), text("Main")},
                                                                    {QStringLiteral("label"), text("MainQml")}},
                                                        QVariantMap{{QStringLiteral("id"), text("Style")},
                                                                    {QStringLiteral("label"), text("AgentsMd")}}}}},
                              QVariantMap{{QStringLiteral("id"), text("Docs")},
                                          {QStringLiteral("label"), text("Documentation")}}}}},
            QVariantMap{{QStringLiteral("disabled"), true},
                        {QStringLiteral("label"), text("Archive")},
                        {QStringLiteral("id"), QStringLiteral("archive")}}});
    result.insert(
        QStringLiteral("Regions"),
        QVariantList{QVariantMap{{QStringLiteral("label"), text("EastChina")},
                                 {QStringLiteral("value"), QStringLiteral("east")},
                                 {QStringLiteral("children"),
                                  QVariantList{QVariantMap{{QStringLiteral("label"), text("Hangzhou")},
                                                           {QStringLiteral("value"), QStringLiteral("hangzhou")}},
                                               QVariantMap{{QStringLiteral("label"), text("Shanghai")},
                                                           {QStringLiteral("value"), QStringLiteral("shanghai")}}}}},
                     QVariantMap{{QStringLiteral("label"), text("SouthChina")},
                                 {QStringLiteral("value"), QStringLiteral("south")},
                                 {QStringLiteral("children"),
                                  QVariantList{QVariantMap{{QStringLiteral("label"), text("Guangzhou")},
                                                           {QStringLiteral("value"), QStringLiteral("guangzhou")}},
                                               QVariantMap{{QStringLiteral("label"), text("Shenzhen")},
                                                           {QStringLiteral("value"), QStringLiteral("shenzhen")}}}}}});
    result.insert(QStringLiteral("InitialRegion"), QVariantList{QStringLiteral("east"), QStringLiteral("shanghai")});
    result.insert(QStringLiteral("InitialTags"), QVariantList{text("Qt"), text("Qml")});
    result.insert(QStringLiteral("Members"),
                  QVariantList{text("Alice2"), text("Bob2"), text("Charlie"), text("David")});
    result.insert(QStringLiteral("InitialCheckedTags"), QVariantList{text("Qml")});
    result.insert(QStringLiteral("Languages"), QVariantList{text("Qml"), text("Qt"), text("C")});
    result.insert(QStringLiteral("InitialTransferredKeys"), QVariantList{2});
    result.insert(QStringLiteral("TransferItems"),
                  QVariantList{QVariantMap{{QStringLiteral("key"), 1}, {QStringLiteral("label"), text("Qml")}},
                               QVariantMap{{QStringLiteral("key"), 2}, {QStringLiteral("label"), text("QtQuick")}},
                               QVariantMap{{QStringLiteral("key"), 3}, {QStringLiteral("label"), text("QtWidgets")}},
                               QVariantMap{{QStringLiteral("key"), 4},
                                           {QStringLiteral("disabled"), true},
                                           {QStringLiteral("label"), text("C")}}});
    result.insert(QStringLiteral("ScrollItems"), QVariantList{1, 2, 3, 4, 5, 6, 7, 8});
    result.insert(QStringLiteral("CarouselSlides"),
                  QVariantList{text("DiscoverComponents"), text("BuildInterfaces"), text("InteractWithEase")});
    result.insert(QStringLiteral("ActiveCollapseNames"), QVariantList{text("Consistency2")});
    result.insert(
        QStringLiteral("CollapsePanels"),
        QVariantList{QVariantMap{{QStringLiteral("name"), text("Consistency2")},
                                 {QStringLiteral("title"), text("Consistency")},
                                 {QStringLiteral("content"), text("KeepColorsSizesTypographyAndInteractionRules")}},
                     QVariantMap{{QStringLiteral("title"), text("Feedback")},
                                 {QStringLiteral("name"), QStringLiteral("feedback")},
                                 {QStringLiteral("content"), text("ProvideClearVisualFeedbackForEveryAction")}},
                     QVariantMap{{QStringLiteral("disabled"), true},
                                 {QStringLiteral("name"), text("Disabled3")},
                                 {QStringLiteral("title"), text("DisabledPanel")},
                                 {QStringLiteral("content"), text("ThisPanelCannotBeExpanded")}}});
    result.insert(
        QStringLiteral("UserDescriptions"),
        QVariantList{
            QVariantMap{{QStringLiteral("label"), text("Name")}, {QStringLiteral("value"), text("Alice")}},
            QVariantMap{{QStringLiteral("label"), text("City")}, {QStringLiteral("value"), text("Shanghai")}},
            QVariantMap{{QStringLiteral("label"), text("Role")}, {QStringLiteral("value"), text("Administrator")}},
            QVariantMap{{QStringLiteral("label"), text("Status")}, {QStringLiteral("value"), text("Enabled")}}});
    result.insert(QStringLiteral("VirtualTableColumns"),
                  QVariantList{QVariantMap{{QStringLiteral("sortable"), true},
                                           {QStringLiteral("label"), text("Name2")},
                                           {QStringLiteral("prop"), QStringLiteral("name")}},
                               QVariantMap{{QStringLiteral("sortable"), true},
                                           {QStringLiteral("label"), text("Value")},
                                           {QStringLiteral("prop"), text("Value2")}}});
    result.insert(QStringLiteral("TimelineEvents"),
                  QVariantList{QVariantMap{{QStringLiteral("timestamp"), text("Value202610101200")},
                                           {QStringLiteral("content"), text("PublishedSuccessfully")},
                                           {QStringLiteral("styleType"), static_cast<int>(EStyle::Component::Success)}},
                               QVariantMap{{QStringLiteral("content"), text("CodeReviewCompleted")},
                                           {QStringLiteral("timestamp"), text("Value202610101030")}},
                               QVariantMap{{QStringLiteral("hollow"), true},
                                           {QStringLiteral("content"), text("ProjectCreated")},
                                           {QStringLiteral("timestamp"), text("Value202610090900")}}});
    result.insert(QStringLiteral("Periods"), QVariantList{text("Day"), text("Week"), text("Month"), text("Year")});
    result.insert(
        QStringLiteral("ViewModes"),
        QVariantList{
            QVariantMap{{QStringLiteral("label"), text("List")}, {QStringLiteral("value"), QStringLiteral("list")}},
            QVariantMap{{QStringLiteral("label"), text("Grid")}, {QStringLiteral("value"), QStringLiteral("grid")}},
            QVariantMap{{QStringLiteral("disabled"), true},
                        {QStringLiteral("label"), text("Disabled2")},
                        {QStringLiteral("value"), text("Disabled3")}}});
    result.insert(QStringLiteral("EmailRules"),
                  QVariantList{QVariantMap{{QStringLiteral("required"), true},
                                           {QStringLiteral("message"), text("EmailIsRequired")}}});
    result.insert(
        QStringLiteral("AccountRules"),
        QVariantMap{{QStringLiteral("name"),
                     QVariantList{QVariantMap{{QStringLiteral("required"), true},
                                              {QStringLiteral("message"), text("NameIsRequired")},
                                              {QStringLiteral("trigger"), QStringLiteral("change")}}}},
                    {QStringLiteral("email"),
                     QVariantList{QVariantMap{{QStringLiteral("type"), QStringLiteral("email")},
                                              {QStringLiteral("trigger"), QStringLiteral("change")},
                                              {QStringLiteral("message"), text("EnterAValidEmailAddress")}}}}});
    result.insert(
        QStringLiteral("TourSteps"),
        QVariantList{QVariantMap{{QStringLiteral("title"), text("Welcome")},
                                 {QStringLiteral("description"), text("ThisButtonStartsTheGuidedTour")}},
                     QVariantMap{{QStringLiteral("title"), text("Continue")},
                                 {QStringLiteral("description"), text("YouHaveLearnedTheMainActionsClick")}}});
    result.insert(QStringLiteral("AnchorLabels"),
                  QVariantList{text("Overview"), text("Configuration"), text("Resources")});
    QVariantList options;
    QVariantList rows;
    QVariantList nodes;
    options.reserve(1000);
    rows.reserve(1000);
    nodes.reserve(1000);
    for (int index = 1; index <= 1000; ++index)
    {
        options.append(text("Option1").arg(index));
        rows.append(
            QVariantMap{{QStringLiteral("value"), index}, {QStringLiteral("name"), text("Record1").arg(index)}});
        nodes.append(QVariantMap{{QStringLiteral("id"), QString::number(index - 1)},
                                 {QStringLiteral("label"), text("Node1").arg(index)}});
    }
    result.insert(QStringLiteral("Password"), QStringLiteral("password"));
    result.insert(QStringLiteral("ManyRows"), rows);
    result.insert(QStringLiteral("ManyNodes"), nodes);
    result.insert(QStringLiteral("ManyOptions"), options);
    result.insert(QStringLiteral("TileHeights"), QVariantList{90, 150, 110, 180, 120, 80});
    result.insert(QStringLiteral("ChartValues"), QVariantList{12, 28, 18, 42, 32, 54, 38});
    result.insert(QStringLiteral("ChartTypes"),
                  QVariantList{QVariantMap{{QStringLiteral("value"), 0},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "折线" : "Line")}},
                               QVariantMap{{QStringLiteral("value"), 1},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "柱形" : "Bar")}},
                               QVariantMap{{QStringLiteral("value"), 2},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "面积" : "Area")}}});
    result.insert(QStringLiteral("StatusStates"),
                  QVariantList{QVariantMap{{QStringLiteral("value"), QStringLiteral("normal")},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "内容" : "Content")}},
                               QVariantMap{{QStringLiteral("value"), QStringLiteral("loading")},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "加载" : "Loading")}},
                               QVariantMap{{QStringLiteral("value"), QStringLiteral("empty")},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "空" : "Empty")}},
                               QVariantMap{{QStringLiteral("value"), QStringLiteral("error")},
                                           {QStringLiteral("label"), QString::fromUtf8(chinese ? "错误" : "Error")}}});
    result.insert(
        QStringLiteral("MainMenu"),
        QVariantList{
            QVariantMap{
                {QStringLiteral("text"), QString::fromUtf8(chinese ? "文件" : "File")},
                {QStringLiteral("children"),
                 QVariantList{QVariantMap{{QStringLiteral("index"), QStringLiteral("new")},
                                          {QStringLiteral("text"), QString::fromUtf8(chinese ? "新建" : "New")}},
                              QVariantMap{{QStringLiteral("index"), QStringLiteral("open")},
                                          {QStringLiteral("text"), QString::fromUtf8(chinese ? "打开" : "Open")}}}}},
            QVariantMap{
                {QStringLiteral("text"), QString::fromUtf8(chinese ? "编辑" : "Edit")},
                {QStringLiteral("children"),
                 QVariantList{QVariantMap{{QStringLiteral("index"), QStringLiteral("copy")},
                                          {QStringLiteral("text"), QString::fromUtf8(chinese ? "复制" : "Copy")}},
                              QVariantMap{{QStringLiteral("disabled"), true},
                                          {QStringLiteral("index"), QStringLiteral("paste")},
                                          {QStringLiteral("text"), QString::fromUtf8(chinese ? "粘贴" : "Paste")}}}}}});
    result.insert(QStringLiteral("EChartsOptions"), buildEChartsOptions(chinese));
    result.insert(
        QStringLiteral("EChartsTypes"),
        QVariantList{
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("line")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "折线图" : "Line")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "趋势与时间" : "Trends and time")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("bar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "分组柱状图" : "Grouped bar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("doughnut")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "圆环图" : "Doughnut")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "占比与转化" : "Share and conversion")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("scatter")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "散点图" : "Scatter")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "分布与密度" : "Distribution and density")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("area")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "面积图" : "Area")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "趋势与时间" : "Trends and time")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("stackedLine")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "堆叠面积图" : "Stacked area")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "趋势与时间" : "Trends and time")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("stepLine")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "阶梯折线图" : "Step line")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "趋势与时间" : "Trends and time")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("horizontalBar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "横向条形图" : "Horizontal bar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("stackedBar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "堆叠柱状图" : "Stacked bar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("negativeBar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "正负条形图" : "Positive and negative bar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("mixed")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "柱线双轴组合图" : "Bar and line with dual axes")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("pie")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "饼图" : "Pie")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "占比与转化" : "Share and conversion")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("rose")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "南丁格尔玫瑰图" : "Nightingale rose")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "占比与转化" : "Share and conversion")}},
            QVariantMap{{QStringLiteral("live"), true},
                        {QStringLiteral("value"), QStringLiteral("nestedPie")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "多层圆环图" : "Nested rings")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "占比与转化" : "Share and conversion")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("bubble")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "气泡图" : "Bubble")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "分布与密度" : "Distribution and density")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("effectScatter")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "涟漪散点图" : "Ripple scatter")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "分布与密度" : "Distribution and density")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("radar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "雷达图" : "Radar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "多维与数据映射" : "Dimensions and data mapping")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("gauge")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "仪表盘" : "Gauge")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "指标与进度" : "Indicators and progress")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("progressGauge")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "进度仪表盘" : "Progress gauge")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "指标与进度" : "Indicators and progress")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("funnel")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "漏斗图" : "Funnel")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "占比与转化" : "Share and conversion")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("heatmap")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "笛卡尔热力图" : "Cartesian heatmap")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "分布与密度" : "Distribution and density")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("calendar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "日历热力图" : "Calendar heatmap")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "分布与密度" : "Distribution and density")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("candlestick")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "K 线图" : "Candlestick")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "区间与统计" : "Ranges and statistics")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("boxplot")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "箱线图" : "Boxplot")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "区间与统计" : "Ranges and statistics")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("graph")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "关系图" : "Graph")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("tree")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "树形图" : "Tree")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("radialTree")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "径向树图" : "Radial tree")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("treemap")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "矩形树图" : "Treemap")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("sunburst")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "旭日图" : "Sunburst")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("sankey")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "桑基图" : "Sankey")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("parallel")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "平行坐标图" : "Parallel coordinates")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "多维与数据映射" : "Dimensions and data mapping")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("themeRiver")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "主题河流图" : "Theme river")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "趋势与时间" : "Trends and time")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("polarBar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "极坐标柱状图" : "Polar bar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("pictorialBar")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "象形柱图" : "Pictorial bar")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "对比与组合" : "Comparison and combinations")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("lines")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "路径流向图" : "Flow lines")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "层级与关系" : "Hierarchy and relationships")}},
            QVariantMap{{QStringLiteral("live"), false},
                        {QStringLiteral("value"), QStringLiteral("dataset")},
                        {QStringLiteral("label"), QString::fromUtf8(chinese ? "Dataset 与编码映射" : "Dataset and encode")},
                        {QStringLiteral("category"), QString::fromUtf8(chinese ? "多维与数据映射" : "Dimensions and data mapping")}}});
    return result;
}
}

ExampleData::ExampleData(QObject *parent) : QObject(parent)
{
    rebuild();
}

QVariantMap ExampleData::ui() const
{
    return ui_;
}

QVariantList ExampleData::icons() const
{
    const auto enumeration = QMetaEnum::fromType<EIcons::IconType>();
    QVariantList result;
    result.reserve(enumeration.keyCount());
    for (int index = 0; index < enumeration.keyCount(); ++index)
    {
        result.append(QVariantMap{{QStringLiteral("name"), QString::fromLatin1(enumeration.key(index))},
                                  {QStringLiteral("value"), enumeration.value(index)}});
    }
    return result;
}

QVariantMap ExampleData::models() const
{
    return models_;
}

QVariantMap ExampleData::samples() const
{
    return samples_;
}

void ExampleData::setLocale(QObject *locale)
{
    if (locale_ == locale)
        return;
    if (locale_)
        disconnect(locale_, nullptr, this, nullptr);
    locale_ = locale;
    if (locale_)
    {
        connect(locale_, SIGNAL(languageChanged()), this, SLOT(rebuild()));
        connect(locale_, &QObject::destroyed, this, &ExampleData::rebuild);
    }
    rebuild();
}

QString ExampleData::codeFor(const QString &componentName) const
{
    if (!componentName.startsWith(QLatin1Char('E')) || componentName.size() < 2)
        return {};
    for (const QChar character : componentName)
    {
        if (!character.isLetterOrNumber() && character != QLatin1Char('_'))
            return {};
    }
    return QStringLiteral("import EUI 1.0\nimport QtQuick 2.15\n\n%1 {\n}\n").arg(componentName);
}

QString ExampleData::highlightCode(const QString &code, const QVariantMap &colors) const
{
    return ECodeTools::highlightCode(code, colors);
}

void ExampleData::rebuild()
{
    const bool chinese = locale_ && locale_->property("language").toString() == QStringLiteral("zh");
    ui_                = QVariantMap{
                       {QStringLiteral("CodeFont"), QStringLiteral("Consolas")},
                       {QStringLiteral("NameSeparator"), QStringLiteral(" · ")},
                       {QStringLiteral("Stop"), QString::fromUtf8(chinese ? "停止" : "Stop")},
                       {QStringLiteral("Muted"), QString::fromUtf8(chinese ? "静音" : "Mute")},
                       {QStringLiteral("Verify"), QString::fromUtf8(chinese ? "验证" : "Verify")},
                       {QStringLiteral("MediaTitle"), QString::fromUtf8(chinese ? "媒体" : "Media")},
                       {QStringLiteral("LiveData"), QString::fromUtf8(chinese ? "实时数据" : "Live data")},
                       {QStringLiteral("NewTab"), QString::fromUtf8(chinese ? "新标签 %1" : "New tab %1")},
                       {QStringLiteral("ChartType"), QString::fromUtf8(chinese ? "图表类型" : "Chart type")},
                       {QStringLiteral("ShadowTitle"), QString::fromUtf8(chinese ? "控件层次" : "Elevation")},
                       {QStringLiteral("BlurRadius"), QString::fromUtf8(chinese ? "模糊半径" : "Blur radius")},
                       {QStringLiteral("ClipClicks"), QString::fromUtf8(chinese ? "点击次数：%1" : "Clicks: %1")},
                       {QStringLiteral("ClipRadius"), QString::fromUtf8(chinese ? "圆角半径" : "Corner radius")},
                       {QStringLiteral("ResetChart"), QString::fromUtf8(chinese ? "重置图表" : "Reset chart")},
                       {QStringLiteral("TableAscending"), QString::fromUtf8(chinese ? "升序" : "Ascending")},
                       {QStringLiteral("CaptchaInvalid"), QString::fromUtf8(chinese ? "请重试" : "Try again")},
                       {QStringLiteral("ChartLegend"), QString::fromUtf8(chinese ? "图例：%1" : "Legend: %1")},
                       {QStringLiteral("Refresh"), QString::fromUtf8(chinese ? "刷新背景" : "Refresh backdrop")},
                       {QStringLiteral("StartProgress"), QString::fromUtf8(chinese ? "开始任务" : "Start task")},
                       {QStringLiteral("TableDescending"), QString::fromUtf8(chinese ? "降序" : "Descending")},
                       {QStringLiteral("LargeData"), QString::fromUtf8(chinese ? "大数据集" : "Large data sets")},
                       {QStringLiteral("PlayPause"), QString::fromUtf8(chinese ? "播放 / 暂停" : "Play / pause")},
                       {QStringLiteral("TableRecords"), QString::fromUtf8(chinese ? "%1 条记录" : "%1 records")},
                       {QStringLiteral("TintOpacity"), QString::fromUtf8(chinese ? "色调透明度" : "Tint opacity")},
                       {QStringLiteral("CaptchaValid"), QString::fromUtf8(chinese ? "验证码正确" : "Correct code")},
                       {QStringLiteral("AcrylicTitle"), QString::fromUtf8(chinese ? "模糊与色调" : "Blur and tint")},
                       {QStringLiteral("ProgressButton"), QString::fromUtf8(chinese ? "任务进度" : "Task progress")},
                       {QStringLiteral("TableClearSort"), QString::fromUtf8(chinese ? "清除排序" : "Clear sorting")},
                       {QStringLiteral("TableSelected"), QString::fromUtf8(chinese ? "已选择：%1" : "Selected: %1")},
                       {QStringLiteral("VisualEffects"), QString::fromUtf8(chinese ? "视觉效果" : "Visual effects")},
                       {QStringLiteral("Waiting"), QString::fromUtf8(chinese ? "等待操作" : "Waiting for an action")},
                       {QStringLiteral("ContentReady"), QString::fromUtf8(chinese ? "内容已就绪" : "Content is ready")},
                       {QStringLiteral("ShadowElevation"), QString::fromUtf8(chinese ? "阴影高度" : "Shadow elevation")},
                       {QStringLiteral("ChartClicked"), QString::fromUtf8(chinese ? "已选择 %1：%2" : "Selected %1: %2")},
                       {QStringLiteral("ChartZoomed"), QString::fromUtf8(chinese ? "缩放范围已更新" : "Zoom range updated")},
                       {QStringLiteral("OpenPreview"), QString::fromUtf8(chinese ? "打开预览窗口" : "Open preview window")},
                       {QStringLiteral("TableOriginalOrder"), QString::fromUtf8(chinese ? "原始顺序" : "Original order")},
                       {QStringLiteral("CaptchaPlaceholder"), QString::fromUtf8(chinese ? "输入验证码" : "Enter the code")},
                       {QStringLiteral("TableClearSelection"), QString::fromUtf8(chinese ? "清除选择" : "Clear selection")},
                       {QStringLiteral("ShortcutDefault"), QString::fromUtf8(chinese ? "Ctrl+Shift+P" : "Ctrl+Shift+P")},
                       {QStringLiteral("EChartsTitle"), QString::fromUtf8(chinese ? "Apache ECharts" : "Apache ECharts")},
                       {QStringLiteral("RangeValue"), QString::fromUtf8(chinese ? "已选范围：%1–%2" : "Selected range: %1–%2")},
                       {QStringLiteral("ExampleUrl"), QString::fromUtf8(chinese ? "https://example.com" : "https://example.com")},
                       {QStringLiteral("QrIntroduction"),
                        QString::fromUtf8(chinese ? "输入文本或网址生成二维码。" : "Enter text or a URL to generate a QR code.")},
                       {QStringLiteral("AppBarPreview"),
                        QString::fromUtf8(chinese ? "拖动标题栏、双击标题栏最大化，或从窗口边缘调整大小。"
                                   : "Drag the title bar, double-click to maximize, or resize from the window edges.")},
                       {QStringLiteral("ChartIntroduction"),
                        QString::fromUtf8(chinese ? "用折线、柱形或面积图查看同一组数据。"
                                   : "View the same values as a line, bar or area chart.")},
                       {QStringLiteral("StatusError"),
                        QString::fromUtf8(chinese ? "请求未完成，请尝试重新加载内容。"
                                   : "The request could not be completed. Try loading the content again.")},
                       {QStringLiteral("MediaSourcePlaceholder"),
                        QString::fromUtf8(chinese ? "file:///C:/media/video.mp4 或媒体网址"
                                   : "file:///C:/media/video.mp4 or a media URL")},
                       {QStringLiteral("StaggeredIntroduction"),
                        QString::fromUtf8(chinese ? "不同高度的项目依次排入可用的列。"
                                   : "Items with different heights are placed in the next available column.")},
                       {QStringLiteral("ParallaxIntroduction"),
                        QString::fromUtf8(chinese ? "滚动时背景与内容以不同速度移动。"
                                   : "The background moves at a different speed from the content while you scroll.")},
                       {QStringLiteral("RangeIntroduction"),
                        QString::fromUtf8(chinese ? "拖动任一滑块选择范围。方向键可以调整当前滑块。"
                                   : "Drag either handle to select a range. Arrow keys adjust the focused handle.")},
                       {QStringLiteral("AppBarIntroduction"),
                        QString::fromUtf8(chinese
                               ? "在独立无边框窗口中体验语言与主题操作、标题栏拖动、双击最大化、窗口按钮和边缘缩放。"
                               : "Try language and theme actions, title-bar dragging, double-click maximize, window "
                                                "buttons and edge resizing in a separate frameless preview.")},
                       {QStringLiteral("EChartsDescription"),
                        QString::fromUtf8(chinese ? "离线 Apache ECharts，支持图例、提示、缩放及动态数据。"
                                   : "Offline Apache ECharts with legends, tooltips, zoom and live data.")},
                       {QStringLiteral("MenuBarIntroduction"),
                        QString::fromUtf8(chinese
                               ? "使用鼠标或方向键打开菜单，选择操作后查看结果。"
                               : "Open a menu with the mouse or arrow keys. Select an action to update the result.")},
                       {QStringLiteral("CaptchaIntroduction"),
                        QString::fromUtf8(chinese
                               ? "输入图片中的验证码并验证。点击图片生成新的验证码。"
                               : "Enter the displayed code and verify it. Click the image to generate another code.")},
                       {QStringLiteral("ShadowIntroduction"),
                        QString::fromUtf8(
             chinese ? "用阴影表现控件层次。调整高度查看阴影的范围和偏移。"
                     : "Use shadows to show elevation. Adjust the elevation to change the shadow spread and offset.")},
                       {QStringLiteral("StatusIntroduction"),
                        QString::fromUtf8(
             chinese
                 ? "切换内容、加载、空和错误状态。点击重试模拟重新加载成功。"
                 : "Switch between content, loading, empty and error states. Retry simulates a successful reload.")},
                       {QStringLiteral("ShortcutIntroduction"),
                        QString::fromUtf8(
             chinese ? "点击输入框录制快捷键。按 Escape 取消，也可以清除当前快捷键。"
                     : "Click the field to record a shortcut. Press Escape to cancel, or clear the current shortcut.")},
                       {QStringLiteral("MediaIntroduction"),
                        QString::fromUtf8(chinese ? "输入本地文件地址或媒体网址播放音频或视频，使用操作栏暂停、跳转和静音。"
                                   : "Enter a local file URL or media URL to play audio or video. Use the controls to "
                                                    "pause, seek and mute.")},
                       {QStringLiteral("CatalogIntroduction"),
                        QString::fromUtf8(chinese ? "选择组件查看交互示例和 QML 源码。也可以用左侧搜索快速查找。"
                                   : "Choose a component to try its behavior and view the QML source. Use the search "
                                                    "on the left to find a "
                                                    "control.")},
                       {QStringLiteral("ClipIntroduction"),
                        QString::fromUtf8(
             chinese ? "裁剪超出边界的子项并应用圆角，内部控件仍可交互。调整半径或点击按钮查看效果。"
                     : "Clip child content to rounded corners while keeping controls interactive. Adjust the radius "
                                      "or click the button.")},
                       {QStringLiteral("AcrylicIntroduction"),
                        QString::fromUtf8(chinese
                               ? "对背景内容做模糊处理，并叠加色调和轻微噪点。调整参数查看效果，刷新时重新采样背景。"
                               : "Blur background content, then add a tint and subtle noise. Adjust the controls or "
                                                "refresh the cached "
                                                "backdrop.")},
                       {QStringLiteral("EChartsIntroduction"),
                        QString::fromUtf8(
             chinese
                 ? "浏览 36 种图表示例并复制完整配置。支持的图表可开启实时数据，点击图例或使用缩放控件查看交互。"
                 : "Explore 36 chart examples and copy complete options. Enable live data for supported charts, "
                                  "toggle legends, or use zoom controls.")},
                       {QStringLiteral("NavigationGroups"),
                        QVariantList{QStringLiteral("basic"),
                      QStringLiteral("config"),
                      QStringLiteral("form"),
                      QStringLiteral("data"),
                      QStringLiteral("navigation"),
                      QStringLiteral("feedback"),
                      QStringLiteral("other"),
                      QStringLiteral("visual"),
                      QStringLiteral("extensions")}},
    };
    models_ = buildModels(chinese);
    samples_.clear();
    for (const auto &sample : Samples)
        samples_.insert(QString::fromLatin1(sample.key), QString::fromUtf8(chinese ? sample.chinese : sample.english));
    emit samplesChanged();
}
