#pragma once

#include <QObject>
#include <QQmlEngine>

#include "macros/singleton.h"

class EUI : public QObject
{
    Q_OBJECT
    SINGLETON(EUI)

    void registerTypes(const char *uri) const;
    Q_DECL_EXPORT void registerTypes(QQmlEngine *engine);
    void initializeEngine(QQmlEngine *engine, [[maybe_unused]] const char *uri);

private:
    const int major_ = 1;
    const int minor_ = 0;
    const char *uri_ = "EUI";
};
