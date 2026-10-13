#include <QFont>
#include <QJsonObject>
#include <QQuickWindow>
#include <QJsonDocument>
#include <QGuiApplication>

#include "Enum/EStyle.h"
#include "Config/ELocale.h"
#include "EEChartsBridge.h"
#include "Style/Palettes/EPaletteManager.h"

namespace
{
QString json(const QVariantMap &value)
{
    return QString::fromUtf8(QJsonDocument::fromVariant(value).toJson(QJsonDocument::Compact));
}
}

EEChartsBridge::EEChartsBridge(QObject *parent) : QObject(parent)
{
    timer_.setSingleShot(true);
    connect(&timer_, &QTimer::timeout, this, &EEChartsBridge::publish);
}

bool EEChartsBridge::dark() const
{
    return dark_;
}
bool EEChartsBridge::ready() const
{
    return ready_;
}
bool EEChartsBridge::supported() const
{
    return QQuickWindow::sceneGraphBackend() != QStringLiteral("software");
}
QUrl EEChartsBridge::pageUrl() const
{
    return QUrl(QStringLiteral("qrc:/qt/qml/EUI/Charts/chart.html"));
}
QString EEChartsBridge::language() const
{
    return language_;
}
QString EEChartsBridge::renderer() const
{
    return renderer_;
}
QVariantMap EEChartsBridge::option() const
{
    return option_;
}
QString EEChartsBridge::errorString() const
{
    return supported() ? errorString_
                       : ELocale::getInstance()->texts().value(QStringLiteral("ChartRequiresOpenGl")).toString();
}

void EEChartsBridge::attach()
{
    attached_    = true;
    optionDirty_ = true;
    errorString_.clear();
    timer_.start(0);
    emit changed();
}

void EEChartsBridge::setDark(bool dark)
{
    if (dark_ == dark)
        return;
    dark_ = dark;
    timer_.start(0);
    emit changed();
}

void EEChartsBridge::setLanguage(const QString &language)
{
    const QString normalized = language.startsWith(QStringLiteral("zh")) ? QStringLiteral("ZH") : QStringLiteral("EN");
    if (language_ == normalized)
        return;
    language_ = normalized;
    timer_.start(0);
    emit changed();
}

void EEChartsBridge::setRenderer(const QString &renderer)
{
    const QString normalized = renderer == QStringLiteral("svg") ? renderer : QStringLiteral("canvas");
    if (renderer_ == normalized)
        return;
    renderer_ = normalized;
    timer_.start(0);
    emit changed();
}

void EEChartsBridge::setOption(const QVariantMap &option)
{
    if (QJsonDocument::fromVariant(option_) == QJsonDocument::fromVariant(option))
        return;
    setOption(option, {{QStringLiteral("notMerge"), true}});
}

void EEChartsBridge::setOption(const QVariantMap &option, const QVariantMap &settings)
{
    if (attached_ && optionDirty_ && timer_.isActive())
        publish();
    option_      = option;
    optionDirty_ = true;
    settings_    = settings;
    timer_.start(0);
    emit optionChanged();
}

void EEChartsBridge::clear()
{
    option_.clear();
    optionDirty_ = false;
    timer_.stop();
    emit optionChanged();
    if (ready_)
        emit commandRequested(QStringLiteral("clear"), QStringLiteral("{}"));
}

void EEChartsBridge::resize()
{
    if (ready_)
        emit commandRequested(QStringLiteral("resize"), QStringLiteral("{}"));
}

void EEChartsBridge::terminate(int exitCode)
{
    attached_ = false;
    timer_.stop();
    report(QStringLiteral("error"),
           ELocale::getInstance()->texts().value(QStringLiteral("ChartProcessFailed")).toString().arg(exitCode));
}

void EEChartsBridge::appendData(const QVariantMap &data)
{
    if (ready_)
    {
        if (timer_.isActive())
            publish();
        emit commandRequested(QStringLiteral("appendData"), json(data));
    }
}

void EEChartsBridge::dispatchAction(const QVariantMap &action)
{
    if (ready_)
    {
        if (timer_.isActive())
            publish();
        emit commandRequested(QStringLiteral("dispatchAction"), json(action));
    }
}

void EEChartsBridge::report(const QString &event, const QString &payload)
{
    if (event == QStringLiteral("error"))
    {
        errorString_ = payload;
        if (ready_)
        {
            ready_ = false;
            emit readyChanged();
        }
        emit changed();
        emit failed(payload);
        return;
    }
    if (event == QStringLiteral("ready"))
    {
        if (!errorString_.isEmpty())
        {
            errorString_.clear();
            emit changed();
        }
        if (!ready_)
        {
            ready_ = true;
            emit readyChanged();
        }
        return;
    }
    const QVariantMap parameters = QJsonDocument::fromJson(payload.toUtf8()).object().toVariantMap();
    if (event == QStringLiteral("click"))
        emit clicked(parameters);
    else if (event == QStringLiteral("datazoom"))
        emit dataZoomed(parameters);
    else if (event == QStringLiteral("legendselectchanged"))
        emit legendSelected(parameters);
    else if (event == QStringLiteral("finished"))
        emit finished();
}

QVariantMap EEChartsBridge::theme() const
{
    auto *palette = EPaletteManager::getInstance()->getThemePalette();
    auto color    = [](const QColor &value) { return value.name(QColor::HexRgb); };
    QVariantList colors;
    for (auto style : {EStyle::Component::Primary,
                       EStyle::Component::Success,
                       EStyle::Component::Warning,
                       EStyle::Component::Danger,
                       EStyle::Component::Info})
        colors.append(color(palette->fetchColorForStyle(static_cast<int>(style))));
    const QVariantMap text{{QStringLiteral("color"), color(palette->textRegular(dark_))},
                           {QStringLiteral("fontFamily"), QGuiApplication::font().family()}};
    const QVariantMap line{{QStringLiteral("color"), color(palette->borderDefault(dark_))}};
    const QVariantMap axis{{QStringLiteral("axisLabel"), text},
                           {QStringLiteral("axisLine"), QVariantMap{{QStringLiteral("lineStyle"), line}}},
                           {QStringLiteral("splitLine"), QVariantMap{{QStringLiteral("lineStyle"), line}}}};
    const QVariantMap titleText{{QStringLiteral("color"), color(palette->textPrimary(dark_))}};
    const QVariantMap title{{QStringLiteral("textStyle"), titleText}};
    const QVariantMap tooltip{{QStringLiteral("textStyle"), text},
                              {QStringLiteral("borderColor"), color(palette->borderDefault(dark_))},
                              {QStringLiteral("backgroundColor"), color(palette->surfaceComponent(dark_))}};
    return {{QStringLiteral("color"), colors},
            {QStringLiteral("textStyle"), text},
            {QStringLiteral("categoryAxis"), axis},
            {QStringLiteral("valueAxis"), axis},
            {QStringLiteral("logAxis"), axis},
            {QStringLiteral("timeAxis"), axis},
            {QStringLiteral("legend"), QVariantMap{{QStringLiteral("textStyle"), text}}},
            {QStringLiteral("backgroundColor"), color(palette->surfaceComponent(dark_))},
            {QStringLiteral("title"), title},
            {QStringLiteral("tooltip"), tooltip}};
}

void EEChartsBridge::publish()
{
    if (!attached_)
        return;
    timer_.stop();
    const bool applyOption = optionDirty_;
    optionDirty_           = false;
    emit renderRequested(json({{QStringLiteral("option"), option_},
                               {QStringLiteral("settings"), settings_},
                               {QStringLiteral("theme"), theme()},
                               {QStringLiteral("dark"), dark_},
                               {QStringLiteral("applyOption"), applyOption},
                               {QStringLiteral("renderer"), renderer_},
                               {QStringLiteral("locale"), language_}}));
}
