#pragma once

#include <QObject>
#include <QString>
#include <QJSValue>
#include <QVariantMap>

class QJSEngine;

class EValueUtils : public QObject
{
    Q_OBJECT

public:
    explicit EValueUtils(QObject *parent = nullptr);

    Q_INVOKABLE QString paddedNumber(int number) const;
    Q_INVOKABLE QString colorName(const QString &text) const;
    Q_INVOKABLE QString parseColor(const QString &text) const;
    Q_INVOKABLE QJSValue parseDate(const QString &text) const;
    Q_INVOKABLE QString formatDate(const QJSValue &date) const;
    Q_INVOKABLE QVariantMap dateParts(const QJSValue &date) const;
    Q_INVOKABLE QString shortcutText(int key, int modifiers) const;
    Q_INVOKABLE bool validTime(const QString &text, bool seconds) const;
    Q_INVOKABLE QVariantMap shiftMonth(int year, int month, int offset) const;
    Q_INVOKABLE QVariantMap parseTime(const QString &text, bool seconds) const;
    Q_INVOKABLE QString formatTime(int hour, int minute, int second, bool seconds) const;
    Q_INVOKABLE QJSValue selectTreeOptions(const QJSValue &nodes, const QString &childrenRole, bool leavesOnly) const;
    Q_INVOKABLE bool dateAllowed(const QJSValue &date,
                                 const QString &minimum,
                                 const QString &maximum,
                                 const QJSValue &disabledDate) const;

private:
    QJSEngine *engine() const;
};
