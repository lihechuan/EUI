#include <QFile>
#include <QMetaEnum>
#include <QFontDatabase>

#include "Enum/EIcons.h"
#include "Enum/EStyle.h"
#include "EDefaultConfig.h"

const int EDefaultConfig::fontPixeSize_ = 13;
const QString EDefaultConfig::font_     = "EIcons";
const QString EDefaultConfig::fontLib_  = "../../Font/EFont.ttf";

QString EDefaultConfig::font() const
{
    return font_;
}

QString EDefaultConfig::fontLib() const
{
    return fontLib_;
}

int EDefaultConfig::fontPixelSize() const
{
    return fontPixeSize_;
}

QString EDefaultConfig::primaryFontFamily() const
{
#ifdef Q_OS_WIN
    const QFontDatabase database;
    for (const auto &family :
         {QStringLiteral("Microsoft YaHei UI"), QStringLiteral("Microsoft YaHei"), QStringLiteral("Segoe UI")})
    {
        if (database.hasFamily(family))
            return family;
    }
#endif
    return QFontDatabase::systemFont(QFontDatabase::GeneralFont).family();
}

QString EDefaultConfig::secondaryFontFamily() const
{
#if defined(Q_OS_WIN) || defined(Q_OS_MACOS)
#ifdef Q_OS_WIN
    const auto family = QStringLiteral("Consolas");
#else
    const auto family = QStringLiteral("Menlo");
#endif
    if (QFontDatabase().hasFamily(family))
        return family;
#endif
    return QFontDatabase::systemFont(QFontDatabase::FixedFont).family();
}

int EDefaultConfig::iconSize(int size) const
{
    return static_cast<EStyle::Size>(size) == EStyle::Size::Small ? 12 : 14;
}

int EDefaultConfig::switchHeight(int size) const
{
    switch (static_cast<EStyle::Size>(size))
    {
    case EStyle::Size::Small:
        return 16;
    case EStyle::Size::Large:
        return 24;
    default:
        return 20;
    }
}

int EDefaultConfig::controlHeight(int size) const
{
    switch (static_cast<EStyle::Size>(size))
    {
    case EStyle::Size::Small:
        return 24;
    case EStyle::Size::Large:
        return 40;
    default:
        return 32;
    }
}

int EDefaultConfig::sliderTrackSize(int size) const
{
    switch (static_cast<EStyle::Size>(size))
    {
    case EStyle::Size::Small:
        return 4;
    case EStyle::Size::Large:
        return 8;
    default:
        return 6;
    }
}

int EDefaultConfig::sliderHandleSize(int size) const
{
    switch (static_cast<EStyle::Size>(size))
    {
    case EStyle::Size::Small:
        return 12;
    case EStyle::Size::Large:
        return 20;
    default:
        return 16;
    }
}

int EDefaultConfig::horizontalPadding(int size) const
{
    switch (static_cast<EStyle::Size>(size))
    {
    case EStyle::Size::Small:
        return 11;
    case EStyle::Size::Large:
        return 19;
    default:
        return 15;
    }
}

int EDefaultConfig::controlFontPixelSize(int size) const
{
    return static_cast<EStyle::Size>(size) == EStyle::Size::Small ? 12 : 14;
}

int EDefaultConfig::selectionIndicatorSize(int size) const
{
    switch (static_cast<EStyle::Size>(size))
    {
    case EStyle::Size::Small:
        return 12;
    case EStyle::Size::Large:
        return 16;
    default:
        return 14;
    }
}

QString EDefaultConfig::iconGraphic(int iconSource) const
{
    const auto iconType = QMetaEnum::fromType<EIcons::IconType>();
    const auto name     = iconType.valueToKey(iconSource);
    if (!name)
        return {};

    const auto path = QStringLiteral(":/qt/qml/EUI/Font/Icons/%1.svg").arg(QString::fromLatin1(name));
    return QFile::exists(path) ? QStringLiteral("qrc") + path : QString();
}
