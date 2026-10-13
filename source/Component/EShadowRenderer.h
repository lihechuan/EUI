#pragma once

#include <QColor>
#include <QQuickPaintedItem>

class EShadowRenderer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY changed)
    Q_PROPERTY(int elevation READ elevation WRITE setElevation NOTIFY changed)

public:
    explicit EShadowRenderer(QQuickItem *parent = nullptr);

    QColor color() const;
    qreal radius() const;
    int elevation() const;
    void setRadius(qreal radius);
    void setElevation(int elevation);
    void setColor(const QColor &color);
    void paint(QPainter *painter) override;

signals:
    void changed();

private:
    QColor color_;
    qreal radius_  = 4;
    int elevation_ = 8;
};
