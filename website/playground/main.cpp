#include <QQmlError>
#include <QQuickItem>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFontDatabase>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QInputMethodEvent>
#include <emscripten/bind.h>

#include "enum/EIcons.h"
#include "enum/EStyle.h"
#include "config/ELocale.h"
#include "layout/ELayout.h"
#include "utils/EImageIO.h"
#include "utils/ETableIO.h"
#include "enum/EPlacement.h"
#include "utils/EAnimation.h"
#include "utils/ECodeTools.h"
#include "utils/EDataUtils.h"
#include "enum/EOrientation.h"
#include "utils/RandomUtils.h"
#include "config/EValueUtils.h"
#include "style/theme/ETheme.h"
#include "utils/ETextDocument.h"
#include "config/EDefaultConfig.h"
#include "enum/EValidationState.h"
#include "utils/EFilePickerUtils.h"
#include "component/EChartRenderer.h"
#include "component/EMasonryLayout.h"
#include "component/EWindowActions.h"
#include "component/EQrCodeRenderer.h"
#include "component/EShadowRenderer.h"
#include "component/ECaptchaRenderer.h"
#include "component/EBackdropRenderer.h"
#include "state/EComponentStateObject.h"
#include "component/EAdvancedRectangle.h"
#include "component/EWatermarkRenderer.h"
#include "../../example/src/ExampleData.h"
#include "style/palettes/EPaletteManager.h"

namespace
{
QQuickView *view = nullptr;
QQuickItem *preview = nullptr;

struct QmlType
{
    const char *uri;
    const char *name;
    const char *path;
};

const QmlType types[] = {
#include "qml-types.h"
};

std::string errorsFor(const QList<QQmlError> &errors)
{
    QStringList messages;
    for (const auto &error : errors)
        messages.append(QStringLiteral("%1:%2 %3").arg(error.line()).arg(error.column()).arg(error.description()));
    return messages.join(QLatin1Char('\n')).toStdString();
}

void inputText(const std::string &preedit, const std::string &commit)
{
    QObject *target = QGuiApplication::focusObject();
    if (!target)
        return;
    QInputMethodEvent event(QString::fromStdString(preedit), {});
    event.setCommitString(QString::fromStdString(commit));
    QGuiApplication::sendEvent(target, &event);
}

emscripten::val inputState()
{
    auto state = emscripten::val::object();
    auto *item = view ? view->activeFocusItem() : nullptr;
    state.set("enabled", false);
    if (!item)
        return state;
    QInputMethodQueryEvent event(Qt::ImEnabled | Qt::ImCursorRectangle);
    QGuiApplication::sendEvent(item, &event);
    const QRectF cursor = item->mapRectToScene(event.value(Qt::ImCursorRectangle).toRectF());
    state.set("enabled", event.value(Qt::ImEnabled).toBool());
    state.set("x", cursor.x());
    state.set("y", cursor.bottom());
    return state;
}

void setAppearance(bool dark, const std::string &language)
{
    ETheme::getInstance()->setDark(dark);
    ELocale::getInstance()->setLanguage(QString::fromStdString(language));
}

std::string runQml(const std::string &source, bool dark, const std::string &language)
{
    if (!view || !view->rootObject())
        return "The QML engine is not ready.";
    if (source.empty() || source.size() > 65536)
        return "Enter between 1 and 65536 bytes of QML.";

    setAppearance(dark, language);
    QQmlComponent component(view->engine());
    component.setData(QByteArray::fromStdString(source), QUrl(QStringLiteral("qrc:/playground/Example.qml")));
    if (component.isError())
        return errorsFor(component.errors());

    QObject *object = component.beginCreate(view->engine()->rootContext());
    auto *item = qobject_cast<QQuickItem *>(object);
    if (!item)
    {
        component.completeCreate();
        delete object;
        return component.isError() ? errorsFor(component.errors()) : "The root object must be a visual QML item.";
    }

    item->setParent(view->rootObject());
    item->setParentItem(view->rootObject()->findChild<QQuickItem *>(QStringLiteral("previewHost")));
    component.completeCreate();
    if (component.isError())
    {
        delete item;
        return errorsFor(component.errors());
    }
    delete preview;
    preview = item;
    return {};
}

void registerTypes(QQmlEngine *engine)
{
    qmlRegisterType<EComponentStateObject>("EUI", 1, 0, "EComponentStateObject");
    qmlRegisterType<EAdvancedRectangle>("EUI", 1, 0, "EAdvancedRectangle");
    qmlRegisterType<ECodeTools>("EUI.Private", 1, 0, "CodeTools");
    qmlRegisterType<EImageIO>("EUI.Private", 1, 0, "ImageIO");
    qmlRegisterType<ETextDocument>("EUI.Private", 1, 0, "TextDocument");
    qmlRegisterType<EChartRenderer>("EUI.Private", 1, 0, "ChartRenderer");
    qmlRegisterType<EMasonryLayout>("EUI.Private", 1, 0, "MasonryLayout");
    qmlRegisterType<EQrCodeRenderer>("EUI.Private", 1, 0, "QrCodeRenderer");
    qmlRegisterType<EShadowRenderer>("EUI.Private", 1, 0, "ShadowRenderer");
    qmlRegisterType<ECaptchaRenderer>("EUI.Private", 1, 0, "CaptchaRenderer");
    qmlRegisterType<EWatermarkRenderer>("EUI.Private", 1, 0, "WatermarkRenderer");
    qmlRegisterType<EBackdropRenderer>("EUI.Private", 1, 0, "BackdropRenderer");
    qmlRegisterType<EWindowActions>("EUI.Private", 1, 0, "WindowActions");
    qmlRegisterType<EFilePickerUtils>("EUI.Private", 1, 0, "PickerFileUtils");
    qmlRegisterSingletonType<EAnimation>("EUI", 1, 0, "EAnimation",
        [](QQmlEngine *engine, QJSEngine *) -> QObject * { return new EAnimation(engine); });
    for (const auto &type : types)
        qmlRegisterType(QUrl(QStringLiteral("qrc:/qt/qml/EUI/") + QString::fromLatin1(type.path)),
                        type.uri, 1, 0, type.name);
    qmlRegisterUncreatableMetaObject(EIcons::staticMetaObject, "EUI", 1, 0, "EIcons", "Enums only");
    qmlRegisterUncreatableMetaObject(EStyle::staticMetaObject, "EUI", 1, 0, "EStyle", "Enums only");
    qmlRegisterUncreatableMetaObject(EPlacement::staticMetaObject, "EUI", 1, 0, "EPlacement", "Enums only");
    qmlRegisterUncreatableMetaObject(EValidationState::staticMetaObject, "EUI", 1, 0, "EValidationState", "Enums only");
    qmlRegisterUncreatableMetaObject(EOrientation::staticMetaObject, "EUI", 1, 0, "EOrientation", "Enums only");
    engine->rootContext()->setContextProperty("ELayout", ELayout::getInstance());
    engine->rootContext()->setContextProperty("RandomUtils", RandomUtils::getInstance());
    engine->rootContext()->setContextProperty("ETableIO", new ETableIO(engine));
    engine->rootContext()->setContextProperty("EValueUtils", new EValueUtils(engine));
    engine->rootContext()->setContextProperty("ETheme", ETheme::getInstance());
    engine->rootContext()->setContextProperty("ELocale", ELocale::getInstance());
    engine->rootContext()->setContextProperty("EDefaultConfig", EDefaultConfig::getInstance());
    engine->rootContext()->setContextProperty("EPaletteManager", EPaletteManager::getInstance());
    auto *examples = new ExampleData(engine);
    examples->setLocale(ELocale::getInstance());
    engine->rootContext()->setContextProperty("exampleData", examples);
    auto *data = new EDataUtils(engine);
    engine->rootContext()->setContextProperty("EDataUtils", data);
    engine->globalObject().setProperty(QStringLiteral("EDataUtils"), engine->newQObject(data));
}
}

EMSCRIPTEN_BINDINGS(eui_playground)
{
    emscripten::function("inputText", &inputText);
    emscripten::function("inputState", &inputState);
    emscripten::function("runQml", &runQml);
    emscripten::function("setAppearance", &setAppearance);
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("Default"));
    const int fontId = QFontDatabase::addApplicationFont(QStringLiteral(":/playground/NotoSansSC.ttf"));
    const QStringList families = QFontDatabase::applicationFontFamilies(fontId);
    QFont font(families.isEmpty() ? QStringLiteral("sans-serif") : families.first());
    font.setPixelSize(14);
    app.setFont(font);
    QQuickView window;
    view = &window;
    registerTypes(window.engine());
    window.setResizeMode(QQuickView::SizeRootObjectToView);
    window.setSource(QUrl(QStringLiteral("qrc:/playground/Main.qml")));
    window.resize(640, 420);
    window.show();
    return app.exec();
}
