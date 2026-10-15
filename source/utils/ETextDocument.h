#pragma once

#include <QObject>
#include <QVariant>
#include <QVariantMap>
#include <QQuickTextDocument>

class ETextDocument : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged)

public:
    explicit ETextDocument(QObject *parent = nullptr);

    QQuickTextDocument *document() const;
    void setDocument(QQuickTextDocument *document);
    Q_INVOKABLE QVariantMap formatAt(int position) const;
    Q_INVOKABLE bool apply(int start, int end, const QString &operation, const QVariant &value);

signals:
    void documentChanged();

private:
    QQuickTextDocument *document_ = nullptr;
};
