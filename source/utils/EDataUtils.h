#pragma once

#include <QObject>
#include <QJSValue>
#include <QVariantList>

class QQmlEngine;

class EDataUtils : public QObject
{
    Q_OBJECT

public:
    explicit EDataUtils(QQmlEngine *engine);

    Q_INVOKABLE QString objectIdentity(const QJSValue &value) const;
    Q_INVOKABLE QVariantList configProviders(QObject *control) const;
    Q_INVOKABLE QString cellText(const QJSValue &row, const QJSValue &column) const;
    Q_INVOKABLE QJSValue treeEntry(const QJSValue &nodes, const QString &key) const;
    Q_INVOKABLE QJSValue columnWidths(const QJSValue &columns, qreal availableWidth) const;
    Q_INVOKABLE QJSValue visiblePages(int pageCount, int currentPage, int pagerCount) const;
    Q_INVOKABLE QJSValue treeVisibleNodes(const QJSValue &nodes, const QJSValue &expandedKeys) const;
    Q_INVOKABLE QJSValue tableRows(const QJSValue &model, const QString &prop, const QString &order) const;
    Q_INVOKABLE QJSValue menuEntries(const QJSValue &model, const QJSValue &openedIndexes, bool horizontal) const;
    Q_INVOKABLE QString formatNumber(double value,
                                     int precision,
                                     const QString &groupSeparator,
                                     const QString &decimalSeparator) const;
    Q_INVOKABLE QJSValue treeNodes(const QJSValue &model,
                                   const QJSValue &expandedKeys,
                                   bool visibleOnly,
                                   int initialLevel           = 0,
                                   const QJSValue &parentPath = QJSValue(),
                                   bool inheritedDisabled     = false) const;

private:
    QQmlEngine *m_engine;
};
