#include <QPainter>
#include <QRandomGenerator>

#include "ECaptchaRenderer.h"

namespace
{
const QString Alphabet = QStringLiteral("23456789ABCDEFGHJKLMNPQRSTUVWXYZ");
}

ECaptchaRenderer::ECaptchaRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
    setImplicitSize(160, 48);
    refresh();
}

int ECaptchaRenderer::length() const
{
    return length_;
}
QColor ECaptchaRenderer::color() const
{
    return color_;
}
QColor ECaptchaRenderer::backgroundColor() const
{
    return backgroundColor_;
}

void ECaptchaRenderer::setLength(int length)
{
    length = qBound(3, length, 8);
    if (length_ == length)
        return;
    length_ = length;
    refresh();
}

void ECaptchaRenderer::setColor(const QColor &color)
{
    if (color_ == color)
        return;
    color_ = color;
    update();
    emit changed();
}

void ECaptchaRenderer::setBackgroundColor(const QColor &color)
{
    if (backgroundColor_ == color)
        return;
    backgroundColor_ = color;
    update();
    emit changed();
}

void ECaptchaRenderer::refresh()
{
    code_.clear();
    for (int index = 0; index < length_; ++index)
        code_.append(Alphabet[QRandomGenerator::global()->bounded(Alphabet.size())]);
    update();
    emit changed();
}

bool ECaptchaRenderer::verify(const QString &text) const
{
    return text.compare(code_, Qt::CaseInsensitive) == 0;
}

void ECaptchaRenderer::paint(QPainter *painter)
{
    painter->fillRect(boundingRect(), backgroundColor_);
    const qreal cell = width() / length_;
    QFont font       = painter->font();
    font.setBold(true);
    font.setPixelSize(qMax(1, qRound(qMin(cell, height()) * 0.6)));
    painter->setFont(font);
    for (int index = 0; index < length_; ++index)
    {
        painter->save();
        painter->setPen(color_);
        painter->translate(cell * (index + 0.5), height() / 2);
        painter->rotate((code_[index].unicode() % 17) - 8);
        painter->drawText(QRectF(-cell / 2, -height() / 2, cell, height()), Qt::AlignCenter, code_.mid(index, 1));
        painter->restore();
    }
    painter->save();
    painter->setPen(color_);
    painter->setOpacity(0.2);
    for (int index = 0; index < 6; ++index)
    {
        const qreal y = height() * (index + 1) / 7;
        painter->drawLine(QPointF(0, y), QPointF(width(), height() - y));
    }
    painter->restore();
}
