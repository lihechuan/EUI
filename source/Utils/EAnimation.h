#pragma once

#include <QHash>
#include <QObject>
#include <QStringList>
#include <QVariantMap>

class QQuickItem;

class EAnimation : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QStringList effects READ effects CONSTANT)

public:
    ~EAnimation() override;
    explicit EAnimation(QObject *parent = nullptr);

    QStringList effects() const;
    Q_INVOKABLE bool stop(QQuickItem *target);
    Q_INVOKABLE bool play(QQuickItem *target, const QString &effect, const QVariantMap &options = {});

private:
    class Controller;

    QHash<QQuickItem *, Controller *> m_controllers;
};
