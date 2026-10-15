#include <QFile>
#include <QTimer>
#include <QFileInfo>
#include <QSaveFile>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "EImageIO.h"

EImageIO::EImageIO(QObject *parent) : QObject(parent), network_(new QNetworkAccessManager(this))
{
}

bool EImageIO::busy() const
{
    return busy_;
}

bool EImageIO::write(const QByteArray &data, const QUrl &destination)
{
    QSaveFile file(destination.toLocalFile());
    if (!file.open(QIODevice::WriteOnly) || file.write(data) != data.size() || !file.commit())
    {
        emit failed(file.errorString());
        return false;
    }
    emit saved(destination);
    return true;
}

bool EImageIO::save(const QUrl &source, const QUrl &destination)
{
    if (busy_ || !source.isValid() || !destination.isLocalFile() || destination.hasQuery()
        || destination.hasFragment() || !QFileInfo(destination.toLocalFile()).isAbsolute())
    {
        emit failed(QStringLiteral("invalid-source-or-destination"));
        return false;
    }
    if (source.isLocalFile() || source.scheme() == QStringLiteral("qrc"))
    {
        QFile file(source.isLocalFile() ? source.toLocalFile() : QLatin1Char(':') + source.path());
        if (!file.open(QIODevice::ReadOnly))
        {
            emit failed(file.errorString());
            return false;
        }
        return write(file.readAll(), destination);
    }
    if (source.scheme() == QStringLiteral("data"))
    {
        const QByteArray encoded = source.toEncoded();
        const int comma = encoded.indexOf(',');
        if (comma < 0)
        {
            emit failed(QStringLiteral("invalid-data-url"));
            return false;
        }
        return write(encoded.left(comma).endsWith(";base64") ? QByteArray::fromBase64(encoded.mid(comma + 1))
                                                            : QByteArray::fromPercentEncoding(encoded.mid(comma + 1)),
                     destination);
    }
    if (source.scheme() != QStringLiteral("http") && source.scheme() != QStringLiteral("https"))
    {
        emit failed(QStringLiteral("unsupported-source-scheme"));
        return false;
    }
    busy_ = true;
    emit busyChanged();
    QNetworkRequest request(source);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    auto *reply = network_->get(request);
    auto *timeout = new QTimer(reply);
    timeout->setSingleShot(true);
    timeout->start(30000);
    connect(timeout, &QTimer::timeout, reply, &QNetworkReply::abort);
    connect(reply, &QNetworkReply::downloadProgress, this, &EImageIO::progress);
    connect(reply, &QNetworkReply::finished, this, [this, reply, destination]() {
        busy_ = false;
        emit busyChanged();
        if (reply->error() == QNetworkReply::NoError)
            write(reply->readAll(), destination);
        else
            emit failed(reply->errorString());
        reply->deleteLater();
    });
    return true;
}
