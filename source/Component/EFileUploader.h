#pragma once

#include <QUrl>
#include <QObject>
#include <QPointer>
#include <QVariantMap>

class QNetworkReply;
class QNetworkAccessManager;

class EFileUploader : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY languageChanged)

public:
    explicit EFileUploader(QObject *parent = nullptr);

    bool busy() const;
    qreal progress() const;
    QString language() const;
    Q_INVOKABLE void cancel();
    void setLanguage(const QString &language);
    Q_INVOKABLE QVariantMap fileInfo(const QUrl &url) const;
    Q_INVOKABLE QVariantList directoryFiles(const QUrl &url) const;
    Q_INVOKABLE bool submit(const QUrl &file,
                            const QUrl &endpoint,
                            const QString &fieldName   = QStringLiteral("file"),
                            const QVariantMap &headers = {},
                            const QVariantMap &data    = {},
                            const QString &method      = QStringLiteral("POST"),
                            bool withCredentials       = false);

signals:
    void busyChanged();
    void languageChanged();
    void progressChanged();
    void started(const QUrl &file);
    void finished(const QUrl &file, bool success, int status, const QString &response, const QString &error);

private:
    QString errorText(const QString &english, const QString &chinese) const;

    QUrl currentFile_;
    qreal progress_ = 0;
    QPointer<QNetworkReply> reply_;
    QNetworkAccessManager *network_;
    QString language_ = QStringLiteral("en");
};
