#pragma once

#include <QFont>
#include <QColor>
#include <QImage>
#include <QPointF>
#include <QQuickPaintedItem>

class EWatermarkRenderer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QPointF gap READ gap WRITE setGap NOTIFY changed)
    Q_PROPERTY(QFont font READ font WRITE setFont NOTIFY changed)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY changed)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)
    Q_PROPERTY(qreal rotate READ rotate WRITE setRotate NOTIFY changed)
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY changed)
    Q_PROPERTY(qreal fontGap READ fontGap WRITE setFontGap NOTIFY changed)
    Q_PROPERTY(QSizeF tileSize READ tileSize WRITE setTileSize NOTIFY changed)
    Q_PROPERTY(QString textAlign READ textAlign WRITE setTextAlign NOTIFY changed)
    Q_PROPERTY(QString textBaseline READ textBaseline WRITE setTextBaseline NOTIFY changed)

public:
    explicit EWatermarkRenderer(QQuickItem *parent = nullptr);

    QFont font() const;
    QPointF gap() const;
    QColor color() const;
    qreal rotate() const;
    QString text() const;
    qreal fontGap() const;
    QPointF offset() const;
    QSizeF tileSize() const;
    QString textAlign() const;
    void setFontGap(qreal gap);
    void setRotate(qreal rotate);
    QString textBaseline() const;
    void setFont(const QFont &font);
    void setGap(const QPointF &gap);
    void setText(const QString &text);
    void setColor(const QColor &color);
    void setTileSize(const QSizeF &size);
    void setOffset(const QPointF &offset);
    void paint(QPainter *painter) override;
    void setTextAlign(const QString &align);
    void setTextBaseline(const QString &baseline);

signals:
    void changed();

private:
    void invalidate();
    void updateTexture();

    QFont font_;
    QPointF gap_;
    QColor color_;
    QString text_;
    QPointF offset_;
    QImage texture_;
    QSizeF tileSize_;
    qreal rotate_         = 0;
    bool dirty_           = true;
    qreal fontGap_        = 0;
    QString textAlign_    = QStringLiteral("center");
    QString textBaseline_ = QStringLiteral("middle");
};
