#pragma once

#include <QTimer>
#include <QObject>
#include <QString>

class ExampleClipboard : public QObject
{
    Q_OBJECT

public:
    explicit ExampleClipboard(QObject *parent = nullptr);

    Q_INVOKABLE void setText(const QString &text);

private:
    bool trySetText();

    QString m_text;
    int m_attempts = 0;
    QTimer m_retryTimer;
};
