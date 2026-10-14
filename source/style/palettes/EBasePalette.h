#pragma once

#include <QColor>
#include <QObject>

#include "macros/singleton.h"

class EBasePalette : public QObject
{
    Q_OBJECT
    friend class EPaletteManager;
    SINGLETON(EBasePalette)

    Q_PROPERTY(QColor blue READ blue CONSTANT)
    Q_PROPERTY(QColor black READ black CONSTANT)
    Q_PROPERTY(QColor white READ white CONSTANT)
    Q_PROPERTY(QColor silver READ silver CONSTANT)
    Q_PROPERTY(QColor manatee READ manatee CONSTANT)
    Q_PROPERTY(QColor darkGray READ darkGray CONSTANT)
    Q_PROPERTY(QColor gainsboro READ gainsboro CONSTANT)
    Q_PROPERTY(QColor lightGray READ lightGray CONSTANT)
    Q_PROPERTY(QColor slateGray READ slateGray CONSTANT)
    Q_PROPERTY(QColor ghostWhite READ ghostWhite CONSTANT)
    Q_PROPERTY(QColor romanSilver READ romanSilver CONSTANT)
    Q_PROPERTY(QColor transparent READ transparent CONSTANT)
    Q_PROPERTY(QColor charcoalGray READ charcoalGray CONSTANT)
    Q_PROPERTY(QColor modalOverlay READ modalOverlay CONSTANT)
    Q_PROPERTY(QColor modelessOverlay READ modelessOverlay CONSTANT)

private:
    explicit EBasePalette(QObject *parent = nullptr);

public:
    QColor blue() const;
    QColor black() const;
    QColor white() const;
    QColor silver() const;
    QColor manatee() const;
    QColor darkGray() const;
    QColor gainsboro() const;
    QColor lightGray() const;
    QColor slateGray() const;
    QColor ghostWhite() const;
    QColor romanSilver() const;
    QColor transparent() const;
    QColor charcoalGray() const;
    QColor modalOverlay() const;
    QColor modelessOverlay() const;
};
