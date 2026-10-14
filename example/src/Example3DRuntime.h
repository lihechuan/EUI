#pragma once

#include <QFile>
#include <QTimer>
#include <QObject>
#include <QPointer>
#include <QJsonArray>
#include <QCryptographicHash>

class QProcess;
class QTemporaryDir;

class Example3DRuntime : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY changed)
    Q_PROPERTY(bool supported READ supported CONSTANT)
    Q_PROPERTY(QString error READ error NOTIFY changed)
    Q_PROPERTY(QString stage READ stage NOTIFY changed)
    Q_PROPERTY(qreal progress READ progress NOTIFY changed)
    Q_PROPERTY(bool installed READ installed NOTIFY changed)

public:
    ~Example3DRuntime() override;
    explicit Example3DRuntime(QObject *parent = nullptr, const QString &payloadDirectory = QString());

    bool busy() const;
    QString error() const;
    QString stage() const;
    bool installed() const;
    bool supported() const;
    qreal progress() const;
    Q_INVOKABLE void confirmReady();
    Q_INVOKABLE void install(const QString &language, bool dark);

signals:
    void changed();
    void handoffReady();

private:
    void launch();
    void copyNext();
    QString locatePayload();
    bool saveInstallation();
    bool cacheVerified() const;
    void fail(const QString &error);
    QByteArray cacheFingerprint() const;
    bool readManifest(const QString &directory);

    bool m_busy;
    bool m_dark;
    int m_index;
    bool m_cached;
    QFile m_input;
    QFile m_output;
    QTimer m_timer;
    qint64 m_total;
    QString m_error;
    QString m_stage;
    bool m_installed;
    qreal m_progress;
    bool m_skipCache;
    QString m_source;
    QTimer m_deadline;
    QJsonArray m_files;
    QString m_language;
    qint64 m_processed;
    QString m_readyFile;
    QString m_destination;
    QByteArray m_manifest;
    QTemporaryDir *m_staging;
    QCryptographicHash m_hash;
    QPointer<QProcess> m_child;
    QString m_payloadDirectory;
};
