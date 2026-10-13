#pragma once

#include <QObject>
#include <QPointer>
#include <QVariantMap>
#include <QVariantList>

class ExampleData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList icons READ icons CONSTANT)
    Q_PROPERTY(QVariantMap ui READ ui NOTIFY samplesChanged)
    Q_PROPERTY(QVariantMap models READ models NOTIFY samplesChanged)
    Q_PROPERTY(QVariantMap samples READ samples NOTIFY samplesChanged)

public:
    explicit ExampleData(QObject *parent = nullptr);

    QVariantMap ui() const;
    QVariantList icons() const;
    QVariantMap models() const;
    QVariantMap samples() const;
    void setLocale(QObject *locale);
    Q_INVOKABLE QString codeFor(const QString &componentName) const;
    Q_INVOKABLE QString highlightCode(const QString &code, const QVariantMap &colors) const;

signals:
    void samplesChanged();

private:
    Q_SLOT void rebuild();

    QVariantMap ui_;
    QVariantMap models_;
    QVariantMap samples_;
    QPointer<QObject> locale_;
};
