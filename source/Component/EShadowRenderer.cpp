#include <QPainter>

#include "EShadowRenderer.h"

EShadowRenderer::EShadowRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

QColor EShadowRenderer::color() const
{
    return color_;
}
qreal EShadowRenderer::radius() const
{
    return radius_;
}
int EShadowRenderer::elevation() const
{
    return elevation_;
}

void EShadowRenderer::setColor(const QColor &color)
{
    if (color_ == color)
        return;
    color_ = color;
    update();
    emit changed();
}

void EShadowRenderer::setRadius(qreal radius)
{
    radius = qIsFinite(radius) ? qMax<qreal>(0, radius) : 0;
    if (qFuzzyCompare(radius_, radius))
        return;
    radius_ = radius;
    update();
    emit changed();
}

void EShadowRenderer::setElevation(int elevation)
{
    elevation = qBound(0, elevation, 32);
    if (elevation_ == elevation)
        return;
    elevation_ = elevation;
    update();
    emit changed();
}

void EShadowRenderer::paint(QPainter *painter)
{
    if (!elevation_ || boundingRect().isEmpty())
        return;
    painter->save();
    painter->setBrush(Qt::NoBrush);
    for (int layer = elevation_; layer > 0; --layer)
    {
        const qreal distance = elevation_ - layer + 0.5;
        painter->setOpacity(0.012 * layer);
        painter->setPen(QPen(color_, 1));
        const QRectF rect = boundingRect().adjusted(distance, distance, -distance, -distance);
        painter->drawRoundedRect(rect, radius_ + layer, radius_ + layer);
    }
    painter->restore();
}
