#pragma once

#include <QColor>
#include <QImage>
#include <QTimer>
#include <QPointer>
#if QT_CONFIG(thread)
#include <QFutureWatcher>
#endif
#include <QSharedPointer>
#include <QQuickPaintedItem>

class EBackdropRenderer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(bool live READ live WRITE setLive NOTIFY changed)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY changed)
    Q_PROPERTY(int blurRadius READ blurRadius WRITE setBlurRadius NOTIFY changed)
    Q_PROPERTY(QColor tintColor READ tintColor WRITE setTintColor NOTIFY changed)
    Q_PROPERTY(qreal tintOpacity READ tintOpacity WRITE setTintOpacity NOTIFY changed)
    Q_PROPERTY(QQuickItem *sourceItem READ sourceItem WRITE setSourceItem NOTIFY changed)
    Q_PROPERTY(qreal noiseOpacity READ noiseOpacity WRITE setNoiseOpacity NOTIFY changed)

public:
    ~EBackdropRenderer() override;
    explicit EBackdropRenderer(QQuickItem *parent = nullptr);

    bool live() const;
    bool ready() const;
    qreal radius() const;
    int blurRadius() const;
    void setLive(bool live);
    QColor tintColor() const;
    qreal tintOpacity() const;
    qreal noiseOpacity() const;
    Q_INVOKABLE void refresh();
    void setRadius(qreal radius);
    void setBlurRadius(int radius);
    QQuickItem *sourceItem() const;
    void setTintOpacity(qreal opacity);
    void setNoiseOpacity(qreal opacity);
    void paint(QPainter *painter) override;
    void setSourceItem(QQuickItem *source);
    void setTintColor(const QColor &color);

signals:
    void changed();
    void readyChanged();

private:
    void capture();
    void updateTimer();
    void updateWindow();

    QImage image_;
    QImage noise_;
    QTimer timer_;
    QRectF region_;
    qreal radius_ = 8;
    int revision_ = 0;
    QColor tintColor_;
    bool dirty_          = true;
    bool live_           = false;
    int blurRadius_      = 32;
    bool pending_        = false;
    qreal tintOpacity_   = 0.7;
    int workingRevision_ = 0;
    qreal noiseOpacity_  = 0.025;
    QPointer<QQuickItem> source_;
#if QT_CONFIG(thread)
    QFutureWatcher<QImage> worker_;
#endif
    QMetaObject::Connection frameConnection_;
    QSharedPointer<QQuickItemGrabResult> grab_;
    QMetaObject::Connection visibilityConnection_;
    QList<QMetaObject::Connection> sourceConnections_;
};
