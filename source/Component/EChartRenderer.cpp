#include <QtMath>
#include <QLocale>
#include <QPainter>
#include <QPainterPath>

#include "EChartRenderer.h"

EChartRenderer::EChartRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
    setImplicitSize(480, 260);
}

int EChartRenderer::type() const
{
    return type_;
}
QColor EChartRenderer::color() const
{
    return color_;
}
QVariantList EChartRenderer::model() const
{
    return model_;
}
QColor EChartRenderer::textColor() const
{
    return textColor_;
}
QColor EChartRenderer::gridColor() const
{
    return gridColor_;
}

void EChartRenderer::setType(int type)
{
    type = qBound(0, type, 2);
    if (type_ == type)
        return;
    type_ = type;
    update();
    emit changed();
}

void EChartRenderer::setModel(const QVariantList &model)
{
    if (model_ == model)
        return;
    model_ = model;
    updatePoints();
    update();
    emit changed();
}

void EChartRenderer::setColor(const QColor &color)
{
    if (color_ == color)
        return;
    color_ = color;
    update();
    emit changed();
}

void EChartRenderer::setTextColor(const QColor &color)
{
    if (textColor_ == color)
        return;
    textColor_ = color;
    update();
    emit changed();
}

void EChartRenderer::setGridColor(const QColor &color)
{
    if (gridColor_ == color)
        return;
    gridColor_ = color;
    update();
    emit changed();
}

void EChartRenderer::updatePoints()
{
    values_.clear();
    labels_.clear();
    minimum_ = 0;
    maximum_ = 0;
    for (int index = 0; index < model_.size(); ++index)
    {
        const QVariant &entry    = model_[index];
        const QVariantMap record = entry.toMap();
        bool valid               = false;
        const qreal value        = (record.isEmpty() ? entry : record.value(QStringLiteral("value"))).toDouble(&valid);
        if (!valid || !qIsFinite(value))
            continue;
        values_.append(QPointF(index, value));
        labels_.append(record.value(QStringLiteral("label"), QString::number(index + 1)).toString());
        minimum_ = qMin(minimum_, value);
        maximum_ = qMax(maximum_, value);
    }
    if (qFuzzyCompare(minimum_, maximum_))
        maximum_ = minimum_ + 1;
}

void EChartRenderer::paint(QPainter *painter)
{
    const QRectF plot = boundingRect().adjusted(52, 12, -12, -32);
    if (plot.width() <= 0 || plot.height() <= 0)
        return;
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    QFont font = painter->font();
    font.setPixelSize(11);
    painter->setFont(font);
    const qreal magnitude = qMax<qreal>(1, qMax(qAbs(minimum_), qAbs(maximum_)));
    const qreal minimum   = minimum_ / magnitude;
    const qreal maximum   = maximum_ / magnitude;
    const qreal range     = maximum - minimum;
    for (int tick = 0; tick <= 4; ++tick)
    {
        const qreal y = plot.bottom() - plot.height() * tick / 4;
        painter->setPen(gridColor_);
        painter->drawLine(QPointF(plot.left(), y), QPointF(plot.right(), y));
        painter->setPen(textColor_);
        const QString label = QLocale().toString((minimum + range * tick / 4) * magnitude, 'g', 4);
        painter->drawText(QRectF(0, y - 8, plot.left() - 8, 16), Qt::AlignRight | Qt::AlignVCenter, label);
    }
    if (values_.isEmpty())
    {
        painter->restore();
        return;
    }

    const qreal spacing  = plot.width() / values_.size();
    const qreal baseline = plot.bottom() + plot.height() * minimum / range;
    auto pointAt         = [&](int index)
    {
        return QPointF(plot.left() + spacing * (index + 0.5),
                       plot.bottom() - plot.height() * (values_[index].y() / magnitude - minimum) / range);
    };
    const int stride = qMax(1, qCeil(values_.size() / qMax<qreal>(1, plot.width())));
    QPainterPath path;
    for (int index = 0; index < values_.size(); index += stride)
    {
        const int end = qMin(values_.size(), index + stride);
        int lowest    = index;
        int highest   = index;
        for (int candidate = index + 1; candidate < end; ++candidate)
        {
            if (values_[candidate].y() < values_[lowest].y())
                lowest = candidate;
            if (values_[candidate].y() > values_[highest].y())
                highest = candidate;
        }
        const QPointF point = pointAt(highest);
        if (type_ == Bar)
        {
            const qreal barWidth = qMax<qreal>(1, spacing * stride * 0.6);
            const qreal top      = qMin(pointAt(highest).y(), baseline);
            const qreal bottom   = qMax(pointAt(lowest).y(), baseline);
            painter->fillRect(QRectF(point.x() - barWidth / 2, top, barWidth, bottom - top), color_);
        }
        else
        {
            if (index == 0)
                path.moveTo(pointAt(0));
            const int first  = qMin(lowest, highest);
            const int second = qMax(lowest, highest);
            path.lineTo(pointAt(first));
            if (second != first)
                path.lineTo(pointAt(second));
        }
    }
    if (type_ != Bar)
    {
        path.lineTo(pointAt(values_.size() - 1));
        if (type_ == Area)
        {
            QPainterPath area(path);
            area.lineTo(pointAt(values_.size() - 1).x(), baseline);
            area.lineTo(pointAt(0).x(), baseline);
            area.closeSubpath();
            QColor fill = color_;
            fill.setAlphaF(fill.alphaF() * 0.16);
            painter->fillPath(area, fill);
        }
        painter->setPen(QPen(color_, 2));
        painter->drawPath(path);
        if (values_.size() == 1)
        {
            painter->setBrush(color_);
            painter->drawEllipse(pointAt(0), 3, 3);
        }
    }
    painter->setPen(textColor_);
    const int labelStride = qMax(1, qCeil(values_.size() / qMax<qreal>(1, plot.width() / 70)));
    for (int index = 0; index < labels_.size(); index += labelStride)
        painter->drawText(QRectF(pointAt(index).x() - 35, plot.bottom() + 8, 70, 20),
                          Qt::AlignHCenter | Qt::AlignTop,
                          labels_[index]);
    painter->restore();
}
