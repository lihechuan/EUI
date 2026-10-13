#pragma once

#include <QColor>
#include <QVariantList>
#include <QQuickPaintedItem>

class EChartRenderer : public QQuickPaintedItem
{
    Q_OBJECT

public:
    enum Type
    {
        Line,
        Bar,
        Area
    };
    Q_ENUM(Type)

    Q_PROPERTY(int type READ type WRITE setType NOTIFY changed)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)
    Q_PROPERTY(QVariantList model READ model WRITE setModel NOTIFY changed)
    Q_PROPERTY(QColor gridColor READ gridColor WRITE setGridColor NOTIFY changed)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY changed)

    explicit EChartRenderer(QQuickItem *parent = nullptr);

    int type() const;
    QColor color() const;
    void setType(int type);
    QColor gridColor() const;
    QColor textColor() const;
    QVariantList model() const;
    void setColor(const QColor &color);
    void paint(QPainter *painter) override;
    void setGridColor(const QColor &color);
    void setTextColor(const QColor &color);
    void setModel(const QVariantList &model);

signals:
    void changed();

private:
    void updatePoints();

    QColor color_;
    int type_ = Line;
    QColor gridColor_;
    QColor textColor_;
    qreal maximum_ = 1;
    qreal minimum_ = 0;
    QVariantList model_;
    QVector<QString> labels_;
    QVector<QPointF> values_;
};
