#include <QFile>
#include <QPrinter>
#include <QSaveFile>
#include <QSettings>
#include <QClipboard>
#include <QPdfWriter>
#include <QPrinterInfo>
#include <QTextDocument>
#include <QGuiApplication>

#include "ETableIO.h"

namespace
{
QVariantMap result(bool ok, const QString &message = {}, const QString &text = {})
{
    return {{QStringLiteral("ok"), ok}, {QStringLiteral("text"), text}, {QStringLiteral("message"), message}};
}

QString localPath(const QUrl &url)
{
    return url.isLocalFile() ? url.toLocalFile() : url.scheme().isEmpty() ? url.toString() : QString();
}
}

ETableIO::ETableIO(QObject *parent) : QObject(parent)
{
}

QString ETableIO::clipboardText() const
{
    return QGuiApplication::clipboard()->text();
}

bool ETableIO::setClipboardText(const QString &text)
{
    QGuiApplication::clipboard()->setText(text);
    return QGuiApplication::clipboard()->text() == text;
}

QVariantMap ETableIO::readText(const QUrl &url) const
{
    const QString path = localPath(url);
    if (path.isEmpty())
        return result(false, QStringLiteral("A local file is required"));
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return result(false, file.errorString());
    if (file.size() > 64 * 1024 * 1024)
        return result(false, QStringLiteral("The input exceeds 64 MiB"));
    QString text = QString::fromUtf8(file.readAll());
    if (text.startsWith(QChar(0xfeff)))
        text.remove(0, 1);
    return result(true, {}, text);
}

QVariantMap ETableIO::writeText(const QUrl &url, const QString &text) const
{
    const QString path = localPath(url);
    if (path.isEmpty())
        return result(false, QStringLiteral("A local file is required"));
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        return result(false, file.errorString());
    const QByteArray bytes = text.toUtf8();
    if (file.write(bytes) != bytes.size() || !file.commit())
        return result(false, file.errorString());
    return result(true);
}

QVariantMap ETableIO::printHtml(const QString &html, const QUrl &url, const QString &printerName) const
{
    const QString path = localPath(url);
    if (path.isEmpty() && printerName.isEmpty())
        return result(false, QStringLiteral("Specify a PDF file or a printer"));
    QTextDocument document;
    document.setHtml(html);
    if (!path.isEmpty())
    {
        QSaveFile output(path);
        if (!output.open(QIODevice::WriteOnly))
            return result(false, output.errorString());
        {
            QPdfWriter writer(&output);
            document.print(&writer);
        }
        if (!output.commit())
            return result(false, output.errorString());
        return result(true);
    }
    if (QGuiApplication::platformName() == QStringLiteral("offscreen") ||
        !QPrinterInfo::availablePrinterNames().contains(printerName))
        return result(false, QStringLiteral("The printer is unavailable"));
    QPrinter printer(QPrinter::HighResolution);
    printer.setPrinterName(printerName);
    document.print(&printer);
    return result(true);
}

QVariantMap ETableIO::loadLayout(const QString &key) const
{
    QSettings settings(QStringLiteral("EUI"), QStringLiteral("TableLayouts"));
    return key.isEmpty() ? QVariantMap() : settings.value(key).toMap();
}

bool ETableIO::saveLayout(const QString &key, const QVariantMap &layout)
{
    if (key.isEmpty())
        return false;
    QSettings settings(QStringLiteral("EUI"), QStringLiteral("TableLayouts"));
    if (layout.isEmpty())
        settings.remove(key);
    else
        settings.setValue(key, layout);
    settings.sync();
    return settings.status() == QSettings::NoError;
}
