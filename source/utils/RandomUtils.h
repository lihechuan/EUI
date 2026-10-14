#pragma once

#include <QObject>
#include <QString>

#include "macros/singleton.h"

class RandomUtils : public QObject
{
    Q_OBJECT
    SINGLETON(RandomUtils)

    explicit RandomUtils(QObject *parent = nullptr);

    Q_INVOKABLE QString nanoid(int size = 21);
};
