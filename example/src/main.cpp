#include <QFont>
#include <QIcon>
#include <QQmlContext>
#include <QLibraryInfo>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QQmlApplicationEngine>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtWebEngine/qtwebengineglobal.h>
#else
#include <QtWebEngineQuick/qtwebenginequickglobal.h>
#endif

#include "ExampleData.h"
#include "ExampleCatalog.h"
#include "ExampleClipboard.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
#ifdef Q_OS_WIN
    QCoreApplication::setAttribute(Qt::AA_UseDesktopOpenGL);
#else
    QCoreApplication::setAttribute(Qt::AA_UseOpenGLES);
#endif

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef Q_OS_WIN
    // Qt 5 WebEngine GPU compositing can crash the display driver when scrolling and recreating chart views.
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", qgetenv("QTWEBENGINE_CHROMIUM_FLAGS") + " --disable-gpu");
#endif
    QtWebEngine::initialize();
#else
    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        qputenv("QT_QUICK_CONTROLS_STYLE", "Basic");
    QtWebEngineQuick::initialize();
#endif
    QGuiApplication app(argc, argv);
    app.setOrganizationName(QStringLiteral("EUI"));
    app.setOrganizationDomain(QStringLiteral("eui.local"));
    app.setWindowIcon(QIcon(QStringLiteral(":/example/eui-mark.svg")));

    ExampleData data(&app);
    ExampleCatalog catalog(&app);
    ExampleClipboard clipboard(&app);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("exampleData"), &data);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleCatalog"), &catalog);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleClipboard"), &clipboard);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    engine.addImportPath(QLibraryInfo::location(QLibraryInfo::Qml2ImportsPath));
#else
    engine.addImportPath(QLibraryInfo::path(QLibraryInfo::QmlImportsPath));
#endif
    engine.addImportPath(QCoreApplication::applicationDirPath());

    QQmlComponent euiModule(&engine);
    euiModule.setData("import EUI 1.0\nimport QtQml 2.15\nQtObject {}", QUrl());
    auto *marker = euiModule.create();
    if (!marker)
    {
        qWarning("%s", qPrintable(euiModule.errorString()));
        return -1;
    }
    delete marker;
    if (auto *theme = engine.rootContext()->contextProperty(QStringLiteral("ETheme")).value<QObject *>())
        theme->setProperty("dark", false);
    QFont interfaceFont = QGuiApplication::font();
    interfaceFont.setPixelSize(14);
    interfaceFont.setWeight(QFont::Normal);
    app.setFont(interfaceFont);
    QObject *locale = engine.rootContext()->contextProperty(QStringLiteral("ELocale")).value<QObject *>();
    data.setLocale(locale);
    catalog.setLocale(locale);

    const QUrl url(QStringLiteral("qrc:/example/qml/component/App.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl)
        {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
