#include "EUI.h"
#include "EUIPlugin.h"

EUIPlugin::EUIPlugin() = default;

void EUIPlugin::registerTypes(const char *uri)
{
    EUI::getInstance()->registerTypes(uri);
}

void EUIPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    EUI::getInstance()->initializeEngine(engine, uri);
}
