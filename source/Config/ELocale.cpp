#include <QFile>
#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include "ELocale.h"

namespace
{
QVariantMap loadTexts(const QString &language)
{
    QFile file(QStringLiteral(":/qt/qml/EUI/i18n/%1.json").arg(language));
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << "Cannot load EUI translations:" << file.fileName() << file.errorString();
        return {};
    }
    QJsonParseError error;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &error);
    if (error.error != QJsonParseError::NoError || !document.isObject())
    {
        qWarning() << "Invalid EUI translations:" << file.fileName() << error.errorString();
        return {};
    }
    return document.object().toVariantMap();
}
}

ELocale::ELocale(QObject *parent) : QObject(parent)
{
    setLanguage(QStringLiteral("en"));
}

QString ELocale::language() const
{
    return language_;
}

QVariantMap ELocale::texts() const
{
    return texts_;
}

QVariantMap ELocale::textsForLanguage(const QString &language) const
{
    static const QVariantMap chinese = loadTexts(QStringLiteral("zh"));
    static const QVariantMap english = loadTexts(QStringLiteral("en"));
    return language.startsWith(QStringLiteral("zh"), Qt::CaseInsensitive) && !chinese.isEmpty() ? chinese : english;
}

void ELocale::setLanguage(const QString &language)
{
    const QString normalized =
        language.startsWith(QStringLiteral("zh"), Qt::CaseInsensitive) ? QStringLiteral("zh") : QStringLiteral("en");
    if (language_ == normalized)
        return;
    language_ = normalized;
    texts_ = textsForLanguage(normalized);
    emit languageChanged();
}
