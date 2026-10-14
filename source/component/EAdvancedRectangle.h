#pragma once

#include <QList>
#include <QColor>
#include <QVector>
#include <QPainter>
#include <QQuickItem>
#include <QQuickPaintedItem>

#include "macros/stdafx.h"

class EAdvancedRectangle : public QQuickPaintedItem
{
    Q_OBJECT
    QML_NAMED_ELEMENT(EAdvancedRectangle)

    Q_PROPERTY_AUTO(QColor, color)
    Q_PROPERTY_AUTO(qreal, borderWidth)
    Q_PROPERTY_AUTO(QList<int>, radius)
    Q_PROPERTY_AUTO(QColor, borderColor)
    Q_PROPERTY_AUTO(Qt::PenStyle, borderStyle)
    Q_PROPERTY_AUTO(QVector<qreal>, dashPattern)

public:
    explicit EAdvancedRectangle(QQuickItem *parent = nullptr);

    bool hasBorder() const;
    void paint(QPainter *painter) override;
};
