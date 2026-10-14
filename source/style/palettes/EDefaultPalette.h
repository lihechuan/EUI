#pragma once

#include <QColor>
#include <QObject>

#include "macros/singleton.h"

class EDefaultPalette : public QObject
{
    Q_OBJECT
    friend class EPaletteManager;
    SINGLETON(EDefaultPalette)

    explicit EDefaultPalette(QObject *parent = nullptr);

    Q_INVOKABLE QColor resolveThemeBorderColor() const;
    Q_INVOKABLE QColor resolveThemeBackgroundColor(bool dark) const;
};
