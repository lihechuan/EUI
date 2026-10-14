#include <QPainter>
#include <stdexcept>

#include "EQrCodeRenderer.h"
#include "thirdparty/qrcodegen/qrcodegen.hpp"

EQrCodeRenderer::EQrCodeRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(false);
}

bool EQrCodeRenderer::valid() const
{
    return modules_ > 0;
}
QString EQrCodeRenderer::text() const
{
    return text_;
}
int EQrCodeRenderer::modules() const
{
    return modules_;
}
QColor EQrCodeRenderer::color() const
{
    return color_;
}
int EQrCodeRenderer::quietZone() const
{
    return quietZone_;
}
int EQrCodeRenderer::errorCorrection() const
{
    return correction_;
}
QColor EQrCodeRenderer::backgroundColor() const
{
    return backgroundColor_;
}

void EQrCodeRenderer::setText(const QString &text)
{
    if (text_ == text)
        return;
    text_ = text;
    encode();
}

void EQrCodeRenderer::setColor(const QColor &color)
{
    if (color_ == color)
        return;
    color_ = color;
    update();
    emit changed();
}

void EQrCodeRenderer::setBackgroundColor(const QColor &color)
{
    if (backgroundColor_ == color)
        return;
    backgroundColor_ = color;
    update();
    emit changed();
}

void EQrCodeRenderer::setQuietZone(int quietZone)
{
    quietZone = qBound(4, quietZone, 32);
    if (quietZone_ == quietZone)
        return;
    quietZone_ = quietZone;
    update();
    emit changed();
}

void EQrCodeRenderer::setErrorCorrection(int correction)
{
    correction = qBound(0, correction, 3);
    if (correction_ == correction)
        return;
    correction_ = correction;
    encode();
}

bool EQrCodeRenderer::moduleAt(int x, int y) const
{
    return x >= 0 && y >= 0 && x < modules_ && y < modules_ && matrix_.testBit(y * modules_ + x);
}

void EQrCodeRenderer::encode()
{
    modules_ = 0;
    matrix_.clear();
    const QByteArray bytes = text_.toUtf8();
    if (!bytes.isEmpty() && bytes.size() <= 2953)
    {
        try
        {
            const std::vector<std::uint8_t> data(bytes.begin(), bytes.end());
            const auto qr = qrcodegen::QrCode::encodeBinary(data, static_cast<qrcodegen::QrCode::Ecc>(correction_));
            modules_      = qr.getSize();
            matrix_.resize(modules_ * modules_);
            for (int y = 0; y < modules_; ++y)
                for (int x = 0; x < modules_; ++x)
                    matrix_.setBit(y * modules_ + x, qr.getModule(x, y));
        }
        catch (const std::length_error &)
        {
            modules_ = 0;
        }
    }
    update();
    emit changed();
}

void EQrCodeRenderer::paint(QPainter *painter)
{
    painter->fillRect(boundingRect(), backgroundColor_);
    if (!valid())
        return;
    const int count  = modules_ + quietZone_ * 2;
    const qreal cell = qMin(width(), height()) / count;
    const QPointF origin((width() - cell * count) / 2, (height() - cell * count) / 2);
    for (int y = 0; y < modules_; ++y)
    {
        for (int x = 0; x < modules_; ++x)
        {
            if (moduleAt(x, y))
                painter->fillRect(
                    QRectF(origin.x() + (x + quietZone_) * cell, origin.y() + (y + quietZone_) * cell, cell, cell),
                    color_);
        }
    }
}
