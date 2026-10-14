#include <QFont>
#include <QIcon>
#include <QTimer>
#include <QJsonObject>
#include <QQmlContext>
#include <QLibraryInfo>
#include <QQuickWindow>
#include <QJsonDocument>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#include <QtWebEngine/qtwebengineglobal.h>
#else
#include <QtWebEngineQuick/qtwebenginequickglobal.h>
#endif

#include "ExampleData.h"
#include "ExampleCatalog.h"
#include "ExampleUpdater.h"
#include "Example3DRuntime.h"
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
    app.setApplicationVersion(QStringLiteral(EUI_GALLERY_VERSION));
    app.setOrganizationDomain(QStringLiteral("eui.local"));
    app.setWindowIcon(QIcon(QStringLiteral(":/example/eui-mark.svg")));

    QCommandLineParser arguments;
    arguments.addHelpOption();
    arguments.addOption({QStringLiteral("dark"), QStringLiteral("Use the dark theme")});
    arguments.addOption({QStringLiteral("quick3d"), QStringLiteral("Open the Littlest Tokyo example")});
    arguments.addOption({QStringLiteral("language"), QStringLiteral("Interface language: zh or en"), QStringLiteral("language"), QStringLiteral("zh")});
    arguments.addOption({QStringLiteral("quick3d-ready-file"), QStringLiteral("Runtime handoff acknowledgement"), QStringLiteral("path")});
    arguments.addOption({QStringLiteral("quick3d-runtime-check"), QStringLiteral("Verify runtime installation and exit")});
    arguments.addOption({QStringLiteral("update-result"), QStringLiteral("Update installation acknowledgement"), QStringLiteral("path")});
    arguments.addOption({QStringLiteral("update-failed"), QStringLiteral("Report an update rollback")});
    arguments.process(app);

    ExampleData data(&app);
    ExampleCatalog catalog(&app);
    ExampleUpdater updater(&app, qEnvironmentVariable("EUI_GALLERY_INSTALL_DIR"));
    ExampleClipboard clipboard(&app);
    Example3DRuntime runtime(&app);
    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("exampleData"), &data);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleCatalog"), &catalog);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleUpdater"), &updater);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleClipboard"), &clipboard);
    engine.rootContext()->setContextProperty(QStringLiteral("exampleRuntime"), &runtime);
    QObject::connect(&runtime, &Example3DRuntime::handoffReady, &app, &QCoreApplication::quit);
    QObject::connect(&updater, &ExampleUpdater::restartReady, &app, &QCoreApplication::quit);
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
        theme->setProperty("dark", arguments.isSet(QStringLiteral("dark")));
    QFont interfaceFont = QGuiApplication::font();
    interfaceFont.setPixelSize(14);
    interfaceFont.setWeight(QFont::Normal);
    app.setFont(interfaceFont);
    QObject *locale = engine.rootContext()->contextProperty(QStringLiteral("ELocale")).value<QObject *>();
    if (locale)
        locale->setProperty("language", arguments.value(QStringLiteral("language")) == QStringLiteral("en")
            ? QStringLiteral("en") : QStringLiteral("zh"));
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
    if (arguments.isSet(QStringLiteral("quick3d")))
        engine.setInitialProperties({{QStringLiteral("currentComponent"), QStringLiteral("TokyoScene")},
            {QStringLiteral("currentSource"), QStringLiteral("Quick3DExamples.qml")}});
    engine.load(url);
    if (!engine.rootObjects().isEmpty())
    {
        if (arguments.isSet(QStringLiteral("quick3d-runtime-check")))
        {
            QStringList components;
            for (const auto &group : catalog.groups())
                for (const auto &entry : group.toMap().value(QStringLiteral("children")).toList())
                    components.append(entry.toMap().value(QStringLiteral("index")).toString());
            auto *window = engine.rootObjects().first();
            auto *version = window->findChild<QObject *>(QStringLiteral("galleryVersion"));
            auto *update = window->findChild<QObject *>(QStringLiteral("galleryUpdateButton"));
            const auto report = QJsonObject::fromVariantMap({
                {QStringLiteral("version"), app.applicationVersion()},
                {QStringLiteral("revision"), QStringLiteral(EUI_GALLERY_REVISION)},
                {QStringLiteral("components"), components},
                {QStringLiteral("footer"), version ? version->property("text") : QVariant()},
                {QStringLiteral("update"), update && update->property("visible").toBool()}});
            qInfo("QUICK3D_GALLERY %s", QJsonDocument(report).toJson(QJsonDocument::Compact).constData());
        }
        const auto result = arguments.value(QStringLiteral("update-result"));
        if (!result.isEmpty())
        {
            auto *window = qobject_cast<QQuickWindow *>(engine.rootObjects().first());
            QObject::connect(window, &QQuickWindow::frameSwapped, &updater, [&updater, result]() {
                if (updater.stage() != QStringLiteral("completed"))
                    updater.confirmReady(result);
            }, Qt::QueuedConnection);
        }
        if (arguments.isSet(QStringLiteral("update-failed")))
            updater.reportRollback();
        else if (result.isEmpty())
            QTimer::singleShot(0, &updater, &ExampleUpdater::check);
    }
    if (arguments.isSet(QStringLiteral("quick3d-runtime-check")) && !catalog.quick3DAvailable())
    {
        QTimer::singleShot(500, &runtime, [&runtime]() { runtime.install(QStringLiteral("zh"), false); });
        QObject::connect(&runtime, &Example3DRuntime::changed, &app, [&runtime]() {
            qInfo("QUICK3D_RUNTIME %s %.3f %s", qPrintable(runtime.stage()), runtime.progress(), qPrintable(runtime.error()));
            if (runtime.stage() == QStringLiteral("failed"))
                QCoreApplication::exit(1);
        });
    }

    return app.exec();
}
