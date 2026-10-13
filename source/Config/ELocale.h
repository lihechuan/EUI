#pragma once

#include <QObject>
#include <QString>
#include <QVariantMap>

#include "Macros/singleton.h"

class ELocale : public QObject
{
    Q_OBJECT
    SINGLETON(ELocale)

    Q_PROPERTY(QVariantMap texts READ texts NOTIFY languageChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

private:
    explicit ELocale(QObject *parent = nullptr);

public:
    QString language() const;
    QVariantMap texts() const;
    void setLanguage(const QString &language);
    Q_INVOKABLE QVariantMap textsForLanguage(const QString &language) const;

signals:
    void languageChanged();

private:
    QString language_;
    QVariantMap texts_;
};
