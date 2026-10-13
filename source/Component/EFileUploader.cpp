#include <QFile>
#include <QFileInfo>
#include <QDirIterator>
#include <QJsonDocument>
#include <QMimeDatabase>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include "Component/EFileUploader.h"

EFileUploader::EFileUploader(QObject *parent) : QObject(parent), network_(new QNetworkAccessManager(this))
{
}

bool EFileUploader::busy() const
{
    return !reply_.isNull();
}

qreal EFileUploader::progress() const
{
    return progress_;
}

QString EFileUploader::language() const
{
    return language_;
}

void EFileUploader::setLanguage(const QString &language)
{
    const QString normalized =
        language.startsWith(QStringLiteral("zh"), Qt::CaseInsensitive) ? QStringLiteral("zh") : QStringLiteral("en");
    if (language_ != normalized)
    {
        language_ = normalized;
        emit languageChanged();
    }
}

QString EFileUploader::errorText(const QString &english, const QString &chinese) const
{
    return language_ == QStringLiteral("zh") ? chinese : english;
}

void EFileUploader::cancel()
{
    if (reply_)
    {
        reply_->abort();
    }
}

QVariantMap EFileUploader::fileInfo(const QUrl &url) const
{
    const QFileInfo info(url.isLocalFile() ? url.toLocalFile() : QString());
    return {{QStringLiteral("name"), info.fileName()},
            {QStringLiteral("size"), info.size()},
            {QStringLiteral("mimeType"), QMimeDatabase().mimeTypeForFile(info).name()},
            {QStringLiteral("readable"), url.isLocalFile() && info.isFile() && info.isReadable()}};
}

QVariantList EFileUploader::directoryFiles(const QUrl &url) const
{
    const QFileInfo root(url.isLocalFile() ? url.toLocalFile() : QString());
    if (!url.isLocalFile() || !root.isDir() || !root.isReadable() || root.isSymLink())
        return {};
    const QDir directory(root.absoluteFilePath());
    QDirIterator iterator(
        directory.absolutePath(), QDir::Files | QDir::Readable | QDir::NoSymLinks, QDirIterator::Subdirectories);
    QVariantList result;
    while (iterator.hasNext())
    {
        const QString path = iterator.next();
        result.append(QVariantMap{{QStringLiteral("url"), QUrl::fromLocalFile(path)},
                                  {QStringLiteral("relativePath"), directory.relativeFilePath(path)}});
    }
    return result;
}

bool EFileUploader::submit(const QUrl &file,
                           const QUrl &endpoint,
                           const QString &fieldName,
                           const QVariantMap &headers,
                           const QVariantMap &data,
                           const QString &method,
                           bool withCredentials)
{
    if (busy())
    {
        return false;
    }

    const auto reject = [this, &file](const QString &message)
    {
        emit finished(file, false, 0, QString(), message);
        return false;
    };
    if (!endpoint.isValid() || endpoint.host().isEmpty() ||
        (endpoint.scheme() != QStringLiteral("http") && endpoint.scheme() != QStringLiteral("https")))
    {
        return reject(errorText(QStringLiteral("A valid HTTP or HTTPS upload URL is required"),
                                QStringLiteral("请设置有效的 HTTP 或 HTTPS 上传地址")));
    }
    if (!file.isLocalFile() || fieldName.isEmpty() || fieldName.contains('\r') || fieldName.contains('\n') ||
        fieldName.contains('"'))
    {
        return reject(errorText(QStringLiteral("Invalid local file or multipart field name"),
                                QStringLiteral("本地文件或上传字段名无效")));
    }
    const QString verb = method.trimmed().toUpper();
    if (verb != QStringLiteral("POST") && verb != QStringLiteral("PUT"))
        return reject(errorText(QStringLiteral("Upload method must be POST or PUT"),
                                QStringLiteral("上传方法必须为 POST 或 PUT")));
    for (auto iterator = data.cbegin(); iterator != data.cend(); ++iterator)
    {
        const QString &name = iterator.key();
        if (name.isEmpty() || name.contains('\r') || name.contains('\n') || name.contains('"') ||
            name.contains(QChar(0)))
            return reject(
                errorText(QStringLiteral("Invalid multipart data field name"), QStringLiteral("上传附加字段名无效")));
    }

    auto *body = new QFile(file.toLocalFile());
    if (!body->open(QIODevice::ReadOnly))
    {
        const QString error = body->errorString();
        delete body;
        return reject(error);
    }

    auto *multipart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    body->setParent(multipart);
    QString filename = QFileInfo(*body).fileName();
    filename.replace('"', '_');
    filename.replace('\r', '_');
    filename.replace('\n', '_');
    QHttpPart part;
    part.setHeader(QNetworkRequest::ContentTypeHeader, QMimeDatabase().mimeTypeForFile(QFileInfo(*body)).name());
    part.setHeader(QNetworkRequest::ContentDispositionHeader,
                   QStringLiteral("form-data; name=\"%1\"; filename=\"%2\"").arg(fieldName, filename));
    part.setBodyDevice(body);
    multipart->append(part);
    for (auto iterator = data.cbegin(); iterator != data.cend(); ++iterator)
    {
        QHttpPart field;
        field.setHeader(QNetworkRequest::ContentDispositionHeader,
                        QStringLiteral("form-data; name=\"%1\"").arg(iterator.key()));
        const QVariant value = iterator.value();
        field.setBody(value.type() == QVariant::Map || value.type() == QVariant::List
                          ? QJsonDocument::fromVariant(value).toJson(QJsonDocument::Compact)
                          : value.toString().toUtf8());
        multipart->append(field);
    }

    QNetworkRequest request(endpoint);
    if (!withCredentials)
    {
        request.setAttribute(QNetworkRequest::CookieLoadControlAttribute, QNetworkRequest::Manual);
        request.setAttribute(QNetworkRequest::CookieSaveControlAttribute, QNetworkRequest::Manual);
    }
    for (auto iterator = headers.cbegin(); iterator != headers.cend(); ++iterator)
    {
        const QByteArray name  = iterator.key().toUtf8();
        const QByteArray value = iterator.value().toString().toUtf8();
        if (!name.contains('\r') && !name.contains('\n') && !value.contains('\r') && !value.contains('\n'))
        {
            request.setRawHeader(name, value);
        }
    }
    currentFile_ = file;
    progress_    = 0;
    reply_ = verb == QStringLiteral("PUT") ? network_->put(request, multipart) : network_->post(request, multipart);
    multipart->setParent(reply_);
    QNetworkReply *activeReply = reply_;
    connect(activeReply,
            &QNetworkReply::uploadProgress,
            this,
            [this, activeReply](qint64 sent, qint64 total)
            {
                if (reply_ != activeReply)
                {
                    return;
                }
                progress_ = total > 0 ? qBound(qreal(0), qreal(sent) / qreal(total), qreal(1)) : 0;
                emit progressChanged();
            });
    connect(activeReply,
            &QNetworkReply::finished,
            this,
            [this, activeReply]
            {
                QNetworkReply *completedReply = activeReply;
                const QUrl file               = currentFile_;
                const int status   = completedReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                const bool success = completedReply->error() == QNetworkReply::NoError && status >= 200 && status < 300;
                const QString response =
                    completedReply->isReadable() ? QString::fromUtf8(completedReply->readAll()) : QString();
                const QString error = success ? QString()
                                              : (completedReply->error() != QNetworkReply::NoError
                                                     ? completedReply->errorString()
                                                     : errorText(QStringLiteral("HTTP upload failed with status %1"),
                                                                 QStringLiteral("HTTP 上传失败，状态码 %1"))
                                                           .arg(status));
                reply_.clear();
                completedReply->deleteLater();
                emit busyChanged();
                emit finished(file, success, status, response, error);
            });
    emit busyChanged();
    if (reply_ != activeReply)
    {
        return true;
    }
    emit progressChanged();
    if (reply_ == activeReply)
    {
        emit started(file);
    }
    return true;
}
