#include <QtMath>
#include <QStringView>

#include "ColorUtils.h"

namespace ColorUtils
{
QColor mix(const QColor &foreground, const QColor &background, double backgroundWeight)
{
    const double weight           = qBound(0.0, backgroundWeight, 1.0);
    const double foregroundWeight = 1.0 - weight;

    return QColor::fromRgbF(foreground.redF() * foregroundWeight + background.redF() * weight,
                            foreground.greenF() * foregroundWeight + background.greenF() * weight,
                            foreground.blueF() * foregroundWeight + background.blueF() * weight,
                            foreground.alphaF() * foregroundWeight + background.alphaF() * weight);
}

double linearChannel(double channel)
{
    return channel <= 0.04045 ? channel / 12.92 : qPow((channel + 0.055) / 1.055, 2.4);
}

double relativeLuminance(const QColor &color)
{
    return 0.2126 * linearChannel(color.redF()) + 0.7152 * linearChannel(color.greenF()) +
           0.0722 * linearChannel(color.blueF());
}

double contrastRatio(const QColor &first, const QColor &second)
{
    const double firstLuminance  = relativeLuminance(first);
    const double secondLuminance = relativeLuminance(second);
    const double lighter         = qMax(firstLuminance, secondLuminance);
    const double darker          = qMin(firstLuminance, secondLuminance);
    return (lighter + 0.05) / (darker + 0.05);
}

QColor readableForeground(const QColor &background, const QColor &light, const QColor &dark)
{
    return contrastRatio(background, light) >= contrastRatio(background, dark) ? light : dark;
}

QList<int> hex2Rgb(const QString &color)
{
    QList<int> rgb = {0, 0, 0};

    if (color.isEmpty())
        return rgb;

    auto hexRef = QStringView(color).mid(color.startsWith('#') ? 1 : 0);

    // 如果包含透明度（AARRGGBB），则跳过前两位透明度部分
    if (hexRef.length() == 8)
        hexRef = hexRef.mid(2, 6);

    for (int i = 0; i < 3; ++i)
    {
        bool ok = false;
        rgb[i]  = hexRef.mid(i * 2, 2).toString().toInt(&ok, 16);
        if (!ok)
            rgb[i] = 0;
    }

    return rgb;
}

QString rgb2Hex(int r, int g, int b)
{
    return QString::asprintf("#%02X%02X%02X", qBound(0, r, 255), qBound(0, g, 255), qBound(0, b, 255));
}

QColor adjustColor(const QColor &color, double level, bool makeLighter)
{
    QColor result = color;
    int r, g, b, a;
    result.getRgb(&r, &g, &b, &a);

    const double normalizedLevel = qBound(0.0, level, 1.0);

    if (makeLighter)
    {
        r = qMin(255, static_cast<int>((255 - r) * normalizedLevel + r));
        g = qMin(255, static_cast<int>((255 - g) * normalizedLevel + g));
        b = qMin(255, static_cast<int>((255 - b) * normalizedLevel + b));
    }
    else
    {
        r = qMax(0, static_cast<int>(r * (1 - normalizedLevel)));
        g = qMax(0, static_cast<int>(g * (1 - normalizedLevel)));
        b = qMax(0, static_cast<int>(b * (1 - normalizedLevel)));
    }

    // 保留原始 alpha，避免半透明自定义颜色在状态变化后变成不透明
    result.setRgb(r, g, b, a);
    return result;
}

QColor lighten(const QString &color, double level)
{
    return adjustColor(QColor(color), level, true);
}

QColor darken(const QString &color, double level)
{
    return adjustColor(QColor(color), level, false);
}

QColor lighten(const QColor &color, double level)
{
    return adjustColor(color, level, true);
}

QColor darken(const QColor &color, double level)
{
    return adjustColor(color, level, false);
}
}
