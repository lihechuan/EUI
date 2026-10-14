#pragma once

#include <QUrl>
#include <QObject>
#include <QVariantMap>
#include <QVariantList>

class EFilePickerUtils : public QObject
{
    Q_OBJECT

public:
    explicit EFilePickerUtils(QObject *parent = nullptr);

    Q_INVOKABLE QString filePath(const QUrl &url) const;
    Q_INVOKABLE QVariantMap selection(const QVariantList &urls, const QString &mode, bool multiple) const;
};
