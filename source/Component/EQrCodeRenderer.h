#pragma once

#include <QColor>
#include <QBitArray>
#include <QQuickPaintedItem>

class EQrCodeRenderer : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(bool valid READ valid NOTIFY changed)
    Q_PROPERTY(int modules READ modules NOTIFY changed)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY changed)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY changed)
    Q_PROPERTY(int quietZone READ quietZone WRITE setQuietZone NOTIFY changed)
    Q_PROPERTY(int errorCorrection READ errorCorrection WRITE setErrorCorrection NOTIFY changed)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY changed)

public:
    explicit EQrCodeRenderer(QQuickItem *parent = nullptr);

    bool valid() const;
    int modules() const;
    QColor color() const;
    QString text() const;
    int quietZone() const;
    int errorCorrection() const;
    QColor backgroundColor() const;
    void setQuietZone(int quietZone);
    void setText(const QString &text);
    void setColor(const QColor &color);
    void paint(QPainter *painter) override;
    void setErrorCorrection(int correction);
    void setBackgroundColor(const QColor &color);
    Q_INVOKABLE bool moduleAt(int x, int y) const;

signals:
    void changed();

private:
    void encode();

    QColor color_;
    QString text_;
    int modules_ = 0;
    QBitArray matrix_;
    int quietZone_  = 4;
    int correction_ = 1;
    QColor backgroundColor_;
};
