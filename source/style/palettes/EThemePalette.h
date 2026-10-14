#pragma once

#include <array>
#include <QColor>
#include <QObject>

#include "EColorScale.h"
#include "enum/EStyle.h"
#include "EThemeTokens.h"
#include "macros/singleton.h"

class EThemePalette : public QObject
{
    Q_OBJECT
    friend class EPaletteManager;
    SINGLETON(EThemePalette)

private:
    explicit EThemePalette(QObject *parent = nullptr);

public:
    QColor seedForStyle(int styleColor) const;
    const EThemeTokens &tokens(bool dark) const;
    Q_INVOKABLE QColor borderHover(bool dark) const;
    Q_INVOKABLE QColor shadowColor(bool dark) const;
    Q_INVOKABLE QColor textPrimary(bool dark) const;
    Q_INVOKABLE QColor textRegular(bool dark) const;
    Q_INVOKABLE QColor surfaceHover(bool dark) const;
    Q_INVOKABLE QColor textDisabled(bool dark) const;
    Q_INVOKABLE QColor borderDefault(bool dark) const;
    Q_INVOKABLE QColor textSecondary(bool dark) const;
    Q_INVOKABLE QColor borderDisabled(bool dark) const;
    Q_INVOKABLE QColor surfacePressed(bool dark) const;
    Q_INVOKABLE QColor fetchThemeColor(bool dark) const;
    Q_INVOKABLE QColor surfaceDisabled(bool dark) const;
    Q_INVOKABLE QColor surfaceComponent(bool dark) const;
    Q_INVOKABLE QColor fetchInverseThemeColor(bool dark) const;
    EColorScale scaleForColor(const QColor &color, bool dark) const;
    const EColorScale &scaleForStyle(int styleColor, bool dark) const;
    Q_INVOKABLE QColor fetchColorForStyle(int styleColor = static_cast<int>(EStyle::Component::Primary)) const;

    EThemeTokens m_darkTokens;
    EThemeTokens m_lightTokens;
    std::array<EColorScale, 6> m_darkScales;
    std::array<EColorScale, 6> m_lightScales;
};
