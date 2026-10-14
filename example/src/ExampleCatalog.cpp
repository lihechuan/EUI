#include <QFile>
#include <QDebug>
#include <cstddef>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include "ExampleCatalog.h"

namespace
{
struct Entry
{
    const char *index;
    const char *source;
    const char *chinese;
    const char *english;
    bool demo = false;
    const char *introducedIn = "";
};

struct Group
{
    std::size_t count;
    const char *index;
    const Entry *items;
    const char *chinese;
    const char *english;
};

constexpr Entry OtherItems[] = {
    {"EDivider", "OtherExamples.qml", "分割线", "Divider"},
    {"EWatermark", "OtherExamples.qml", "水印", "Watermark"},
};

constexpr Entry ConfigItems[] = {
    {"EApp", "AntDesignExamples.qml", "应用上下文", "Application context"},
    {"EConfigProvider", "LayoutExamples.qml", "全局配置", "Config provider"},
};

constexpr Entry VisualItems[] = {
    {"EShadow", "VisualEffectsExamples.qml", "阴影", "Shadow"},
    {"EAcrylic", "VisualEffectsExamples.qml", "亚克力", "Acrylic"},
    {"EClip", "VisualEffectsExamples.qml", "圆角裁剪", "Rounded clip"},
    {"EAnimation", "AnimationExamples.qml", "动画", "Animation"},
    {"EBorderBeam", "AntDesignExamples.qml", "边框流光", "Border beam"},
    {"EParallaxView", "VisualEffectsExamples.qml", "视差滚动", "Parallax view"},
    {"TokyoScene", "Quick3DExamples.qml", "3D 动画", "3D animation", true},
};

constexpr Entry FeedbackItems[] = {
    {"EAlert", "OverlayExamples.qml", "提示", "Alert"},
    {"EDrawer", "OverlayExamples.qml", "抽屉", "Drawer"},
    {"EDialog", "OverlayExamples.qml", "对话框", "Dialog"},
    {"ELoading", "FeedbackExamples.qml", "加载", "Loading"},
    {"EPopover", "OverlayExamples.qml", "弹出框", "Popover"},
    {"EMessage", "FeedbackExamples.qml", "消息提示", "Message"},
    {"EToolTip", "FeedbackExamples.qml", "文字提示", "Tooltip"},
    {"EPopconfirm", "OverlayExamples.qml", "气泡确认", "Popconfirm"},
    {"ENotification", "OverlayExamples.qml", "通知", "Notification"},
    {"EMessageBox", "ExtendedFeedbackExamples.qml", "消息弹窗", "Message box"},
};

constexpr Entry NavigationItems[] = {
    {"EMenu", "OtherExamples.qml", "菜单", "Menu"},
    {"ETabs", "NavigationExamples.qml", "标签页", "Tabs"},
    {"ESteps", "NavigationExamples.qml", "步骤条", "Steps"},
    {"EMenuBar", "NavigationExamples.qml", "菜单栏", "Menu bar"},
    {"EAffix", "ExtendedNavigationExamples.qml", "固钉", "Affix"},
    {"EDropdown", "NavigationExamples.qml", "下拉菜单", "Dropdown"},
    {"EAnchor", "ExtendedNavigationExamples.qml", "锚点", "Anchor"},
    {"EBreadcrumb", "NavigationExamples.qml", "面包屑", "Breadcrumb"},
    {"EBacktop", "ExtendedNavigationExamples.qml", "回到顶部", "Backtop"},
    {"EWizard", "DesktopExamples.qml", "步骤向导", "Wizard", false, "1.2.0"},
    {"EPageHeader", "ExtendedNavigationExamples.qml", "页头", "Page header"},
    {"ECommandPalette", "DesktopExamples.qml", "命令面板", "Command palette", false, "1.2.0"},
};

constexpr Entry BasicItems[] = {
    {"EIcon", "BasicExamples.qml", "图标", "Icon"},
    {"EText", "BasicExamples.qml", "文本", "Text"},
    {"ELink", "LayoutExamples.qml", "链接", "Link"},
    {"ERow", "LayoutExamples.qml", "栅格布局", "Grid"},
    {"ESpace", "LayoutExamples.qml", "间距", "Space"},
    {"EButton", "BasicExamples.qml", "按钮", "Button"},
    {"EFlex", "AntDesignExamples.qml", "弹性布局", "Flex"},
    {"EAppBar", "LayoutExamples.qml", "窗口操作栏", "App bar"},
    {"ESplitter", "LayoutExamples.qml", "分隔面板", "Splitter"},
    {"EScrollbar", "LayoutExamples.qml", "滚动条", "Scrollbar"},
    {"EContainer", "LayoutExamples.qml", "布局容器", "Container"},
    {"ETypography", "LayoutExamples.qml", "排版", "Typography"},
    {"EFloatButton", "AntDesignExamples.qml", "悬浮按钮", "Float button"},
    {"ESplitterPanel", "LayoutExamples.qml", "分隔面板项", "Splitter panel"},
    {"EToolBar", "DesktopExamples.qml", "工具栏", "Toolbar", false, "1.2.0"},
    {"EStaggeredLayout", "LayoutExamples.qml", "瀑布布局", "Staggered layout"},
};

constexpr Entry ExtensionsItems[] = {
    {"ESpinBox", "FormExamples.qml", "数值调节", "Spinbox"},
    {"EFormItem", "FormExamples.qml", "表单项", "Form item"},
    {"ERectangle", "BasicExamples.qml", "矩形", "Rectangle"},
    {"ETextArea", "BasicExamples.qml", "多行输入", "Text area"},
    {"EListItem", "AntDesignExamples.qml", "列表项", "List item"},
    {"ETextButton", "BasicExamples.qml", "文字按钮", "Text button"},
    {"EFlexItem", "AntDesignExamples.qml", "弹性布局项", "Flex item"},
    {"ERadioButton", "FormExamples.qml", "单选按钮", "Radio button"},
    {"EThemeSwitch", "FormExamples.qml", "主题切换", "Theme switch"},
    {"ERotatingIcon", "BasicExamples.qml", "旋转图标", "Rotating icon"},
    {"ECopyableText", "BasicExamples.qml", "可复制文本", "Copyable text"},
    {"ECheckBoxButton", "FormExamples.qml", "多选按钮", "Checkbox button"},
    {"ERadioGroup", "ExtendedFormExamples.qml", "单选框组", "Radio group"},
    {"ECircleProgress", "FeedbackExamples.qml", "环形进度", "Circle progress"},
    {"ECheckBoxGroup", "ExtendedFormExamples.qml", "多选框组", "Checkbox group"},
    {"EAdvancedRectangle", "BasicExamples.qml", "高级矩形", "Advanced rectangle"},
    {"EFloatButtonGroup", "AntDesignExamples.qml", "悬浮按钮组", "Float button group"},
};

constexpr Entry FormItems[] = {
    {"EForm", "FormExamples.qml", "表单", "Form"},
    {"EInput", "FormExamples.qml", "输入框", "Input"},
    {"ERadio", "FormExamples.qml", "单选框", "Radio"},
    {"ESlider", "FormExamples.qml", "滑块", "Slider"},
    {"ESwitch", "FormExamples.qml", "开关", "Switch"},
    {"ERate", "SelectionExamples.qml", "评分", "Rate"},
    {"ECheckBox", "FormExamples.qml", "多选框", "Checkbox"},
    {"ESelect", "SelectionExamples.qml", "选择器", "Select"},
    {"EUpload", "ExtendedFormExamples.qml", "上传", "Upload"},
    {"EMention", "ExtendedFormExamples.qml", "提及", "Mention"},
    {"ECaptcha", "ExtendedFormExamples.qml", "图片验证码", "Captcha"},
    {"ETransfer", "ExtendedFormExamples.qml", "穿梭框", "Transfer"},
    {"ECascader", "ExtendedFormExamples.qml", "级联选择", "Cascader"},
    {"EInputTag", "ExtendedFormExamples.qml", "标签输入", "Input tag"},
    {"EDatePicker", "SelectionExamples.qml", "日期选择", "Date picker"},
    {"EInputOtp", "ExtendedFormExamples.qml", "验证码输入", "Input OTP"},
    {"ETimePicker", "SelectionExamples.qml", "时间选择", "Time picker"},
    {"ETimeSelect", "ExtendedFormExamples.qml", "时间下拉", "Time select"},
    {"ETreeSelect", "ExtendedFormExamples.qml", "树形选择", "Tree select"},
    {"EAutocomplete", "SelectionExamples.qml", "自动补全", "Autocomplete"},
    {"EColorPicker", "ExtendedFormExamples.qml", "颜色选择", "Color picker"},
    {"ERangeSlider", "ExtendedFormExamples.qml", "范围滑块", "Range slider"},
    {"EDatePickerPanel", "SelectionExamples.qml", "日期面板", "Date picker panel"},
    {"EShortcutPicker", "ExtendedFormExamples.qml", "快捷键", "Shortcut picker"},
    {"EDateTimePicker", "ExtendedFormExamples.qml", "日期时间", "Date time picker"},
    {"EFilePicker", "DesktopExamples.qml", "文件选择", "File picker", false, "1.2.0"},
    {"EColorPickerPanel", "ExtendedFormExamples.qml", "颜色面板", "Color picker panel"},
    {"EFolderPicker", "DesktopExamples.qml", "目录选择", "Folder picker", false, "1.2.0"},
};

constexpr Entry DataItems[] = {
    {"ETag", "DataExamples.qml", "标签", "Tag"},
    {"ECard", "DataExamples.qml", "卡片", "Card"},
    {"EBadge", "DataExamples.qml", "徽章", "Badge"},
    {"ETable", "DataExamples.qml", "表格", "Table"},
    {"ETree", "DataExamples.qml", "树形控件", "Tree"},
    {"EEmpty", "DataExamples.qml", "空状态", "Empty"},
    {"EAvatar", "DataExamples.qml", "头像", "Avatar"},
    {"EList", "AntDesignExamples.qml", "列表", "List"},
    {"ESkeleton", "DataExamples.qml", "骨架屏", "Skeleton"},
    {"EChart", "ExtendedDataExamples.qml", "图表", "Chart"},
    {"EImage", "ExtendedDataExamples.qml", "图片", "Image"},
    {"ETour", "ExtendedDataExamples.qml", "漫游引导", "Tour"},
    {"EPagination", "DataExamples.qml", "分页", "Pagination"},
    {"EResult", "ExtendedDataExamples.qml", "结果", "Result"},
    {"EProgress", "FeedbackExamples.qml", "进度条", "Progress"},
    {"EQRCode", "ExtendedDataExamples.qml", "二维码", "QR code"},
    {"EAvatarGroup", "DataExamples.qml", "头像组", "Avatar group"},
    {"EListy", "AntDesignExamples.qml", "虚拟列表", "Virtual list"},
    {"ECalendar", "ExtendedDataExamples.qml", "日历", "Calendar"},
    {"ECarousel", "ExtendedDataExamples.qml", "走马灯", "Carousel"},
    {"ESkeletonItem", "DataExamples.qml", "骨架项", "Skeleton item"},
    {"ETimeline", "ExtendedDataExamples.qml", "时间线", "Timeline"},
    {"ECollapse", "ExtendedDataExamples.qml", "折叠面板", "Collapse"},
    {"EMediaPlayer", "MediaExamples.qml", "媒体播放器", "Media player"},
    {"EStatistic", "ExtendedDataExamples.qml", "统计", "Statistic"},
    {"ECountdown", "ExtendedDataExamples.qml", "倒计时", "Countdown"},
    {"ESegmented", "ExtendedDataExamples.qml", "分段控制", "Segmented"},
    {"EStatusView", "ExtendedDataExamples.qml", "状态视图", "Status view"},
    {"EImageViewer", "ExtendedDataExamples.qml", "图片预览", "Image viewer"},
    {"EDescriptions", "ExtendedDataExamples.qml", "描述列表", "Descriptions"},
    {"ECodeView", "DesktopExamples.qml", "代码展示", "Code view", false, "1.2.0"},
    {"EECharts", "EEChartsExamples.qml", "Apache ECharts", "Apache ECharts"},
    {"EInfiniteScroll", "ExtendedDataExamples.qml", "无限滚动", "Infinite scroll"},
};

constexpr Group Groups[] = {
    {sizeof(BasicItems) / sizeof(Entry), "basic", BasicItems, "基础组件", "Basic components"},
    {sizeof(ConfigItems) / sizeof(Entry), "config", ConfigItems, "配置组件", "Configuration"},
    {sizeof(FormItems) / sizeof(Entry), "form", FormItems, "表单组件", "Form components"},
    {sizeof(DataItems) / sizeof(Entry), "data", DataItems, "数据展示", "Data display"},
    {sizeof(NavigationItems) / sizeof(Entry), "navigation", NavigationItems, "导航组件", "Navigation"},
    {sizeof(FeedbackItems) / sizeof(Entry), "feedback", FeedbackItems, "反馈组件", "Feedback"},
    {sizeof(OtherItems) / sizeof(Entry), "other", OtherItems, "其他组件", "Other components"},
    {sizeof(VisualItems) / sizeof(Entry), "visual", VisualItems, "视觉效果", "Visual effects"},
    {sizeof(ExtensionsItems) / sizeof(Entry), "extensions", ExtensionsItems, "EUI 扩展", "EUI extensions"},
};
}

ExampleCatalog::ExampleCatalog(QObject *parent) : QObject(parent)
{
    QFile file(QStringLiteral(":/example/data/component-api.json"));
    if (file.open(QIODevice::ReadOnly))
    {
        QJsonParseError error;
        const auto document = QJsonDocument::fromJson(file.readAll(), &error);
        if (error.error == QJsonParseError::NoError && document.isObject())
            m_documentation = document.object().toVariantMap();
        else
            qWarning() << "Invalid component documentation:" << error.errorString();
    }
    else
        qWarning() << "Cannot load component documentation:" << file.errorString();
    rebuild();
}

QVariantList ExampleCatalog::groups() const
{
    return m_groups;
}

bool ExampleCatalog::quick3DAvailable() const
{
#ifdef EUI_HAS_QUICK3D
    return true;
#else
    return false;
#endif
}

QString ExampleCatalog::quick3DSource(const QString &fileName) const
{
    if (fileName != QStringLiteral("TokyoView.qml") && fileName != QStringLiteral("TokyoScene.qml")
        && fileName != QStringLiteral("assets/LittlestTokyo.qml"))
        return QString();
    QFile file(QStringLiteral(":/example/quick3d-source/") + fileName + QStringLiteral(".txt"));
    return file.open(QIODevice::ReadOnly) ? QString::fromUtf8(file.readAll()) : QString();
}

void ExampleCatalog::setLocale(QObject *locale)
{
    if (m_locale == locale)
        return;
    if (m_locale)
        disconnect(m_locale, nullptr, this, nullptr);
    m_locale = locale;
    if (m_locale)
    {
        connect(m_locale, SIGNAL(languageChanged()), this, SLOT(rebuild()));
        connect(m_locale,
                &QObject::destroyed,
                this,
                [this]()
                {
                    m_locale = nullptr;
                    rebuild();
                });
    }
    rebuild();
}

QVariant ExampleCatalog::find(const QString &index) const
{
    for (const auto &group : m_groups)
    {
        const auto items = group.toMap().value(QStringLiteral("children")).toList();
        for (const auto &item : items)
        {
            if (item.toMap().value(QStringLiteral("index")).toString() == index)
                return item;
        }
    }
    return QVariant::fromValue(static_cast<QObject *>(nullptr));
}

QVariantList ExampleCatalog::filtered(const QString &query) const
{
    const QString term = query.trimmed();
    if (term.isEmpty())
        return m_groups;

    QVariantList results;
    for (std::size_t groupIndex = 0; groupIndex < sizeof(Groups) / sizeof(Group); ++groupIndex)
    {
        const Group &definition = Groups[groupIndex];
        QVariantMap group       = m_groups.at(static_cast<int>(groupIndex)).toMap();
        const bool groupMatches = QString::fromUtf8(definition.chinese).contains(term, Qt::CaseInsensitive) ||
                                  QString::fromUtf8(definition.english).contains(term, Qt::CaseInsensitive);
        const QVariantList items = group.value(QStringLiteral("children")).toList();
        QVariantList matches;
        for (std::size_t index = 0; index < definition.count; ++index)
        {
            const Entry &entry = definition.items[index];
            if (groupMatches || QString::fromUtf8(entry.index).contains(term, Qt::CaseInsensitive) ||
                QString::fromUtf8(entry.chinese).contains(term, Qt::CaseInsensitive) ||
                QString::fromUtf8(entry.english).contains(term, Qt::CaseInsensitive))
                matches.append(items.at(static_cast<int>(index)));
        }
        if (!matches.isEmpty())
        {
            group.insert(QStringLiteral("children"), matches);
            results.append(group);
        }
    }
    return results;
}

QVariantList ExampleCatalog::breadcrumb(const QString &index) const
{
    const bool chinese = m_locale && m_locale->property("language").toString() == QStringLiteral("zh");
    QVariantList path{QString::fromUtf8(chinese ? "组件目录" : "Components")};
    for (const auto &group : m_groups)
    {
        const QVariantMap category = group.toMap();
        for (const auto &item : category.value(QStringLiteral("children")).toList())
        {
            const QVariantMap entry = item.toMap();
            if (entry.value(QStringLiteral("index")).toString() == index)
            {
                path.append(category.value(QStringLiteral("text")));
                path.append(entry.value(QStringLiteral("label")));
                return path;
            }
        }
    }
    return path;
}

QVariantMap ExampleCatalog::documentation(const QString &index) const
{
    QVariantMap result     = m_documentation.value(index).toMap();
    const QString language = m_locale && m_locale->property("language").toString() == QStringLiteral("zh")
                                 ? QStringLiteral("zh")
                                 : QStringLiteral("en");
    QVariantList properties;
    for (const auto &value : result.value(QStringLiteral("properties")).toList())
    {
        QVariantMap row = value.toMap();
        row.insert(QStringLiteral("defaultValue"), row.take(QStringLiteral("default")).toMap().value(language));
        row.insert(QStringLiteral("description"), row.value(QStringLiteral("description")).toMap().value(language));
        row.insert(QStringLiteral("details"), row.value(QStringLiteral("details")).toMap().value(language).toString());
        properties.append(row);
    }
    result.insert(QStringLiteral("properties"), properties);
    return result;
}

void ExampleCatalog::rebuild()
{
    const bool chinese = m_locale && m_locale->property("language").toString() == QStringLiteral("zh");
    QVariantList groups;
    for (const auto &definition : Groups)
    {
        QVariantList items;
        for (std::size_t index = 0; index < definition.count; ++index)
        {
            const Entry &entry  = definition.items[index];
            const QString name  = QString::fromUtf8(entry.index);
            const QString label = QString::fromUtf8(chinese ? entry.chinese : entry.english);
            const QString introducedIn = QString::fromLatin1(entry.introducedIn);
            items.append(QVariantMap{
                {QStringLiteral("isNew"), introducedIn == QStringLiteral(EUI_GALLERY_VERSION)},
                {QStringLiteral("introducedIn"), introducedIn},
                {QStringLiteral("demo"), entry.demo},
                {QStringLiteral("index"), name},
                {QStringLiteral("label"), label},
                {QStringLiteral("text"), label},
                {QStringLiteral("source"), QString::fromUtf8(entry.source)},
            });
        }
        groups.append(QVariantMap{
            {QStringLiteral("index"), QString::fromUtf8(definition.index)},
            {QStringLiteral("text"), QString::fromUtf8(chinese ? definition.chinese : definition.english)},
            {QStringLiteral("children"), items},
        });
    }
    if (m_groups == groups)
        return;
    m_groups = groups;
    emit groupsChanged();
}
