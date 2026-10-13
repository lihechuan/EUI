#include <QBrush>
#include <QtMath>
#include <QPainter>
#include <QTransform>
#include <QFontMetricsF>

#include "Component/EWatermarkRenderer.h"

EWatermarkRenderer::EWatermarkRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
}

QFont EWatermarkRenderer::font() const
{
    return font_;
}

QPointF EWatermarkRenderer::gap() const
{
    return gap_;
}

QColor EWatermarkRenderer::color() const
{
    return color_;
}

QString EWatermarkRenderer::text() const
{
    return text_;
}

qreal EWatermarkRenderer::rotate() const
{
    return rotate_;
}

QPointF EWatermarkRenderer::offset() const
{
    return offset_;
}

QSizeF EWatermarkRenderer::tileSize() const
{
    return tileSize_;
}

qreal EWatermarkRenderer::fontGap() const
{
    return fontGap_;
}

QString EWatermarkRenderer::textAlign() const
{
    return textAlign_;
}

QString EWatermarkRenderer::textBaseline() const
{
    return textBaseline_;
}

void EWatermarkRenderer::setFontGap(qreal gap)
{
    if (qIsFinite(gap) && gap >= 0 && fontGap_ != gap)
    {
        fontGap_ = gap;
        invalidate();
    }
}

void EWatermarkRenderer::setTextAlign(const QString &align)
{
    if ((align == QStringLiteral("left") || align == QStringLiteral("center") || align == QStringLiteral("right")) &&
        textAlign_ != align)
    {
        textAlign_ = align;
        invalidate();
    }
}

void EWatermarkRenderer::setTextBaseline(const QString &baseline)
{
    if ((baseline == QStringLiteral("top") || baseline == QStringLiteral("hanging") ||
         baseline == QStringLiteral("middle") || baseline == QStringLiteral("alphabetic") ||
         baseline == QStringLiteral("ideographic") || baseline == QStringLiteral("bottom")) &&
        textBaseline_ != baseline)
    {
        textBaseline_ = baseline;
        invalidate();
    }
}

void EWatermarkRenderer::invalidate()
{
    dirty_ = true;
    update();
    emit changed();
}

void EWatermarkRenderer::setFont(const QFont &font)
{
    if (font_ != font)
    {
        font_ = font;
        invalidate();
    }
}

void EWatermarkRenderer::setGap(const QPointF &gap)
{
    if (qIsFinite(gap.x()) && qIsFinite(gap.y()) && gap_ != gap)
    {
        gap_ = gap;
        invalidate();
    }
}

void EWatermarkRenderer::setColor(const QColor &color)
{
    if (color_ != color)
    {
        color_ = color;
        invalidate();
    }
}

void EWatermarkRenderer::setText(const QString &text)
{
    if (text_ != text)
    {
        text_ = text;
        invalidate();
    }
}

void EWatermarkRenderer::setRotate(qreal rotate)
{
    if (qIsFinite(rotate) && rotate_ != rotate)
    {
        rotate_ = rotate;
        invalidate();
    }
}

void EWatermarkRenderer::setOffset(const QPointF &offset)
{
    if (qIsFinite(offset.x()) && qIsFinite(offset.y()) && offset_ != offset)
    {
        offset_ = offset;
        update();
        emit changed();
    }
}

void EWatermarkRenderer::setTileSize(const QSizeF &size)
{
    if (qIsFinite(size.width()) && qIsFinite(size.height()) && tileSize_ != size)
    {
        tileSize_ = size;
        invalidate();
    }
}

void EWatermarkRenderer::updateTexture()
{
    dirty_                 = false;
    const qreal stepWidth  = tileSize_.width() + gap_.x();
    const qreal stepHeight = tileSize_.height() + gap_.y();
    if (text_.isEmpty() || !qIsFinite(stepWidth) || !qIsFinite(stepHeight) || stepWidth <= 0 || stepHeight <= 0)
    {
        texture_ = QImage();
        return;
    }
    // Bounding the cached texture also covers extreme user-supplied tile and gap dimensions.
    const qreal scale = qMin(qreal(1), qreal(2048) / qMax(stepWidth, stepHeight));
    texture_          = QImage(
        qMax(1, qCeil(stepWidth * scale)), qMax(1, qCeil(stepHeight * scale)), QImage::Format_ARGB32_Premultiplied);
    texture_.fill(Qt::transparent);
    QPainter painter(&texture_);
    painter.setFont(font_);
    painter.setPen(color_);
    painter.setRenderHint(QPainter::TextAntialiasing);
    painter.scale(texture_.width() / stepWidth, texture_.height() / stepHeight);
    for (int row = -1; row <= 1; ++row)
    {
        for (int column = -1; column <= 1; ++column)
        {
            painter.save();
            painter.translate(column * stepWidth + tileSize_.width() / 2, row * stepHeight + tileSize_.height() / 2);
            painter.rotate(rotate_);
            if (fontGap_ == 0 && textAlign_ == QStringLiteral("center") && textBaseline_ == QStringLiteral("middle"))
                painter.drawText(
                    QRectF(-tileSize_.width() / 2, -tileSize_.height() / 2, tileSize_.width(), tileSize_.height()),
                    Qt::AlignCenter | Qt::TextWordWrap,
                    text_);
            else
            {
                const QFontMetricsF metrics(font_);
                const QStringList lines = text_.split(QLatin1Char('\n'));
                const qreal blockHeight = lines.size() * metrics.height() + qMax(0, lines.size() - 1) * fontGap_;
                qreal baseline          = -blockHeight / 2 + metrics.ascent();
                if (textBaseline_ == QStringLiteral("top"))
                    baseline = -tileSize_.height() / 2 + metrics.ascent();
                else if (textBaseline_ == QStringLiteral("hanging"))
                    baseline = -tileSize_.height() / 2 + metrics.ascent() * 0.8;
                else if (textBaseline_ == QStringLiteral("alphabetic"))
                    baseline = 0;
                else if (textBaseline_ == QStringLiteral("ideographic"))
                    baseline = -metrics.descent();
                else if (textBaseline_ == QStringLiteral("bottom"))
                    baseline = tileSize_.height() / 2 - blockHeight + metrics.ascent();
                for (const QString &line : lines)
                {
                    const qreal lineWidth = metrics.horizontalAdvance(line);
                    const qreal x         = textAlign_ == QStringLiteral("left")    ? -tileSize_.width() / 2
                                            : textAlign_ == QStringLiteral("right") ? tileSize_.width() / 2 - lineWidth
                                                                                    : -lineWidth / 2;
                    painter.drawText(QPointF(x, baseline), line);
                    baseline += metrics.height() + fontGap_;
                }
            }
            painter.restore();
        }
    }
}

void EWatermarkRenderer::paint(QPainter *painter)
{
    if (dirty_)
    {
        updateTexture();
    }
    if (texture_.isNull())
    {
        return;
    }
    QBrush pattern(texture_);
    QTransform transform;
    transform.translate(offset_.x(), offset_.y());
    transform.scale((tileSize_.width() + gap_.x()) / texture_.width(),
                    (tileSize_.height() + gap_.y()) / texture_.height());
    pattern.setTransform(transform);
    painter->fillRect(boundingRect(), pattern);
}
