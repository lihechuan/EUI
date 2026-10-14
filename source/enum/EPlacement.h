#pragma once

#include <QObject>
#include <QQmlEngine>

namespace EPlacement
{
Q_NAMESPACE

enum class Position
{
    TopLeft     = 0x0000,
    Top         = 0x0001,
    TopRight    = 0x0002,
    Left        = 0x0003,
    Center      = 0x0004,
    Right       = 0x0005,
    BottomLeft  = 0x0006,
    Bottom      = 0x0007,
    BottomRight = 0x0008,
};

Q_ENUM_NS(Position);
QML_NAMED_ELEMENT(EPlacement);
}
