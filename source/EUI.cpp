#include <QQmlContext>
#include <QFontDatabase>
#include <QGuiApplication>

#include "EUI.h"
#include "Enum/EIcons.h"
#include "Enum/EStyle.h"
#include "Config/ELocale.h"
#include "Layout/ELayout.h"
#include "Utils/ETableIO.h"
#include "Enum/EPlacement.h"
#include "Utils/EDataUtils.h"
#include "Enum/EOrientation.h"
#include "Utils/RandomUtils.h"
#include "Config/EValueUtils.h"
#include "Style/Theme/ETheme.h"
#include "Config/EDefaultConfig.h"
#include "Enum/EValidationState.h"
#include "Component/EFileUploader.h"
#include "Component/EChartRenderer.h"
#include "Component/EEChartsBridge.h"
#include "Component/EMasonryLayout.h"
#include "Component/EWindowActions.h"
#include "Component/EQrCodeRenderer.h"
#include "Component/EShadowRenderer.h"
#include "Component/ECaptchaRenderer.h"
#include "Component/EBackdropRenderer.h"
#include "State/EComponentStateObject.h"
#include "Component/EAdvancedRectangle.h"
#include "Component/EWatermarkRenderer.h"
#include "Style/Palettes/EPaletteManager.h"

namespace
{
struct QmlType
{
    const char *name;
    const char *path;
};

const QmlType PrivateTypes[] = {
    {"ColorSquare", "Controls/Private/ColorSquare.qml"},
    {"MessageCard", "Controls/Private/MessageCard.qml"},
    {"PickerPopup", "Controls/Private/PickerPopup.qml"},
    {"SliderTrack", "Controls/Private/SliderTrack.qml"},
    {"FieldSurface", "Controls/Private/FieldSurface.qml"},
    {"ScrollHandle", "Controls/Private/ScrollHandle.qml"},
    {"SliderHandle", "Controls/Private/SliderHandle.qml"},
    {"TransferPanel", "Controls/Private/TransferPanel.qml"},
    {"OptionDelegate", "Controls/Private/OptionDelegate.qml"},
    {"NotificationCard", "Controls/Private/NotificationCard.qml"},
};

const QmlType PublicTypes[] = {
    {"ETag", "Controls/Data/ETag.qml"},
    {"ECol", "Controls/Basic/ECol.qml"},
    {"ERow", "Controls/Basic/ERow.qml"},
    {"EApp", "Controls/Config/EApp.qml"},
    {"ECard", "Controls/Data/ECard.qml"},
    {"EForm", "Controls/Form/EForm.qml"},
    {"EList", "Controls/Data/EList.qml"},
    {"ERate", "Controls/Form/ERate.qml"},
    {"ETour", "Controls/Data/ETour.qml"},
    {"ETree", "Controls/Data/ETree.qml"},
    {"EFlex", "Controls/Basic/EFlex.qml"},
    {"EIcon", "Controls/Basic/EIcon.qml"},
    {"ELink", "Controls/Basic/ELink.qml"},
    {"EMain", "Controls/Basic/EMain.qml"},
    {"EText", "Controls/Basic/EText.qml"},
    {"EBadge", "Controls/Data/EBadge.qml"},
    {"EChart", "Controls/Data/EChart.qml"},
    {"EEmpty", "Controls/Data/EEmpty.qml"},
    {"EImage", "Controls/Data/EImage.qml"},
    {"EInput", "Controls/Form/EInput.qml"},
    {"EListy", "Controls/Data/EListy.qml"},
    {"ERadio", "Controls/Form/ERadio.qml"},
    {"ETable", "Controls/Data/ETable.qml"},
    {"EAside", "Controls/Basic/EAside.qml"},
    {"EClip", "Controls/Effects/EClip.qml"},
    {"ESpace", "Controls/Basic/ESpace.qml"},
    {"EAvatar", "Controls/Data/EAvatar.qml"},
    {"EQRCode", "Controls/Data/EQRCode.qml"},
    {"EResult", "Controls/Data/EResult.qml"},
    {"ESelect", "Controls/Form/ESelect.qml"},
    {"ESlider", "Controls/Form/ESlider.qml"},
    {"ESwitch", "Controls/Form/ESwitch.qml"},
    {"EUpload", "Controls/Form/EUpload.qml"},
    {"EAppBar", "Controls/Basic/EAppBar.qml"},
    {"EButton", "Controls/Basic/EButton.qml"},
    {"EFooter", "Controls/Basic/EFooter.qml"},
    {"EHeader", "Controls/Basic/EHeader.qml"},
    {"ECaptcha", "Controls/Form/ECaptcha.qml"},
    {"EECharts", "Controls/Data/EECharts.qml"},
    {"EMention", "Controls/Form/EMention.qml"},
    {"EMenu", "Controls/Navigation/EMenu.qml"},
    {"ESpinBox", "Controls/Form/ESpinBox.qml"},
    {"ETabs", "Controls/Navigation/ETabs.qml"},
    {"EAlert", "Controls/Indicator/EAlert.qml"},
    {"EShadow", "Controls/Effects/EShadow.qml"},
    {"EAffix", "Controls/Navigation/EAffix.qml"},
    {"ECalendar", "Controls/Data/ECalendar.qml"},
    {"ECarousel", "Controls/Data/ECarousel.qml"},
    {"ECascader", "Controls/Form/ECascader.qml"},
    {"ECheckBox", "Controls/Form/ECheckBox.qml"},
    {"ECollapse", "Controls/Data/ECollapse.qml"},
    {"EDivider", "Controls/Others/EDivider.qml"},
    {"EFormItem", "Controls/Form/EFormItem.qml"},
    {"EInputOtp", "Controls/Form/EInputOtp.qml"},
    {"EInputTag", "Controls/Form/EInputTag.qml"},
    {"EListItem", "Controls/Data/EListItem.qml"},
    {"ESkeleton", "Controls/Data/ESkeleton.qml"},
    {"ESteps", "Controls/Navigation/ESteps.qml"},
    {"ETimeline", "Controls/Data/ETimeline.qml"},
    {"ETransfer", "Controls/Form/ETransfer.qml"},
    {"EAcrylic", "Controls/Effects/EAcrylic.qml"},
    {"EDialog", "Controls/Indicator/EDialog.qml"},
    {"EDrawer", "Controls/Indicator/EDrawer.qml"},
    {"EFlexItem", "Controls/Basic/EFlexItem.qml"},
    {"ESplitter", "Controls/Basic/ESplitter.qml"},
    {"ETextArea", "Controls/Basic/ETextArea.qml"},
    {"EAnchor", "Controls/Navigation/EAnchor.qml"},
    {"ECountdown", "Controls/Data/ECountdown.qml"},
    {"ESegmented", "Controls/Data/ESegmented.qml"},
    {"EStatistic", "Controls/Data/EStatistic.qml"},
    {"EContainer", "Controls/Basic/EContainer.qml"},
    {"ELoading", "Controls/Indicator/ELoading.qml"},
    {"EMessage", "Controls/Indicator/EMessage.qml"},
    {"EPopover", "Controls/Indicator/EPopover.qml"},
    {"ERectangle", "Controls/Basic/ERectangle.qml"},
    {"EScrollbar", "Controls/Basic/EScrollbar.qml"},
    {"EToolTip", "Controls/Indicator/EToolTip.qml"},
    {"EBacktop", "Controls/Navigation/EBacktop.qml"},
    {"EDatePicker", "Controls/Form/EDatePicker.qml"},
    {"EMenuBar", "Controls/Navigation/EMenuBar.qml"},
    {"EPagination", "Controls/Data/EPagination.qml"},
    {"ERadioGroup", "Controls/Form/ERadioGroup.qml"},
    {"EStatusView", "Controls/Data/EStatusView.qml"},
    {"ETimePicker", "Controls/Form/ETimePicker.qml"},
    {"ETimeSelect", "Controls/Form/ETimeSelect.qml"},
    {"ETreeSelect", "Controls/Form/ETreeSelect.qml"},
    {"EWatermark", "Controls/Others/EWatermark.qml"},
    {"EProgress", "Controls/Indicator/EProgress.qml"},
    {"ETextButton", "Controls/Basic/ETextButton.qml"},
    {"ETypography", "Controls/Basic/ETypography.qml"},
    {"EAvatarGroup", "Controls/Data/EAvatarGroup.qml"},
    {"EColorPicker", "Controls/Form/EColorPicker.qml"},
    {"EDropdown", "Controls/Navigation/EDropdown.qml"},
    {"EImageViewer", "Controls/Data/EImageViewer.qml"},
    {"EMediaPlayer", "Controls/Data/EMediaPlayer.qml"},
    {"ERadioButton", "Controls/Form/ERadioButton.qml"},
    {"ERangeSlider", "Controls/Form/ERangeSlider.qml"},
    {"EThemeSwitch", "Controls/Form/EThemeSwitch.qml"},
    {"EBorderBeam", "Controls/Effects/EBorderBeam.qml"},
    {"EFloatButton", "Controls/Basic/EFloatButton.qml"},
    {"EAutocomplete", "Controls/Form/EAutocomplete.qml"},
    {"EDescriptions", "Controls/Data/EDescriptions.qml"},
    {"ESkeletonItem", "Controls/Data/ESkeletonItem.qml"},
    {"ECopyableText", "Controls/Basic/ECopyableText.qml"},
    {"EMessageBox", "Controls/Indicator/EMessageBox.qml"},
    {"EPopconfirm", "Controls/Indicator/EPopconfirm.qml"},
    {"ERotatingIcon", "Controls/Basic/ERotatingIcon.qml"},
    {"EBreadcrumb", "Controls/Navigation/EBreadcrumb.qml"},
    {"ECheckBoxGroup", "Controls/Form/ECheckBoxGroup.qml"},
    {"EPageHeader", "Controls/Navigation/EPageHeader.qml"},
    {"EParallaxView", "Controls/Effects/EParallaxView.qml"},
    {"ESplitterPanel", "Controls/Basic/ESplitterPanel.qml"},
    {"ECheckBoxButton", "Controls/Form/ECheckBoxButton.qml"},
    {"EDateTimePicker", "Controls/Form/EDateTimePicker.qml"},
    {"EInfiniteScroll", "Controls/Data/EInfiniteScroll.qml"},
    {"EShortcutPicker", "Controls/Form/EShortcutPicker.qml"},
    {"ENotification", "Controls/Indicator/ENotification.qml"},
    {"EConfigProvider", "Controls/Config/EConfigProvider.qml"},
    {"EDatePickerPanel", "Controls/Form/EDatePickerPanel.qml"},
    {"EStaggeredLayout", "Controls/Basic/EStaggeredLayout.qml"},
    {"EColorPickerPanel", "Controls/Form/EColorPickerPanel.qml"},
    {"ECircleProgress", "Controls/Indicator/ECircleProgress.qml"},
    {"EFloatButtonGroup", "Controls/Basic/EFloatButtonGroup.qml"},
    {"EApplicationWindow", "Controls/Basic/EApplicationWindow.qml"},
};
}

void EUI::registerTypes(QQmlEngine *engine)
{
    initializeEngine(engine, uri_);
    registerTypes(uri_);
}

void EUI::registerTypes(const char *uri) const
{
    int major = major_;
    int minor = minor_;
    qmlRegisterType<EComponentStateObject>(uri, major, minor, "EComponentStateObject");
    qmlRegisterType<EAdvancedRectangle>(uri, major, minor, "EAdvancedRectangle");
    qmlRegisterType<EFileUploader>("EUI.Private", major, minor, "FileUploader");
    qmlRegisterType<EShadowRenderer>("EUI.Private", major, minor, "ShadowRenderer");
    qmlRegisterType<EBackdropRenderer>("EUI.Private", major, minor, "BackdropRenderer");
    qmlRegisterType<EWatermarkRenderer>("EUI.Private", major, minor, "WatermarkRenderer");
    qmlRegisterType<EQrCodeRenderer>("EUI.Private", major, minor, "QrCodeRenderer");
    qmlRegisterType<EChartRenderer>("EUI.Private", major, minor, "ChartRenderer");
    qmlRegisterType<EMasonryLayout>("EUI.Private", major, minor, "MasonryLayout");
    qmlRegisterType<ECaptchaRenderer>("EUI.Private", major, minor, "CaptchaRenderer");
    qmlRegisterType<EWindowActions>("EUI.Private", major, minor, "WindowActions");
    qmlRegisterType<EEChartsBridge>("EUI.Private", major, minor, "EChartsBridge");
    qmlRegisterModule("EUI.Private", major, minor);

    for (const auto &type : PublicTypes)
        qmlRegisterType(
            QUrl(QStringLiteral("qrc:/qt/qml/EUI/") + QString::fromLatin1(type.path)), uri, major, minor, type.name);
    for (const auto &type : PrivateTypes)
        qmlRegisterType(QUrl(QStringLiteral("qrc:/qt/qml/EUI/") + QString::fromLatin1(type.path)),
                        "EUI.Private",
                        major,
                        minor,
                        type.name);

    qmlRegisterUncreatableMetaObject(
        EIcons::staticMetaObject, uri, major, minor, "EIcons", "Access to enums & flags only");
    qmlRegisterUncreatableMetaObject(
        EOrientation::staticMetaObject, uri, major, minor, "EOrientation", "Access to enums & flags only");
    qmlRegisterUncreatableMetaObject(
        EPlacement::staticMetaObject, uri, major, minor, "EPlacement", "Access to enums & flags only");
    qmlRegisterUncreatableMetaObject(
        EStyle::staticMetaObject, uri, major, minor, "EStyle", "Access to enums & flags only");
    qmlRegisterUncreatableMetaObject(
        EValidationState::staticMetaObject, uri, major, minor, "EValidationState", "Access to enums & flags only");
    qmlRegisterModule(uri, major, minor);
}

void EUI::initializeEngine(QQmlEngine *engine, [[maybe_unused]] const char *uri)
{
    EDefaultConfig *defaultConfig = EDefaultConfig::getInstance();
    QFont font                    = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    font.setFamily(defaultConfig->primaryFontFamily());
    font.setPixelSize(defaultConfig->fontPixelSize());
    QGuiApplication::setFont(font);

    ETheme *theme                   = ETheme::getInstance();
    ELayout *layout                 = ELayout::getInstance();
    RandomUtils *random             = RandomUtils::getInstance();
    EDefaultConfig *defs            = EDefaultConfig::getInstance();
    EPaletteManager *paletteManager = EPaletteManager::getInstance();
    EDataUtils *dataUtils           = new EDataUtils(engine);

    engine->rootContext()->setContextProperty("ELocale", ELocale::getInstance());
    engine->rootContext()->setContextProperty("EDataUtils", dataUtils);
    engine->rootContext()->setContextProperty("ETableIO", new ETableIO(engine));
    engine->rootContext()->setContextProperty("EValueUtils", new EValueUtils(engine));
    engine->rootContext()->setContextProperty("ETheme", theme);
    engine->rootContext()->setContextProperty("ELayout", layout);
    engine->rootContext()->setContextProperty("RandomUtils", random);
    engine->rootContext()->setContextProperty("EDefaultConfig", defs);
    engine->rootContext()->setContextProperty("EPaletteManager", paletteManager);

    // Shared JavaScript libraries do not inherit QML context properties.
    engine->globalObject().setProperty(QStringLiteral("EDataUtils"), engine->newQObject(dataUtils));
}
