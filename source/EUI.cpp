#include <QQmlContext>
#include <QFontDatabase>
#include <QGuiApplication>

#include "EUI.h"
#include "enum/EIcons.h"
#include "enum/EStyle.h"
#include "config/ELocale.h"
#include "layout/ELayout.h"
#include "utils/ETableIO.h"
#include "enum/EPlacement.h"
#include "utils/EAnimation.h"
#include "utils/ECodeTools.h"
#include "utils/EDataUtils.h"
#include "enum/EOrientation.h"
#include "utils/RandomUtils.h"
#include "config/EValueUtils.h"
#include "style/theme/ETheme.h"
#include "config/EDefaultConfig.h"
#include "enum/EValidationState.h"
#include "utils/EFilePickerUtils.h"
#include "component/EFileUploader.h"
#include "component/EChartRenderer.h"
#include "component/EEChartsBridge.h"
#include "component/EMasonryLayout.h"
#include "component/EWindowActions.h"
#include "component/EQrCodeRenderer.h"
#include "component/EShadowRenderer.h"
#include "component/ECaptchaRenderer.h"
#include "component/EBackdropRenderer.h"
#include "state/EComponentStateObject.h"
#include "component/EAdvancedRectangle.h"
#include "component/EWatermarkRenderer.h"
#include "style/palettes/EPaletteManager.h"

namespace
{
struct QmlType
{
    const char *name;
    const char *path;
};

const QmlType PrivateTypes[] = {
    {"ColorSquare", "controls/private/ColorSquare.qml"},
    {"MessageCard", "controls/private/MessageCard.qml"},
    {"PickerPopup", "controls/private/PickerPopup.qml"},
    {"SliderTrack", "controls/private/SliderTrack.qml"},
    {"FieldSurface", "controls/private/FieldSurface.qml"},
    {"ScrollHandle", "controls/private/ScrollHandle.qml"},
    {"SliderHandle", "controls/private/SliderHandle.qml"},
    {"TransferPanel", "controls/private/TransferPanel.qml"},
    {"OptionDelegate", "controls/private/OptionDelegate.qml"},
    {"NotificationCard", "controls/private/NotificationCard.qml"},
};

const QmlType PublicTypes[] = {
    {"ETag", "controls/data/ETag.qml"},
    {"ECol", "controls/basic/ECol.qml"},
    {"ERow", "controls/basic/ERow.qml"},
    {"EApp", "controls/config/EApp.qml"},
    {"ECard", "controls/data/ECard.qml"},
    {"EForm", "controls/form/EForm.qml"},
    {"EList", "controls/data/EList.qml"},
    {"ERate", "controls/form/ERate.qml"},
    {"ETour", "controls/data/ETour.qml"},
    {"ETree", "controls/data/ETree.qml"},
    {"EFlex", "controls/basic/EFlex.qml"},
    {"EIcon", "controls/basic/EIcon.qml"},
    {"ELink", "controls/basic/ELink.qml"},
    {"EMain", "controls/basic/EMain.qml"},
    {"EText", "controls/basic/EText.qml"},
    {"EBadge", "controls/data/EBadge.qml"},
    {"EChart", "controls/data/EChart.qml"},
    {"EEmpty", "controls/data/EEmpty.qml"},
    {"EImage", "controls/data/EImage.qml"},
    {"EInput", "controls/form/EInput.qml"},
    {"EListy", "controls/data/EListy.qml"},
    {"ERadio", "controls/form/ERadio.qml"},
    {"ETable", "controls/data/ETable.qml"},
    {"EAside", "controls/basic/EAside.qml"},
    {"EClip", "controls/effects/EClip.qml"},
    {"ESpace", "controls/basic/ESpace.qml"},
    {"EAvatar", "controls/data/EAvatar.qml"},
    {"EQRCode", "controls/data/EQRCode.qml"},
    {"EResult", "controls/data/EResult.qml"},
    {"ESelect", "controls/form/ESelect.qml"},
    {"ESlider", "controls/form/ESlider.qml"},
    {"ESwitch", "controls/form/ESwitch.qml"},
    {"EUpload", "controls/form/EUpload.qml"},
    {"EAppBar", "controls/basic/EAppBar.qml"},
    {"EButton", "controls/basic/EButton.qml"},
    {"EFooter", "controls/basic/EFooter.qml"},
    {"EHeader", "controls/basic/EHeader.qml"},
    {"ECaptcha", "controls/form/ECaptcha.qml"},
    {"EECharts", "controls/data/EECharts.qml"},
    {"EMention", "controls/form/EMention.qml"},
    {"EMenu", "controls/navigation/EMenu.qml"},
    {"ESpinBox", "controls/form/ESpinBox.qml"},
    {"ETabs", "controls/navigation/ETabs.qml"},
    {"EAlert", "controls/indicator/EAlert.qml"},
    {"EShadow", "controls/effects/EShadow.qml"},
    {"EToolBar", "controls/basic/EToolBar.qml"},
    {"EAffix", "controls/navigation/EAffix.qml"},
    {"ECalendar", "controls/data/ECalendar.qml"},
    {"ECarousel", "controls/data/ECarousel.qml"},
    {"ECascader", "controls/form/ECascader.qml"},
    {"ECheckBox", "controls/form/ECheckBox.qml"},
    {"ECodeView", "controls/data/ECodeView.qml"},
    {"ECollapse", "controls/data/ECollapse.qml"},
    {"EDivider", "controls/others/EDivider.qml"},
    {"EFormItem", "controls/form/EFormItem.qml"},
    {"EInputOtp", "controls/form/EInputOtp.qml"},
    {"EInputTag", "controls/form/EInputTag.qml"},
    {"EListItem", "controls/data/EListItem.qml"},
    {"ESkeleton", "controls/data/ESkeleton.qml"},
    {"ESteps", "controls/navigation/ESteps.qml"},
    {"ETimeline", "controls/data/ETimeline.qml"},
    {"ETransfer", "controls/form/ETransfer.qml"},
    {"EAcrylic", "controls/effects/EAcrylic.qml"},
    {"EDialog", "controls/indicator/EDialog.qml"},
    {"EDrawer", "controls/indicator/EDrawer.qml"},
    {"EFlexItem", "controls/basic/EFlexItem.qml"},
    {"ESplitter", "controls/basic/ESplitter.qml"},
    {"ETextArea", "controls/basic/ETextArea.qml"},
    {"EAnchor", "controls/navigation/EAnchor.qml"},
    {"ECountdown", "controls/data/ECountdown.qml"},
    {"ESegmented", "controls/data/ESegmented.qml"},
    {"EStatistic", "controls/data/EStatistic.qml"},
    {"EWizard", "controls/navigation/EWizard.qml"},
    {"EContainer", "controls/basic/EContainer.qml"},
    {"ELoading", "controls/indicator/ELoading.qml"},
    {"EMessage", "controls/indicator/EMessage.qml"},
    {"EPopover", "controls/indicator/EPopover.qml"},
    {"ERectangle", "controls/basic/ERectangle.qml"},
    {"EScrollbar", "controls/basic/EScrollbar.qml"},
    {"EToolTip", "controls/indicator/EToolTip.qml"},
    {"EBacktop", "controls/navigation/EBacktop.qml"},
    {"EDatePicker", "controls/form/EDatePicker.qml"},
    {"EFilePicker", "controls/form/EFilePicker.qml"},
    {"EMenuBar", "controls/navigation/EMenuBar.qml"},
    {"EPagination", "controls/data/EPagination.qml"},
    {"ERadioGroup", "controls/form/ERadioGroup.qml"},
    {"EStatusView", "controls/data/EStatusView.qml"},
    {"ETimePicker", "controls/form/ETimePicker.qml"},
    {"ETimeSelect", "controls/form/ETimeSelect.qml"},
    {"ETreeSelect", "controls/form/ETreeSelect.qml"},
    {"EWatermark", "controls/others/EWatermark.qml"},
    {"EProgress", "controls/indicator/EProgress.qml"},
    {"ETextButton", "controls/basic/ETextButton.qml"},
    {"ETypography", "controls/basic/ETypography.qml"},
    {"EAvatarGroup", "controls/data/EAvatarGroup.qml"},
    {"EColorPicker", "controls/form/EColorPicker.qml"},
    {"EDropdown", "controls/navigation/EDropdown.qml"},
    {"EImageViewer", "controls/data/EImageViewer.qml"},
    {"EMediaPlayer", "controls/data/EMediaPlayer.qml"},
    {"ERadioButton", "controls/form/ERadioButton.qml"},
    {"ERangeSlider", "controls/form/ERangeSlider.qml"},
    {"EThemeSwitch", "controls/form/EThemeSwitch.qml"},
    {"EBorderBeam", "controls/effects/EBorderBeam.qml"},
    {"EFloatButton", "controls/basic/EFloatButton.qml"},
    {"EAutocomplete", "controls/form/EAutocomplete.qml"},
    {"EDescriptions", "controls/data/EDescriptions.qml"},
    {"EFolderPicker", "controls/form/EFolderPicker.qml"},
    {"ESkeletonItem", "controls/data/ESkeletonItem.qml"},
    {"ECopyableText", "controls/basic/ECopyableText.qml"},
    {"EMessageBox", "controls/indicator/EMessageBox.qml"},
    {"EPopconfirm", "controls/indicator/EPopconfirm.qml"},
    {"ERotatingIcon", "controls/basic/ERotatingIcon.qml"},
    {"EBreadcrumb", "controls/navigation/EBreadcrumb.qml"},
    {"ECheckBoxGroup", "controls/form/ECheckBoxGroup.qml"},
    {"EPageHeader", "controls/navigation/EPageHeader.qml"},
    {"EParallaxView", "controls/effects/EParallaxView.qml"},
    {"ESplitterPanel", "controls/basic/ESplitterPanel.qml"},
    {"ECheckBoxButton", "controls/form/ECheckBoxButton.qml"},
    {"EDateTimePicker", "controls/form/EDateTimePicker.qml"},
    {"EInfiniteScroll", "controls/data/EInfiniteScroll.qml"},
    {"EShortcutPicker", "controls/form/EShortcutPicker.qml"},
    {"ENotification", "controls/indicator/ENotification.qml"},
    {"EConfigProvider", "controls/config/EConfigProvider.qml"},
    {"EDatePickerPanel", "controls/form/EDatePickerPanel.qml"},
    {"EStaggeredLayout", "controls/basic/EStaggeredLayout.qml"},
    {"EColorPickerPanel", "controls/form/EColorPickerPanel.qml"},
    {"ECircleProgress", "controls/indicator/ECircleProgress.qml"},
    {"EFloatButtonGroup", "controls/basic/EFloatButtonGroup.qml"},
    {"ECommandPalette", "controls/navigation/ECommandPalette.qml"},
    {"EApplicationWindow", "controls/basic/EApplicationWindow.qml"},
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
    qmlRegisterType<ECodeTools>("EUI.Private", major, minor, "CodeTools");
    qmlRegisterType<EFilePickerUtils>("EUI.Private", major, minor, "PickerFileUtils");
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

    qmlRegisterSingletonType<EAnimation>(uri,
                                        major,
                                        minor,
                                        "EAnimation",
                                        [](QQmlEngine *engine, QJSEngine *) -> QObject *
                                        {
                                            return new EAnimation(engine);
                                        });

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
