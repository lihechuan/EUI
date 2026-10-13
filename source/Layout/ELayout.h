#pragma once

#include <QMap>
#include <QList>
#include <QSizeF>
#include <QObject>
#include <QPointF>
#include <QPointer>

#include "Macros/singleton.h"

class ELayout : public QObject
{
    Q_OBJECT
    SINGLETON(ELayout)

private:
    explicit ELayout(QObject *parent = nullptr);

public:
    QPointF calculateBasePos(int placement, QObject *component) const;
    Q_INVOKABLE QPointF calculatePosition(int placement, QObject *component);
    Q_INVOKABLE QList<QObject *> releasePosition(int placement, QObject *component);

    qreal margin = 10;
    QMap<int, QList<QPointer<QObject>>> componentMap;
};
