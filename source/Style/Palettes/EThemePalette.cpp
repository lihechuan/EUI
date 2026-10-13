#include "EThemePalette.h"
#include "Enum/EPalette.h"
#include "Utils/ColorUtils.h"

namespace
{
constexpr int StyleCount = 6;

int normalizedStyle(int style)
{
    return style >= 0 && style < StyleCount ? style : static_cast<int>(EStyle::Component::Primary);
}
}

EThemePalette::EThemePalette(QObject *parent) : QObject(parent)
{
    using namespace EPalette;

    const QColor darkSurface(Neutral::N950);
    const QColor lightSurface(Neutral::N0);

    m_lightTokens = {
        QColor(Neutral::N900),
        QColor(Neutral::N700),
        QColor(Neutral::N400),
        QColor(Neutral::N600),
        lightSurface,
        QColor(Neutral::N50),
        QColor(Neutral::N100),
        QColor(Neutral::N50),
        lightSurface,
        QColor(Neutral::N400),
        QColor(Neutral::N300),
        QColor(Neutral::N200),
        lightSurface,
        darkSurface,
    };

    m_darkTokens = {
        QColor(Neutral::N100),
        QColor(Neutral::N400),
        QColor(Neutral::N700),
        QColor(Neutral::N500),
        darkSurface,
        ColorUtils::mix(darkSurface, lightSurface, 0.08),
        ColorUtils::mix(darkSurface, lightSurface, 0.12),
        ColorUtils::mix(darkSurface, lightSurface, 0.06),
        ColorUtils::mix(darkSurface, lightSurface, 0.04),
        QColor(Neutral::N700),
        QColor(Neutral::N800),
        QColor(Neutral::N850),
        lightSurface,
        darkSurface,
    };

    for (int style = 0; style < StyleCount; ++style)
    {
        const QColor seed    = seedForStyle(style);
        m_lightScales[style] = EColorScale(seed, lightSurface, darkSurface);
        m_darkScales[style]  = EColorScale(seed, darkSurface, lightSurface);
    }
}

QColor EThemePalette::fetchThemeColor(bool dark) const
{
    return tokens(dark).surfacePage;
}

QColor EThemePalette::fetchInverseThemeColor(bool dark) const
{
    return dark ? tokens(dark).onDark : tokens(dark).onLight;
}

QColor EThemePalette::fetchColorForStyle(int styleColor) const
{
    return seedForStyle(styleColor);
}

const EThemeTokens &EThemePalette::tokens(bool dark) const
{
    return dark ? m_darkTokens : m_lightTokens;
}

const EColorScale &EThemePalette::scaleForStyle(int styleColor, bool dark) const
{
    const int style = normalizedStyle(styleColor);
    return dark ? m_darkScales[style] : m_lightScales[style];
}

EColorScale EThemePalette::scaleForColor(const QColor &color, bool dark) const
{
    const EThemeTokens &theme = tokens(dark);
    return EColorScale(color, theme.surfacePage, dark ? theme.onDark : theme.onLight);
}

QColor EThemePalette::seedForStyle(int styleColor) const
{
    using namespace EPalette;

    switch (static_cast<EStyle::Component>(normalizedStyle(styleColor)))
    {
    case EStyle::Component::Success:
        return QColor(Seed::Success);
    case EStyle::Component::Info:
        return QColor(Seed::Info);
    case EStyle::Component::Warning:
        return QColor(Seed::Warning);
    case EStyle::Component::Danger:
        return QColor(Seed::Danger);
    default:
        return QColor(Seed::Primary);
    }
}

QColor EThemePalette::textPrimary(bool dark) const
{
    return tokens(dark).textPrimary;
}

QColor EThemePalette::textRegular(bool dark) const
{
    return tokens(dark).textRegular;
}

QColor EThemePalette::textSecondary(bool dark) const
{
    return tokens(dark).textSecondary;
}

QColor EThemePalette::textDisabled(bool dark) const
{
    return tokens(dark).textDisabled;
}

QColor EThemePalette::surfaceComponent(bool dark) const
{
    return tokens(dark).surfaceComponent;
}

QColor EThemePalette::surfaceHover(bool dark) const
{
    return tokens(dark).surfaceHover;
}

QColor EThemePalette::surfacePressed(bool dark) const
{
    return tokens(dark).surfacePressed;
}

QColor EThemePalette::surfaceDisabled(bool dark) const
{
    return tokens(dark).surfaceDisabled;
}

QColor EThemePalette::borderDefault(bool dark) const
{
    return tokens(dark).borderDefault;
}

QColor EThemePalette::borderHover(bool dark) const
{
    return tokens(dark).borderHover;
}

QColor EThemePalette::shadowColor(bool dark) const
{
    return QColor(dark ? EPalette::Neutral::N950 : EPalette::Neutral::N700);
}

QColor EThemePalette::borderDisabled(bool dark) const
{
    return tokens(dark).borderDisabled;
}
