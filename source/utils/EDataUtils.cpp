#include <QSet>
#include <cmath>
#include <QDebug>
#include <vector>
#include <QCollator>
#include <algorithm>
#include <QQmlEngine>
#include <QQuickItem>
#include <QStringList>
#include <private/qjsvalue_p.h>

#include "EDataUtils.h"

namespace
{
struct TableRow
{
    bool missing;
    bool numeric;
    QJSValue row;
    QString text;
    quint32 index;
    double number;
};

struct TreeFrame
{
    int level;
    quint32 count;
    bool disabled;
    quint32 index;
    QJSValue items;
    QJSValue parent;
};

struct MenuFrame
{
    int level;
    quint32 count;
    bool disabled;
    quint32 index;
    QJSValue items;
    QJSValue parent;
    QStringList path;
};

quint32 arrayLength(const QJSValue &value)
{
    return value.isArray() ? value.property(QStringLiteral("length")).toUInt() : 0;
}
}

EDataUtils::EDataUtils(QQmlEngine *engine) : QObject(engine), m_engine(engine)
{
}

QString EDataUtils::objectIdentity(const QJSValue &value) const
{
    // Snapshot memo entries retain their source objects, keeping these heap identities valid during cloning.
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    const auto *object = QJSValuePrivate::asManagedType<QV4::Object>(&value);
    return object ? QString::number(reinterpret_cast<quintptr>(object->heapObject()), 16) : QString();
#else
    const auto *object = QJSValuePrivate::getValue(&value);
    return object && object->isObject() ? QString::number(reinterpret_cast<quintptr>(object->heapObject()), 16)
                                       : QString();
#endif
}

QString EDataUtils::formatNumber(double value,
                                 int precision,
                                 const QString &groupSeparator,
                                 const QString &decimalSeparator) const
{
    if (!std::isfinite(value))
        return {};
    const QString text      = QString::number(value, 'f', qBound(0, precision, 20));
    const QStringList parts = text.split(QLatin1Char('.'));
    const QString sign      = parts[0].startsWith(QLatin1Char('-')) ? QStringLiteral("-") : QString();
    const QString integer   = parts[0].mid(sign.size());
    QStringList groups;
    for (int end = integer.size(); end > 0; end -= 3)
        groups.prepend(integer.mid(qMax(0, end - 3), qMin(3, end)));
    return sign + groups.join(groupSeparator) + (parts.size() > 1 ? decimalSeparator + parts[1] : QString());
}

QJSValue EDataUtils::visiblePages(int pageCount, int currentPage, int pagerCount) const
{
    const int pages  = qMax(1, pageCount);
    const int count  = qBound(5, pagerCount, 21);
    QJSValue result  = m_engine->newArray();
    quint32 position = 0;
    if (pages <= count)
    {
        for (int page = 1; page <= pages; ++page)
            result.setProperty(position++, page);
        return result;
    }
    const int half  = (count - 3) / 2;
    const int start = qBound(2, qBound(1, currentPage, pages) - half, pages - count + 2);
    const int end   = qMin(pages - 1, start + count - 3);
    result.setProperty(position++, 1);
    if (start > 2)
        result.setProperty(position++, 0);
    for (int page = start; page <= end; ++page)
        result.setProperty(position++, page);
    if (end < pages - 1)
        result.setProperty(position++, 0);
    result.setProperty(position, pages);
    return result;
}

QJSValue EDataUtils::menuEntries(const QJSValue &model, const QJSValue &openedIndexes, bool horizontal) const
{
    QSet<QString> opened;
    for (quint32 index = 0; index < arrayLength(openedIndexes); ++index)
        opened.insert(openedIndexes.property(index).toString());
    QJSValue result              = m_engine->newArray();
    quint32 position             = 0;
    std::vector<MenuFrame> stack = {{0, arrayLength(model), false, 0, model, QJSValue(), {}}};
    while (!stack.empty())
    {
        MenuFrame &frame = stack.back();
        if (frame.index >= frame.count)
        {
            stack.pop_back();
            continue;
        }
        const quint32 sourceIndex = frame.index++;
        QJSValue item             = frame.items.property(sourceIndex);
        if (!item.isObject())
            item = m_engine->newObject();
        const bool cycle = std::any_of(stack.begin(),
                                       stack.end(),
                                       [&](const MenuFrame &ancestor) { return ancestor.parent.strictlyEquals(item); });
        if (cycle)
            continue;
        QStringList path             = frame.path;
        const QJSValue declaredIndex = item.property(QStringLiteral("index"));
        const QString fallback       = (path + QStringList{QString::number(sourceIndex)}).join(QLatin1Char('-'));
        const QString key = declaredIndex.isNull() || declaredIndex.isUndefined() ? fallback : declaredIndex.toString();
        path.append(key);
        const QJSValue children     = item.property(QStringLiteral("children"));
        const quint32 childrenCount = arrayLength(children);
        const bool disabled         = frame.disabled || item.property(QStringLiteral("disabled")).toBool();
        const QJSValue text         = item.property(QStringLiteral("text"));
        QJSValue entry              = m_engine->newObject();
        entry.setProperty(QStringLiteral("index"), key);
        entry.setProperty(QStringLiteral("indexPath"), m_engine->toScriptValue(path));
        entry.setProperty(QStringLiteral("item"), item);
        entry.setProperty(QStringLiteral("text"), text.isUndefined() ? key : text.toString());
        entry.setProperty(QStringLiteral("iconSource"), item.property(QStringLiteral("iconSource")).toInt());
        entry.setProperty(QStringLiteral("disabled"), disabled);
        entry.setProperty(QStringLiteral("level"), frame.level);
        entry.setProperty(QStringLiteral("hasChildren"), childrenCount > 0);
        result.setProperty(position++, entry);
        if (!horizontal && childrenCount && opened.contains(key))
            stack.push_back({frame.level + 1, childrenCount, disabled, 0, children, item, path});
    }
    return result;
}

QString EDataUtils::cellText(const QJSValue &row, const QJSValue &column) const
{
    QJSValue value = row;
    QJSValue prop  = column.property(QStringLiteral("prop"));
    if (prop.isUndefined())
        prop = column.property(QStringLiteral("property"));
    for (const QString &part : prop.toString().split(QLatin1Char('.')))
        value = value.isObject() ? value.property(part) : QJSValue();
    QJSValue formatter = column.property(QStringLiteral("formatter"));
    if (formatter.isCallable())
    {
        const QJSValue result = formatter.call({row, value});
        if (result.isError())
        {
            qWarning().noquote() << "ETable formatter:" << result.toString();
            return {};
        }
        return result.toString();
    }
    return value.isNull() || value.isUndefined() ? QString() : value.toString();
}

QJSValue EDataUtils::treeEntry(const QJSValue &nodes, const QString &key) const
{
    const quint32 count = arrayLength(nodes);
    for (quint32 index = 0; index < count; ++index)
    {
        const QJSValue entry = nodes.property(index);
        if (entry.property(QStringLiteral("key")).toString() == key)
            return entry;
    }
    return QJSValue(QJSValue::NullValue);
}

QJSValue EDataUtils::treeVisibleNodes(const QJSValue &nodes, const QJSValue &expandedKeys) const
{
    QSet<QString> expanded;
    const quint32 expandedCount = arrayLength(expandedKeys);
    for (quint32 index = 0; index < expandedCount; ++index)
        expanded.insert(expandedKeys.property(index).toString());

    QJSValue result     = m_engine->newArray();
    const quint32 count = arrayLength(nodes);
    bool filtered       = false;
    int hiddenLevel     = -1;
    quint32 position    = 0;
    for (quint32 index = 0; index < count; ++index)
    {
        const QJSValue entry = nodes.property(index);
        if (!filtered)
        {
            if (!entry.property(QStringLiteral("hasChildren")).toBool() ||
                expanded.contains(entry.property(QStringLiteral("key")).toString()))
                continue;
            filtered = true;
            for (quint32 prefix = 0; prefix <= index; ++prefix)
                result.setProperty(position++, nodes.property(prefix));
            hiddenLevel = entry.property(QStringLiteral("level")).toInt();
            continue;
        }
        const int level = entry.property(QStringLiteral("level")).toInt();
        if (hiddenLevel >= 0 && level > hiddenLevel)
            continue;
        hiddenLevel = -1;
        result.setProperty(position++, entry);
        if (entry.property(QStringLiteral("hasChildren")).toBool() &&
            !expanded.contains(entry.property(QStringLiteral("key")).toString()))
            hiddenLevel = level;
    }
    return filtered || !nodes.isArray() ? result : nodes;
}

QJSValue EDataUtils::columnWidths(const QJSValue &columns, qreal availableWidth) const
{
    const quint32 count = arrayLength(columns);
    std::vector<qreal> widths(count);
    qreal fixed      = 0;
    quint32 flexible = 0;
    for (quint32 index = 0; index < count; ++index)
    {
        const double requested = columns.property(index).property(QStringLiteral("width")).toNumber();
        if (std::isfinite(requested) && requested > 0)
        {
            widths[index] = qMax(qreal(40), qreal(requested));
            fixed += widths[index];
        }
        else
            ++flexible;
    }

    qreal minimum = 0;
    for (quint32 index = 0; index < count; ++index)
    {
        if (widths[index] > 0)
            continue;
        const double requested = columns.property(index).property(QStringLiteral("minWidth")).toNumber();
        widths[index]          = std::isfinite(requested) && requested > 0 ? qMax(qreal(40), qreal(requested)) : 100;
        minimum += widths[index];
    }
    const qreal extra = qMax(qreal(0), availableWidth - fixed - minimum) / qMax(quint32(1), flexible);
    QJSValue values   = m_engine->newArray(count);
    qreal total       = 0;
    for (quint32 index = 0; index < count; ++index)
    {
        const double requested = columns.property(index).property(QStringLiteral("width")).toNumber();
        const qreal width      = widths[index] + (std::isfinite(requested) && requested > 0 ? 0 : extra);
        values.setProperty(index, width);
        total += width;
    }
    QJSValue result = m_engine->newObject();
    result.setProperty(QStringLiteral("widths"), values);
    result.setProperty(QStringLiteral("total"), total);
    return result;
}

QJSValue EDataUtils::tableRows(const QJSValue &model, const QString &prop, const QString &order) const
{
    const quint32 count = arrayLength(model);
    const bool sorted =
        !prop.isEmpty() && (order == QStringLiteral("ascending") || order == QStringLiteral("descending"));
    QJSValue result = m_engine->newArray(count);
    std::vector<TableRow> rows;
    if (sorted)
        rows.reserve(count);
    for (quint32 index = 0; index < count; ++index)
    {
        const QJSValue row = model.property(index);
        if (!sorted)
        {
            QJSValue entry = m_engine->newObject();
            entry.setProperty(QStringLiteral("row"), row);
            entry.setProperty(QStringLiteral("index"), index);
            result.setProperty(index, entry);
            continue;
        }
        QJSValue value = row;
        for (const QString &part : prop.split(QLatin1Char('.')))
            value = value.isObject() ? value.property(part) : QJSValue();
        const bool numeric  = value.isNumber();
        const double number = numeric ? value.toNumber() : 0;
        rows.push_back({value.isNull() || value.isUndefined() || (numeric && std::isnan(number)),
                        numeric,
                        row,
                        numeric ? QString() : value.toString(),
                        index,
                        number});
    }
    if (!sorted)
        return result;

    const int direction = order == QStringLiteral("descending") ? -1 : 1;
    QCollator collator;
    std::stable_sort(rows.begin(),
                     rows.end(),
                     [&](const TableRow &left, const TableRow &right)
                     {
                         if (left.missing || right.missing)
                             return left.missing != right.missing && (direction > 0 ? !left.missing : left.missing);
                         if (left.numeric != right.numeric)
                             return direction > 0 ? left.numeric : right.numeric;
                         if (left.numeric)
                             return direction > 0 ? left.number < right.number : left.number > right.number;
                         return direction * collator.compare(left.text, right.text) < 0;
                     });
    for (quint32 index = 0; index < count; ++index)
    {
        QJSValue entry = m_engine->newObject();
        entry.setProperty(QStringLiteral("row"), rows[index].row);
        entry.setProperty(QStringLiteral("index"), rows[index].index);
        result.setProperty(index, entry);
    }
    return result;
}

QJSValue EDataUtils::treeNodes(const QJSValue &model,
                               const QJSValue &expandedKeys,
                               bool visibleOnly,
                               int initialLevel,
                               const QJSValue &parentPath,
                               bool inheritedDisabled) const
{
    QSet<QString> expanded;
    const quint32 expandedCount = arrayLength(expandedKeys);
    for (quint32 index = 0; index < expandedCount; ++index)
        expanded.insert(expandedKeys.property(index).toString());

    QStringList prefix;
    const quint32 prefixCount = arrayLength(parentPath);
    for (quint32 index = 0; index < prefixCount; ++index)
        prefix.append(parentPath.property(index).toString());

    QJSValue result = m_engine->newArray();
    std::vector<TreeFrame> stack;
    stack.push_back({initialLevel, arrayLength(model), inheritedDisabled, 0, model, QJSValue()});
    quint32 position = 0;
    while (!stack.empty())
    {
        TreeFrame &frame = stack.back();
        if (frame.index >= frame.count)
        {
            stack.pop_back();
            continue;
        }
        QJSValue node = frame.items.property(frame.index++);
        if (!node.isObject())
            node = m_engine->newObject();
        const bool cycle = std::any_of(stack.begin(),
                                       stack.end(),
                                       [&](const TreeFrame &ancestor) { return ancestor.parent.strictlyEquals(node); });
        if (cycle)
            continue;
        const QJSValue children     = node.property(QStringLiteral("children"));
        const quint32 childrenCount = arrayLength(children);
        const bool disabled         = frame.disabled || node.property(QStringLiteral("disabled")).toBool();
        const QJSValue id           = node.property(QStringLiteral("id"));
        QString key;
        if (!id.isUndefined())
            key = id.toString();
        else
        {
            QStringList path = prefix;
            for (const TreeFrame &ancestor : stack)
                path.append(QString::number(ancestor.index - 1));
            key = path.join(QLatin1Char('-'));
        }

        const QJSValue label = node.property(QStringLiteral("label"));
        const QJSValue text  = node.property(QStringLiteral("text"));
        QJSValue entry       = m_engine->newObject();
        entry.setProperty(QStringLiteral("key"), key);
        entry.setProperty(QStringLiteral("node"), node);
        entry.setProperty(QStringLiteral("level"), frame.level);
        entry.setProperty(QStringLiteral("disabled"), disabled);
        entry.setProperty(QStringLiteral("hasChildren"), childrenCount > 0);
        entry.setProperty(QStringLiteral("label"),
                          label.isUndefined() ? (text.toBool() ? text.toString() : key) : label.toString());
        result.setProperty(position++, entry);

        if (childrenCount == 0 || (visibleOnly && !expanded.contains(key)))
            continue;
        stack.push_back({frame.level + 1, childrenCount, disabled, 0, children, node});
    }
    return result;
}

QVariantList EDataUtils::configProviders(QObject *control) const
{
    QVariantList providers;
    QList<QObject *> pending{control};
    QSet<QObject *> seen;
    for (int index = 0; index < pending.size(); ++index)
    {
        QObject *current = pending.at(index);
        if (!current || seen.contains(current))
            continue;
        seen.insert(current);
        if (current != control && current->property("euiConfigProvider").toBool())
            providers.append(QVariant::fromValue(current));
        if (auto *item = qobject_cast<QQuickItem *>(current))
            pending.append(item->parentItem());
        pending.append(current->parent());
    }
    return providers;
}
