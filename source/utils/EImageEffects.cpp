#include <array>
#include <QtGlobal>

#include "EImageEffects.h"

namespace
{
using Channels = std::array<int, 4>;

Channels channels(QRgb pixel)
{
    return {qRed(pixel), qGreen(pixel), qBlue(pixel), qAlpha(pixel)};
}

void add(Channels &sum, QRgb pixel, int factor)
{
    const Channels value = channels(pixel);
    for (int channel = 0; channel < 4; ++channel)
        sum[channel] += value[channel] * factor;
}

QImage boxPass(const QImage &source, int radius, bool horizontal)
{
    QImage result(source.size(), QImage::Format_ARGB32_Premultiplied);
    const int rows    = horizontal ? source.height() : source.width();
    const int length  = horizontal ? source.width() : source.height();
    const int divisor = radius * 2 + 1;

    auto pixel = [&](int row, int index)
    {
        const int bounded = qBound(0, index, length - 1);
        return horizontal ? source.pixel(bounded, row) : source.pixel(row, bounded);
    };

    for (int row = 0; row < rows; ++row)
    {
        Channels sum = {};
        for (int index = -radius; index <= radius; ++index)
            add(sum, pixel(row, index), 1);

        for (int index = 0; index < length; ++index)
        {
            const QRgb value = qRgba(sum[0] / divisor, sum[1] / divisor, sum[2] / divisor, sum[3] / divisor);
            if (horizontal)
                reinterpret_cast<QRgb *>(result.scanLine(row))[index] = value;
            else
                reinterpret_cast<QRgb *>(result.scanLine(index))[row] = value;

            add(sum, pixel(row, index - radius), -1);
            add(sum, pixel(row, index + radius + 1), 1);
        }
    }
    return result;
}
}

QImage EImageEffects::blur(const QImage &source, int radius)
{
    if (source.isNull())
        return {};

    QImage result     = source.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    const int bounded = qBound(0, radius, 64);
    if (bounded == 0)
        return result;

    const int passRadius = qMax(1, bounded / 2);
    for (int pass = 0; pass < 3; ++pass)
    {
        result = boxPass(result, passRadius, true);
        result = boxPass(result, passRadius, false);
    }
    return result;
}
