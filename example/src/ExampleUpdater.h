#pragma once

#include <QUrl>
#include <QFile>
#include <QTimer>
#include <QObject>
#include <QPointer>
#include <QCryptographicHash>
#include <QNetworkAccessManager>

class QProcess;
class QLockFile;
class QNetworkReply;
class QTemporaryDir;

class ExampleUpdater : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY changed)
    Q_PROPERTY(QString error READ error NOTIFY changed)
    Q_PROPERTY(QString stage READ stage NOTIFY changed)
    Q_PROPERTY(qreal progress READ progress NOTIFY changed)
    Q_PROPERTY(QString currentVersion READ currentVersion CONSTANT)
    Q_PROPERTY(QString latestVersion READ latestVersion NOTIFY changed)

public:
    ~ExampleUpdater() override;
    explicit ExampleUpdater(QObject *parent = nullptr, const QString &directory = QString());

    bool busy() const;
    QString error() const;
    void reportRollback();
    QString stage() const;
    qreal progress() const;
    Q_INVOKABLE void check();
    QString latestVersion() const;
    QString currentVersion() const;
    void confirmReady(const QString &result);
    Q_INVOKABLE void install(const QString &language = QString(), bool dark = false);

signals:
    void changed();
    void restartReady();

private:
    friend class UpdateTests;

    void prepare();
    QString assetName() const;
    void fail(const QString &error);
    bool trustedUrl(const QUrl &url) const;
    void request(const QUrl &url, bool download = false);
    void setStage(const QString &stage, qreal progress = 0);

    bool m_dark;
    QFile m_file;
    QUrl m_sumsUrl;
    QTimer m_timer;
    QUrl m_assetUrl;
    QString m_error;
    QString m_stage;
    qreal m_progress;
    QLockFile *m_lock;
    QUrl m_releaseUrl;
    qint64 m_assetSize;
    QString m_language;
    QString m_assetName;
    QString m_directory;
    QByteArray m_checksum;
    QByteArray m_response;
    QString m_latestVersion;
    QCryptographicHash m_hash;
    QTemporaryDir *m_workspace;
    QPointer<QProcess> m_process;
    QNetworkAccessManager m_network;
    QPointer<QNetworkReply> m_reply;
};
