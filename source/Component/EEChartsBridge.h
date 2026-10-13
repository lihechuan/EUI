#pragma once

#include <QUrl>
#include <QTimer>
#include <QObject>
#include <QVariantMap>

class EEChartsBridge : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl pageUrl READ pageUrl CONSTANT)
    Q_PROPERTY(bool supported READ supported CONSTANT)
    Q_PROPERTY(bool ready READ ready NOTIFY readyChanged)
    Q_PROPERTY(bool dark READ dark WRITE setDark NOTIFY changed)
    Q_PROPERTY(QString errorString READ errorString NOTIFY changed)
    Q_PROPERTY(QString language READ language WRITE setLanguage NOTIFY changed)
    Q_PROPERTY(QString renderer READ renderer WRITE setRenderer NOTIFY changed)
    Q_PROPERTY(QVariantMap option READ option WRITE setOption NOTIFY optionChanged)

public:
    explicit EEChartsBridge(QObject *parent = nullptr);

    bool dark() const;
    bool ready() const;
    QUrl pageUrl() const;
    bool supported() const;
    void setDark(bool dark);
    Q_INVOKABLE void clear();
    QString language() const;
    QString renderer() const;
    Q_INVOKABLE void attach();
    Q_INVOKABLE void resize();
    QVariantMap option() const;
    QString errorString() const;
    Q_INVOKABLE void terminate(int exitCode);
    void setLanguage(const QString &language);
    void setOption(const QVariantMap &option);
    void setRenderer(const QString &renderer);
    Q_INVOKABLE void appendData(const QVariantMap &data);
    Q_INVOKABLE void dispatchAction(const QVariantMap &action);
    Q_INVOKABLE void report(const QString &event, const QString &payload);
    Q_INVOKABLE void setOption(const QVariantMap &option, const QVariantMap &settings);

signals:
    void changed();
    void finished();
    void readyChanged();
    void optionChanged();
    void failed(const QString &message);
    void clicked(const QVariantMap &parameters);
    void dataZoomed(const QVariantMap &parameters);
    void legendSelected(const QVariantMap &parameters);
    void renderRequested(const QString &configuration);
    void commandRequested(const QString &name, const QString &payload);

private:
    void publish();
    QVariantMap theme() const;

    QTimer timer_;
    bool dark_ = false;
    QVariantMap option_;
    bool ready_ = false;
    QString errorString_;
    QVariantMap settings_;
    bool attached_    = false;
    bool optionDirty_ = true;
    QString language_ = QStringLiteral("en");
    QString renderer_ = QStringLiteral("canvas");
};
