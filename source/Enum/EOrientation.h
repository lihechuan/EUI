#pragma once

#include <QObject>
#include <QQmlEngine>

namespace EOrientation
{
Q_NAMESPACE

enum class Direction
{
    Horizontal = 0x0000,
    Vertical   = 0x0001,
};

Q_ENUM_NS(Direction);
QML_NAMED_ELEMENT(EOrientation);
}
