#pragma once

#include <QList>
#include <QColor>
#include <QString>

namespace ColorUtils
{
double linearChannel(double channel);
QString rgb2Hex(int r, int g, int b);
QList<int> hex2Rgb(const QString &color);
double relativeLuminance(const QColor &color);
QColor darken(const QColor &color, double level);
QColor darken(const QString &color, double level);
QColor lighten(const QColor &color, double level);
QColor lighten(const QString &color, double level);
double contrastRatio(const QColor &first, const QColor &second);
QColor adjustColor(const QColor &color, double level, bool makeLighter);
QColor mix(const QColor &foreground, const QColor &background, double backgroundWeight);
QColor readableForeground(const QColor &background, const QColor &light, const QColor &dark);
}
