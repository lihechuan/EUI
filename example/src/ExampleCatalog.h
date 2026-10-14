#pragma once

#include <QObject>
#include <QString>
#include <QPointer>
#include <QVariant>

class ExampleCatalog : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariantList groups READ groups NOTIFY groupsChanged)
    Q_PROPERTY(bool quick3DAvailable READ quick3DAvailable CONSTANT)

public:
    explicit ExampleCatalog(QObject *parent = nullptr);

    QVariantList groups() const;
    bool quick3DAvailable() const;
    void setLocale(QObject *locale);
    Q_INVOKABLE QVariant find(const QString &index) const;
    Q_INVOKABLE QVariantList filtered(const QString &query) const;
    Q_INVOKABLE QVariantList breadcrumb(const QString &index) const;
    Q_INVOKABLE QString quick3DSource(const QString &fileName) const;
    Q_INVOKABLE QVariantMap documentation(const QString &index) const;

signals:
    void groupsChanged();

private:
    Q_SLOT void rebuild();

    QVariantList m_groups;
    QPointer<QObject> m_locale;
    QVariantMap m_documentation;
};
