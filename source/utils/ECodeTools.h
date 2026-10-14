#pragma once

#include <QObject>
#include <QVariantMap>

class ECodeTools : public QObject
{
    Q_OBJECT

public:
    explicit ECodeTools(QObject *parent = nullptr);

    Q_INVOKABLE bool copyText(const QString &text) const;
    static QString highlightCode(const QString &code, const QVariantMap &colors);
    Q_INVOKABLE QString highlight(const QString &code, const QVariantMap &colors) const;
};
