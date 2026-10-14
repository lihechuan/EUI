#pragma once

#include <QQuickItem>

class EMasonryLayout : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(int columns READ columns WRITE setColumns NOTIFY changed)
    Q_PROPERTY(qreal spacing READ spacing WRITE setSpacing NOTIFY changed)

public:
    ~EMasonryLayout() override;
    explicit EMasonryLayout(QQuickItem *parent = nullptr);

    int columns() const;
    qreal spacing() const;
    void setColumns(int columns);
    void setSpacing(qreal spacing);

signals:
    void changed();

private:
    void watchChildren();

protected:
    void updatePolish() override;

private:
    int columns_        = 3;
    qreal spacing_      = 12;
    bool childrenDirty_ = true;
    QList<QMetaObject::Connection> connections_;
};
