#pragma once

#include <QObject>
#include <QQuickItem>

class EWindowActions : public QObject
{
    Q_OBJECT

public:
    explicit EWindowActions(QObject *parent = nullptr);

    Q_INVOKABLE bool move(QQuickItem *item);
    Q_INVOKABLE void close(QQuickItem *item);
    Q_INVOKABLE void minimize(QQuickItem *item);
    Q_INVOKABLE void toggleMaximized(QQuickItem *item);
    Q_INVOKABLE void toggleFullScreen(QQuickItem *item);
    Q_INVOKABLE bool resize(QQuickItem *item, int edges);
};
