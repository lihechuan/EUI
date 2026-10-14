#pragma once

#include <QObject>
#include <QQmlEngine>

namespace EStyle
{
Q_NAMESPACE

enum class Component
{
    Default = 0x0000,
    Primary = 0x0001,
    Success = 0x0002,
    Info    = 0x0003,
    Warning = 0x0004,
    Danger  = 0x0005,
};

enum class System
{
    Light = 0x0000,
    Dark  = 0x0001,
};

enum class Size
{
    Small  = 0x0000,
    Medium = 0x0001,
    Large  = 0x0002,
};

enum class Role
{
    Uncertain       = 0x0000,
    BorderColor     = 0x0001,
    TextColor       = 0x0002,
    Color           = 0x0003,
    BackgroundColor = 0x0004,
};

Q_ENUM_NS(Component);
Q_ENUM_NS(System);
Q_ENUM_NS(Size);
Q_ENUM_NS(Role);
QML_NAMED_ELEMENT(EStyle);
}
