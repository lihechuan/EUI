#include <QDate>
#include <QTime>
#include <QtQml>
#include <QColor>
#include <vector>
#include <QDateTime>
#include <QJSEngine>
#include <algorithm>
#include <QKeySequence>

#include "Config/EValueUtils.h"

namespace
{
QDate dateFromText(const QString &text)
{
    const QDate date = QDate::fromString(text, QStringLiteral("yyyy-MM-dd"));
    return date.isValid() && text.size() == 10 && date.year() >= 100 &&
                   date.toString(QStringLiteral("yyyy-MM-dd")) == text
               ? date
               : QDate();
}
}

EValueUtils::EValueUtils(QObject *parent) : QObject(parent)
{
}

QString EValueUtils::shortcutText(int key, int modifiers) const
{
    if (key <= 0 || key == Qt::Key_unknown || key == Qt::Key_Shift || key == Qt::Key_Control || key == Qt::Key_Alt ||
        key == Qt::Key_Meta || key == Qt::Key_AltGr)
    {
        return {};
    }
    const int mask = Qt::ShiftModifier | Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier;
    return QKeySequence(key | (modifiers & mask)).toString(QKeySequence::NativeText);
}

QJSEngine *EValueUtils::engine() const
{
    QJSEngine *result = qjsEngine(this);
    return result ? result : qobject_cast<QJSEngine *>(parent());
}

QJSValue EValueUtils::parseDate(const QString &text) const
{
    const QDate date = dateFromText(text);
    if (!date.isValid())
    {
        return QJSValue(QJSValue::NullValue);
    }
    QJSEngine *scriptEngine = engine();
    if (!scriptEngine)
    {
        return QJSValue(QJSValue::NullValue);
    }
    QJSValue constructor = scriptEngine->globalObject().property(QStringLiteral("Date"));
    return constructor.callAsConstructor({QJSValue(double(QDateTime(date, QTime(0, 0)).toMSecsSinceEpoch()))});
}

QString EValueUtils::formatDate(const QJSValue &date) const
{
    return date.isDate() ? date.toDateTime().date().toString(QStringLiteral("yyyy-MM-dd")) : QString();
}

QString EValueUtils::paddedNumber(int number) const
{
    return QStringLiteral("%1").arg(number, 2, 10, QLatin1Char('0'));
}

QVariantMap EValueUtils::dateParts(const QJSValue &date) const
{
    const QDate parsed = date.isDate() ? date.toDateTime().date() : QDate();
    if (!parsed.isValid())
    {
        return {};
    }
    return {{QStringLiteral("year"), parsed.year()}, {QStringLiteral("month"), parsed.month() - 1}};
}

bool EValueUtils::validTime(const QString &text, bool seconds) const
{
    const QString format = seconds ? QStringLiteral("HH:mm:ss") : QStringLiteral("HH:mm");
    const QTime time     = QTime::fromString(text, format);
    return time.isValid() && time.toString(format) == text;
}

QVariantMap EValueUtils::parseTime(const QString &text, bool seconds) const
{
    const QTime time = validTime(text, seconds)
                           ? QTime::fromString(text, seconds ? QStringLiteral("HH:mm:ss") : QStringLiteral("HH:mm"))
                           : QTime(0, 0);
    return {{QStringLiteral("hour"), time.hour()},
            {QStringLiteral("minute"), time.minute()},
            {QStringLiteral("second"), time.second()}};
}

QVariantMap EValueUtils::shiftMonth(int year, int month, int offset) const
{
    const QDate date = QDate(year, 1, 1).addMonths(month + offset);
    if (!date.isValid())
    {
        return {{QStringLiteral("year"), year}, {QStringLiteral("month"), month}};
    }
    return {{QStringLiteral("year"), date.year()}, {QStringLiteral("month"), date.month() - 1}};
}

QString EValueUtils::formatTime(int hour, int minute, int second, bool seconds) const
{
    return QTime(hour, minute, seconds ? second : 0)
        .toString(seconds ? QStringLiteral("HH:mm:ss") : QStringLiteral("HH:mm"));
}

bool EValueUtils::dateAllowed(const QJSValue &date,
                              const QString &minimum,
                              const QString &maximum,
                              const QJSValue &disabledDate) const
{
    const QString key = formatDate(date);
    if (key.isEmpty() || (!minimum.isEmpty() && (!dateFromText(minimum).isValid() || key < minimum)) ||
        (!maximum.isEmpty() && (!dateFromText(maximum).isValid() || key > maximum)))
    {
        return false;
    }
    QJSValue callback = disabledDate;
    return !callback.isCallable() || !callback.call({date}).toBool();
}

QJSValue EValueUtils::selectTreeOptions(const QJSValue &nodes, const QString &childrenRole, bool leavesOnly) const
{
    QJSEngine *scriptEngine = engine();
    if (!scriptEngine)
    {
        return QJSValue(QJSValue::NullValue);
    }
    struct Frame
    {
        int depth;
        quint32 index;
        QJSValue items;
        QJSValue parent;
    };
    QJSValue result = scriptEngine->newArray();
    std::vector<Frame> stack{{0, 0, nodes, QJSValue()}};
    quint32 position = 0;
    while (!stack.empty())
    {
        Frame &frame = stack.back();
        if (!frame.items.isArray() || frame.index >= frame.items.property(QStringLiteral("length")).toUInt())
        {
            stack.pop_back();
            continue;
        }
        const QJSValue node = frame.items.property(frame.index++);
        if (!node.isObject() || node.property(QStringLiteral("disabled")).toBool() ||
            std::any_of(stack.begin(),
                        stack.end(),
                        [&](const Frame &ancestor) { return ancestor.parent.strictlyEquals(node); }))
        {
            continue;
        }
        const QJSValue children = node.property(childrenRole);
        const bool hasChildren  = children.isArray() && children.property(QStringLiteral("length")).toUInt() > 0;
        if (!leavesOnly || !hasChildren)
        {
            const QJSValue label = node.property(QStringLiteral("label"));
            const QJSValue value = node.property(QStringLiteral("value"));
            QJSValue option      = scriptEngine->newObject();
            option.setProperty(QStringLiteral("level"), frame.depth);
            option.setProperty(QStringLiteral("label"),
                               label.isUndefined() || label.isNull() ? QString() : label.toString());
            option.setProperty(QStringLiteral("value"), value.isUndefined() ? label : value);
            result.setProperty(position++, option);
        }
        if (hasChildren)
        {
            stack.push_back({frame.depth + 1, 0, children, node});
        }
    }
    return result;
}

QString EValueUtils::parseColor(const QString &text) const
{
    const QColor color(text.trimmed());
    return color.isValid() ? color.name(QColor::HexArgb) : QString();
}

QString EValueUtils::colorName(const QString &text) const
{
    const QColor color(text.trimmed());
    if (!color.isValid())
    {
        return {};
    }
    for (const QString &name : QColor::colorNames())
    {
        if (QColor(name).rgba() == color.rgba())
        {
            return name;
        }
    }
    return color.name(color.alpha() == 255 ? QColor::HexRgb : QColor::HexArgb);
}
