#pragma once

#include <QColor>
#include <QObject>
#include <QtQml/qqml.h>

#include "enum/EStyle.h"
#include "macros/singleton.h"

class ETheme : public QObject
{
    Q_OBJECT
    SINGLETON(ETheme)

    Q_PROPERTY(bool dark READ dark WRITE setDark NOTIFY darkChanged)

    explicit ETheme(QObject *parent = nullptr);

    bool dark() const;
    Q_INVOKABLE void changeTheme();
    Q_INVOKABLE void setDark(bool dark);

    Q_SIGNAL void darkChanged();

    EStyle::System darkMode_;
};
