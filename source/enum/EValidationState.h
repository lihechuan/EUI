#pragma once

#include <QObject>
#include <QQmlEngine>

namespace EValidationState
{
Q_NAMESPACE

enum class State
{
    None    = 0x0000,
    Success = 0x0001,
    Warning = 0x0002,
    Error   = 0x0003,
};

Q_ENUM_NS(State);
QML_NAMED_ELEMENT(EValidationState);
}
