#pragma once

#include <QColor>

enum class EColorTone
{
    Base,
    Soft3,
    Soft5,
    Soft7,
    Soft8,
    Soft9,
    Strong2,
};

class EColorScale
{
public:
    EColorScale() = default;
    EColorScale(const QColor &seed, const QColor &surface, const QColor &contrastSurface);

    QColor tone(EColorTone tone) const;

private:
    QColor m_base;
    QColor m_soft3;
    QColor m_soft5;
    QColor m_soft7;
    QColor m_soft8;
    QColor m_soft9;
    QColor m_strong2;
};
