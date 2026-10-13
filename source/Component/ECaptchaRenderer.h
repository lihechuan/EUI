#pragma once

#include <QColor>
#include <QQuickPaintedItem>

class ECaptchaRenderer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)
    Q_PROPERTY(int length READ length WRITE setLength NOTIFY changed)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY changed)

public:
    explicit ECaptchaRenderer(QQuickItem *parent = nullptr);

    int length() const;
    QColor color() const;
    Q_INVOKABLE void refresh();
    void setLength(int length);
    QColor backgroundColor() const;
    void setColor(const QColor &color);
    void paint(QPainter *painter) override;
    void setBackgroundColor(const QColor &color);
    Q_INVOKABLE bool verify(const QString &text) const;

signals:
    void changed();

private:
    QString code_;
    QColor color_;
    int length_ = 4;
    QColor backgroundColor_;
};
