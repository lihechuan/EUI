#pragma once

#include <QUrl>
#include <QObject>
#include <QVariantMap>
#include <QVariantList>

class ETableIO : public QObject
{
    Q_OBJECT

public:
    explicit ETableIO(QObject *parent = nullptr);

    Q_INVOKABLE QString clipboardText() const;
    Q_INVOKABLE bool setClipboardText(const QString &text);
    Q_INVOKABLE QVariantMap readText(const QUrl &url) const;
    Q_INVOKABLE QVariantMap readWorkbook(const QUrl &url) const;
    Q_INVOKABLE QVariantMap loadLayout(const QString &key) const;
    Q_INVOKABLE bool saveLayout(const QString &key, const QVariantMap &layout);
    Q_INVOKABLE QVariantMap writeText(const QUrl &url, const QString &text) const;
    Q_INVOKABLE QVariantMap writeWorkbook(const QUrl &url, const QVariantList &matrix) const;
    Q_INVOKABLE QVariantMap printHtml(const QString &html, const QUrl &url, const QString &printerName = {}) const;
};
