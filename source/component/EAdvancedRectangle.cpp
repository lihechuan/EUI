#include <array>
#include <QPainterPath>

#include "EAdvancedRectangle.h"
#include "style/theme/ETheme.h"
#include "style/palettes/EPaletteManager.h"

namespace
{
using CornerRadii = std::array<qreal, 4>;

qreal scaleForEdge(qreal length, qreal firstRadius, qreal secondRadius)
{
    const qreal radiusSum = firstRadius + secondRadius;
    return radiusSum > 0.0 ? qMin(1.0, length / radiusSum) : 1.0;
}

CornerRadii normalizedRadii(const QList<int> &source, const QSizeF &size)
{
    CornerRadii radii = {0.0, 0.0, 0.0, 0.0};
    const int count   = qMin<int>(source.size(), static_cast<int>(radii.size()));
    for (int index = 0; index < count; ++index)
        radii[index] = qMax<qreal>(0.0, source[index]);

    qreal scale = 1.0;
    scale       = qMin(scale, scaleForEdge(size.width(), radii[0], radii[1]));
    scale       = qMin(scale, scaleForEdge(size.width(), radii[3], radii[2]));
    scale       = qMin(scale, scaleForEdge(size.height(), radii[0], radii[3]));
    scale       = qMin(scale, scaleForEdge(size.height(), radii[1], radii[2]));

    for (qreal &radius : radii)
        radius *= scale;
    return radii;
}

QPainterPath roundedPath(const QRectF &rect, const CornerRadii &radii)
{
    const qreal topLeft     = radii[0];
    const qreal topRight    = radii[1];
    const qreal bottomRight = radii[2];
    const qreal bottomLeft  = radii[3];

    QPainterPath path;
    path.moveTo(rect.left() + topLeft, rect.top());
    path.lineTo(rect.right() - topRight, rect.top());
    if (topRight > 0.0)
        path.arcTo(QRectF(rect.right() - 2.0 * topRight, rect.top(), 2.0 * topRight, 2.0 * topRight), 90.0, -90.0);

    path.lineTo(rect.right(), rect.bottom() - bottomRight);
    if (bottomRight > 0.0)
        path.arcTo(QRectF(rect.right() - 2.0 * bottomRight,
                          rect.bottom() - 2.0 * bottomRight,
                          2.0 * bottomRight,
                          2.0 * bottomRight),
                   0.0,
                   -90.0);

    path.lineTo(rect.left() + bottomLeft, rect.bottom());
    if (bottomLeft > 0.0)
        path.arcTo(
            QRectF(rect.left(), rect.bottom() - 2.0 * bottomLeft, 2.0 * bottomLeft, 2.0 * bottomLeft), 270.0, -90.0);

    path.lineTo(rect.left(), rect.top() + topLeft);
    if (topLeft > 0.0)
        path.arcTo(QRectF(rect.left(), rect.top(), 2.0 * topLeft, 2.0 * topLeft), 180.0, -90.0);

    path.closeSubpath();
    return path;
}
}

EAdvancedRectangle::EAdvancedRectangle(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    ETheme *theme          = ETheme::getInstance();
    EThemePalette *palette = EPaletteManager::getInstance()->getThemePalette();

    color(palette->fetchThemeColor(theme->dark()));
    radius({0, 0, 0, 0});
    borderWidth(0);
    borderColor(palette->fetchColorForStyle());
    borderStyle(Qt::SolidLine);
    dashPattern({});
    setAntialiasing(true);

    connect(this, &EAdvancedRectangle::colorChanged, this, [this]() { update(); });
    connect(this, &EAdvancedRectangle::radiusChanged, this, [this]() { update(); });
    connect(this, &EAdvancedRectangle::borderWidthChanged, this, [this]() { update(); });
    connect(this, &EAdvancedRectangle::borderColorChanged, this, [this]() { update(); });
    connect(this, &EAdvancedRectangle::borderStyleChanged, this, [this]() { update(); });
    connect(this, &EAdvancedRectangle::dashPatternChanged, this, [this]() { update(); });
}

bool EAdvancedRectangle::hasBorder() const
{
    return _borderWidth > 0.0 && _borderColor.isValid() && _borderColor.alpha() > 0;
}

void EAdvancedRectangle::paint(QPainter *painter)
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    QRectF rect = boundingRect();
    if (rect.isEmpty())
    {
        painter->restore();
        return;
    }

    const qreal borderWidth = qMin(qMax<qreal>(0.0, _borderWidth), qMin(rect.width(), rect.height()));
    const bool drawBorder   = hasBorder() && borderWidth > 0.0;

    if (drawBorder)
    {
        const qreal halfBorderWidth = borderWidth / 2.0;
        rect.adjust(halfBorderWidth, halfBorderWidth, -halfBorderWidth, -halfBorderWidth);
    }

    const CornerRadii radii = normalizedRadii(_radius, rect.size());
    const QPainterPath path = roundedPath(rect, radii);

    painter->fillPath(path, _color);

    if (drawBorder)
    {
        QPen pen(_borderColor, borderWidth, _borderStyle);
        if (_borderStyle == Qt::DashLine || _borderStyle == Qt::CustomDashLine)
            pen.setDashPattern(_dashPattern);
        painter->strokePath(path, pen);
    }

    painter->restore();
}
