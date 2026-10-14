#include <QDir>
#include <QFileInfo>

#include "EFilePickerUtils.h"

EFilePickerUtils::EFilePickerUtils(QObject *parent) : QObject(parent)
{
}

QString EFilePickerUtils::filePath(const QUrl &url) const
{
    return url.isLocalFile() ? QDir::toNativeSeparators(url.toLocalFile()) : url.toDisplayString();
}

QVariantMap EFilePickerUtils::selection(const QVariantList &urls, const QString &mode, bool multiple) const
{
    const auto rejected = [](const QString &reason) {
        return QVariantMap{{QStringLiteral("valid"), false}, {QStringLiteral("reason"), reason}};
    };
    if (mode != QStringLiteral("open") && mode != QStringLiteral("save") && mode != QStringLiteral("folder"))
        return rejected(QStringLiteral("invalid-mode"));
    if (urls.isEmpty())
        return rejected(QStringLiteral("empty-selection"));
    QVariantList result;
    for (const auto &value : urls)
    {
        const QUrl url(value.toString());
        if (!url.isValid() || !url.isLocalFile() || url.hasQuery() || url.hasFragment())
            return rejected(QStringLiteral("invalid-url"));
        const QFileInfo file(url.toLocalFile());
        if (!file.isAbsolute() || (file.fileName().isEmpty() && mode != QStringLiteral("folder")))
            return rejected(QStringLiteral("invalid-path"));
        if (mode == QStringLiteral("open") && !file.isFile())
            return rejected(QStringLiteral("not-a-file"));
        if (mode == QStringLiteral("folder") && !file.isDir())
            return rejected(QStringLiteral("not-a-folder"));
        if (mode == QStringLiteral("save") && (file.isDir() || !QFileInfo(file.absolutePath()).isDir()))
            return rejected(QStringLiteral("invalid-save-target"));
        const QString normalized =
            QUrl::fromLocalFile(QDir::cleanPath(file.absoluteFilePath())).toString(QUrl::FullyEncoded);
        if (!result.contains(normalized))
            result.append(normalized);
    }
    if (result.size() > 1 && (mode != QStringLiteral("open") || !multiple))
        return rejected(QStringLiteral("multiple-not-allowed"));
    return {{QStringLiteral("valid"), true}, {QStringLiteral("urls"), result}};
}
