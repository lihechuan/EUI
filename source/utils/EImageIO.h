#pragma once

#include <QUrl>
#include <QObject>

class QNetworkAccessManager;

class EImageIO : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool busy READ busy NOTIFY busyChanged)

public:
    explicit EImageIO(QObject *parent = nullptr);

    bool busy() const;
    Q_INVOKABLE bool save(const QUrl &source, const QUrl &destination);

signals:
    void busyChanged();
    void failed(const QString &message);
    void saved(const QUrl &destination);
    void progress(qint64 received, qint64 total);

private:
    bool write(const QByteArray &data, const QUrl &destination);

    bool busy_ = false;
    QNetworkAccessManager *network_;
};
