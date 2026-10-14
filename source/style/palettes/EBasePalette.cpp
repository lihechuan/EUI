#include "EBasePalette.h"
#include "enum/EPalette.h"

EBasePalette::EBasePalette(QObject *parent) : QObject(parent)
{
}

QColor EBasePalette::white() const
{
    return QColor(EPalette::Base::White);
}

QColor EBasePalette::black() const
{
    return QColor(EPalette::Base::Black);
}

QColor EBasePalette::transparent() const
{
    return QColor(EPalette::Base::Transparent);
}

QColor EBasePalette::blue() const
{
    return QColor(EPalette::Seed::Primary);
}

QColor EBasePalette::lightGray() const
{
    return QColor(EPalette::Neutral::N500);
}

QColor EBasePalette::slateGray() const
{
    return QColor(EPalette::Neutral::N700);
}

QColor EBasePalette::silver() const
{
    return QColor(EPalette::Neutral::N400);
}

QColor EBasePalette::charcoalGray() const
{
    return QColor(EPalette::Neutral::N800);
}

QColor EBasePalette::gainsboro() const
{
    return QColor(EPalette::Neutral::N300);
}

QColor EBasePalette::darkGray() const
{
    return QColor(EPalette::Neutral::N900);
}

QColor EBasePalette::ghostWhite() const
{
    return QColor(EPalette::Neutral::N50);
}

QColor EBasePalette::manatee() const
{
    return QColor(EPalette::Neutral::N500);
}

QColor EBasePalette::romanSilver() const
{
    return QColor(EPalette::Neutral::N600);
}

QColor EBasePalette::modalOverlay() const
{
    return QColor(0, 0, 0, 128);
}

QColor EBasePalette::modelessOverlay() const
{
    return QColor(0, 0, 0, 0);
}
