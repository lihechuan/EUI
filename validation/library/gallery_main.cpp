#include <QFont>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QGuiApplication>
#include <QCoreApplication>
#include <QtQuickTest/quicktest.h>
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

class GallerySetup : public QObject
{
    Q_OBJECT

public slots:
    void qmlEngineAvailable(QQmlEngine *engine);
};

void GallerySetup::qmlEngineAvailable(QQmlEngine *engine)
{
#if defined(Q_OS_WIN64) && QT_VERSION == QT_VERSION_CHECK(5, 15, 2)
    engine->rootContext()->setContextProperty(QStringLiteral("galleryAsyncLoading"), false);
#else
    engine->rootContext()->setContextProperty(QStringLiteral("galleryAsyncLoading"), true);
#endif
    QCoreApplication::setOrganizationName(QStringLiteral("EUI"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("eui.local"));
    QQmlComponent component(engine);
    component.setData("import EUI 1.0\nimport QtQml 2.15\nQtObject {}", QUrl());
    delete component.create();
    QFont interfaceFont = QGuiApplication::font();
    interfaceFont.setPixelSize(14);
    interfaceFont.setWeight(QFont::Normal);
    QGuiApplication::setFont(interfaceFont);
    auto *data = new ExampleData(engine);
    auto *catalog = new ExampleCatalog(engine);
    auto *updater = new ExampleUpdater(engine);
    auto *clipboard = new ExampleClipboard(engine);
    auto *runtime = new Example3DRuntime(engine);
    QObject *locale = engine->rootContext()->contextProperty(QStringLiteral("ELocale")).value<QObject *>();
    data->setLocale(locale);
    catalog->setLocale(locale);
    engine->rootContext()->setContextProperty(QStringLiteral("exampleData"), data);
    engine->rootContext()->setContextProperty(QStringLiteral("exampleCatalog"), catalog);
    engine->rootContext()->setContextProperty(QStringLiteral("exampleUpdater"), updater);
    engine->rootContext()->setContextProperty(QStringLiteral("exampleClipboard"), clipboard);
    engine->rootContext()->setContextProperty(QStringLiteral("exampleRuntime"), runtime);
}

int main(int argc, char **argv)
{
#if defined(Q_OS_WIN64) && QT_VERSION == QT_VERSION_CHECK(5, 15, 2)
    qputenv("QV4_FORCE_INTERPRETER", "1");
#endif
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#ifdef Q_OS_WIN
    qputenv("QTWEBENGINE_CHROMIUM_FLAGS", qgetenv("QTWEBENGINE_CHROMIUM_FLAGS") + " --disable-gpu");
#endif
    QtWebEngine::initialize();
#else
    QtWebEngineQuick::initialize();
#endif
    GallerySetup setup;
    return quick_test_main_with_setup(argc, argv, "eui_gallery", nullptr, &setup);
}

#include "gallery_main.moc"
