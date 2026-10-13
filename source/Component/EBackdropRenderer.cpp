#include <QPainter>
#include <QPainterPath>
#include <QQuickWindow>
#include <QQuickItemGrabResult>
#include <QtConcurrent/QtConcurrentRun>

#include "EBackdropRenderer.h"
#include "Utils/EImageEffects.h"

namespace
{
constexpr int CaptureLimit  = 512;
constexpr int FrameInterval = 33;
}

EBackdropRenderer::EBackdropRenderer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAntialiasing(true);
    timer_.setInterval(FrameInterval);
    noise_       = QImage(64, 64, QImage::Format_ARGB32_Premultiplied);
    quint32 seed = 1;
    for (int y = 0; y < noise_.height(); ++y)
    {
        auto row = reinterpret_cast<QRgb *>(noise_.scanLine(y));
        for (int x = 0; x < noise_.width(); ++x)
        {
            seed            = seed * 1664525 + 1013904223;
            const int value = static_cast<int>(seed >> 24);
            row[x]          = qRgb(value, value, value);
        }
    }

    connect(&timer_, &QTimer::timeout, this, &EBackdropRenderer::capture);
    connect(this, &QQuickItem::xChanged, this, &EBackdropRenderer::refresh);
    connect(this, &QQuickItem::yChanged, this, &EBackdropRenderer::refresh);
    connect(this, &QQuickItem::widthChanged, this, &EBackdropRenderer::refresh);
    connect(this, &QQuickItem::heightChanged, this, &EBackdropRenderer::refresh);
    connect(this, &QQuickItem::visibleChanged, this, &EBackdropRenderer::updateTimer);
    connect(this, &QQuickItem::windowChanged, this, &EBackdropRenderer::updateWindow);
    connect(&worker_,
            &QFutureWatcher<QImage>::finished,
            this,
            [this]()
            {
                if (workingRevision_ == revision_ && source_)
                {
                    image_ = worker_.result();
                    update();
                    emit readyChanged();
                }
                pending_ = false;
                if (dirty_)
                    QTimer::singleShot(0, this, &EBackdropRenderer::capture);
            });
    updateWindow();
}

bool EBackdropRenderer::live() const
{
    return live_;
}
bool EBackdropRenderer::ready() const
{
    return !image_.isNull();
}
qreal EBackdropRenderer::radius() const
{
    return radius_;
}
int EBackdropRenderer::blurRadius() const
{
    return blurRadius_;
}
QColor EBackdropRenderer::tintColor() const
{
    return tintColor_;
}
qreal EBackdropRenderer::tintOpacity() const
{
    return tintOpacity_;
}
qreal EBackdropRenderer::noiseOpacity() const
{
    return noiseOpacity_;
}
QQuickItem *EBackdropRenderer::sourceItem() const
{
    return source_.data();
}

EBackdropRenderer::~EBackdropRenderer()
{
    disconnect(this, nullptr, this, nullptr);
    disconnect(&worker_, nullptr, this, nullptr);
    disconnect(frameConnection_);
    disconnect(visibilityConnection_);
    for (const auto &connection : sourceConnections_)
        disconnect(connection);
    if (grab_)
        disconnect(grab_.data(), nullptr, this, nullptr);
}

void EBackdropRenderer::setLive(bool live)
{
    if (live_ == live)
        return;
    live_ = live;
    updateTimer();
    emit changed();
}

void EBackdropRenderer::setRadius(qreal radius)
{
    radius = qIsFinite(radius) ? qMax<qreal>(0, radius) : 0;
    if (qFuzzyCompare(radius_, radius))
        return;
    radius_ = radius;
    update();
    emit changed();
}

void EBackdropRenderer::setBlurRadius(int radius)
{
    radius = qBound(0, radius, 128);
    if (blurRadius_ == radius)
        return;
    blurRadius_ = radius;
    refresh();
    emit changed();
}

void EBackdropRenderer::setTintColor(const QColor &color)
{
    if (tintColor_ == color)
        return;
    tintColor_ = color;
    update();
    emit changed();
}

void EBackdropRenderer::setTintOpacity(qreal opacity)
{
    opacity = qIsFinite(opacity) ? qBound<qreal>(0, opacity, 1) : 0;
    if (qFuzzyCompare(tintOpacity_, opacity))
        return;
    tintOpacity_ = opacity;
    update();
    emit changed();
}

void EBackdropRenderer::setNoiseOpacity(qreal opacity)
{
    opacity = qIsFinite(opacity) ? qBound<qreal>(0, opacity, 1) : 0;
    if (qFuzzyCompare(noiseOpacity_, opacity))
        return;
    noiseOpacity_ = opacity;
    update();
    emit changed();
}

void EBackdropRenderer::setSourceItem(QQuickItem *source)
{
    for (QQuickItem *ancestor = this; ancestor; ancestor = ancestor->parentItem())
    {
        if (ancestor == source)
            source = nullptr;
    }
    if (source_ == source)
        return;

    for (const auto &connection : sourceConnections_)
        disconnect(connection);
    sourceConnections_.clear();
    if (grab_)
    {
        disconnect(grab_.data(), nullptr, this, nullptr);
        grab_.clear();
        pending_ = worker_.isRunning();
    }
    source_ = source;
    image_  = {};
    if (source)
    {
        sourceConnections_.append(connect(source, &QQuickItem::widthChanged, this, &EBackdropRenderer::refresh));
        sourceConnections_.append(connect(source, &QQuickItem::heightChanged, this, &EBackdropRenderer::refresh));
        sourceConnections_.append(connect(source, &QQuickItem::windowChanged, this, &EBackdropRenderer::updateTimer));
        sourceConnections_.append(connect(source,
                                          &QObject::destroyed,
                                          this,
                                          [this]()
                                          {
                                              if (grab_)
                                              {
                                                  disconnect(grab_.data(), nullptr, this, nullptr);
                                                  grab_.clear();
                                                  pending_ = worker_.isRunning();
                                              }
                                              ++revision_;
                                              image_ = {};
                                              updateTimer();
                                              update();
                                              emit readyChanged();
                                              emit changed();
                                          }));
    }
    refresh();
    updateTimer();
    update();
    emit readyChanged();
    emit changed();
}

void EBackdropRenderer::refresh()
{
    dirty_ = true;
    ++revision_;
    QTimer::singleShot(0, this, &EBackdropRenderer::capture);
}

void EBackdropRenderer::updateTimer()
{
    const bool active = live_ && isVisible() && window() && window()->isVisible() && source_;
    if (active)
        timer_.start();
    else
        timer_.stop();
    if (isVisible())
        refresh();
}

void EBackdropRenderer::updateWindow()
{
    disconnect(frameConnection_);
    disconnect(visibilityConnection_);
    if (window())
    {
        visibilityConnection_ = connect(window(), &QWindow::visibleChanged, this, &EBackdropRenderer::updateTimer);
        frameConnection_      = connect(window(),
                                   &QQuickWindow::afterAnimating,
                                   this,
                                   [this]()
                                   {
                                       if (source_ && isVisible() && mapRectToItem(source_, boundingRect()) != region_)
                                           refresh();
                                   });
    }
    updateTimer();
}

void EBackdropRenderer::capture()
{
    if (pending_ || !source_ || !isVisible() || !window() || !window()->isVisible() || !source_->window() ||
        !source_->window()->isVisible() || width() <= 0 || height() <= 0 || source_->width() <= 0 ||
        source_->height() <= 0)
        return;

    for (QQuickItem *ancestor = this; ancestor; ancestor = ancestor->parentItem())
    {
        if (ancestor == source_)
        {
            setSourceItem(nullptr);
            return;
        }
    }
    const QSizeF sourceSize(source_->width(), source_->height());
    const qreal scale = qMin<qreal>(1, CaptureLimit / qMax(sourceSize.width(), sourceSize.height()));
    const QSize size(qMax(1, qRound(sourceSize.width() * scale)), qMax(1, qRound(sourceSize.height() * scale)));
    grab_ = source_->grabToImage(size);
    if (!grab_)
        return;

    pending_            = true;
    dirty_              = false;
    const int revision  = revision_;
    const QRectF region = mapRectToItem(source_, boundingRect());
    region_             = region;
    const int radius    = qRound(blurRadius_ * scale);
    connect(grab_.data(),
            &QQuickItemGrabResult::ready,
            this,
            [this, region, scale, radius, revision]()
            {
                const auto grab = grab_;
                disconnect(grab.data(), nullptr, this, nullptr);
                grab_.clear();
                if (revision != revision_ || !source_)
                {
                    pending_ = false;
                    QTimer::singleShot(0, this, &EBackdropRenderer::capture);
                    return;
                }
                const qreal outputScale = qMin<qreal>(scale, CaptureLimit / qMax(region.width(), region.height()));
                QImage image(qMax(1, qRound(region.width() * outputScale)),
                             qMax(1, qRound(region.height() * outputScale)),
                             QImage::Format_ARGB32_Premultiplied);
                image.fill(Qt::transparent);
                QPainter painter(&image);
                const QRectF crop(
                    region.x() * scale, region.y() * scale, region.width() * scale, region.height() * scale);
                painter.drawImage(QRectF(image.rect()), grab->image(), crop);
                painter.end();
                if (radius == 0)
                {
                    image_   = image;
                    pending_ = false;
                    update();
                    emit readyChanged();
                    if (dirty_)
                        QTimer::singleShot(0, this, &EBackdropRenderer::capture);
                    return;
                }
                workingRevision_ = revision;
                worker_.setFuture(QtConcurrent::run([image, radius]() { return EImageEffects::blur(image, radius); }));
            });
}

void EBackdropRenderer::paint(QPainter *painter)
{
    if (boundingRect().isEmpty())
        return;
    painter->save();
    QPainterPath clip;
    clip.addRoundedRect(boundingRect(), radius_, radius_);
    painter->setClipPath(clip);
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    if (!image_.isNull())
        painter->drawImage(boundingRect(), image_);
    painter->setOpacity(tintOpacity_);
    painter->fillRect(boundingRect(), tintColor_);
    painter->setOpacity(noiseOpacity_);
    painter->fillRect(boundingRect(), QBrush(noise_));
    painter->restore();
}
