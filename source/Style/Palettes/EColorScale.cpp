#include "EColorScale.h"
#include "Utils/ColorUtils.h"

EColorScale::EColorScale(const QColor &seed, const QColor &surface, const QColor &contrastSurface)
    : m_base(seed), m_soft3(ColorUtils::mix(seed, surface, 0.3)), m_soft5(ColorUtils::mix(seed, surface, 0.5)),
      m_soft7(ColorUtils::mix(seed, surface, 0.7)), m_soft8(ColorUtils::mix(seed, surface, 0.8)),
      m_soft9(ColorUtils::mix(seed, surface, 0.9)), m_strong2(ColorUtils::mix(seed, contrastSurface, 0.2))
{
}

QColor EColorScale::tone(EColorTone tone) const
{
    switch (tone)
    {
    case EColorTone::Soft3:
        return m_soft3;
    case EColorTone::Soft5:
        return m_soft5;
    case EColorTone::Soft7:
        return m_soft7;
    case EColorTone::Soft8:
        return m_soft8;
    case EColorTone::Soft9:
        return m_soft9;
    case EColorTone::Strong2:
        return m_strong2;
    default:
        return m_base;
    }
}
