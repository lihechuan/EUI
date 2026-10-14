#include <QVector>
#include <algorithm>

#include "EMasonryLayout.h"

EMasonryLayout::EMasonryLayout(QQuickItem *parent) : QQuickItem(parent)
{
    connect(this,
            &QQuickItem::childrenChanged,
            this,
            [this]()
            {
                childrenDirty_ = true;
                polish();
            });
    connect(this, &QQuickItem::widthChanged, this, &QQuickItem::polish);
}

EMasonryLayout::~EMasonryLayout()
{
    disconnect(this, nullptr, this, nullptr);
    for (const auto &connection : connections_)
        disconnect(connection);
}

int EMasonryLayout::columns() const
{
    return columns_;
}
qreal EMasonryLayout::spacing() const
{
    return spacing_;
}

void EMasonryLayout::setColumns(int columns)
{
    columns = qBound(1, columns, 64);
    if (columns_ == columns)
        return;
    columns_ = columns;
    polish();
    emit changed();
}

void EMasonryLayout::setSpacing(qreal spacing)
{
    spacing = qIsFinite(spacing) ? qMax<qreal>(0, spacing) : 0;
    if (qFuzzyCompare(spacing_, spacing))
        return;
    spacing_ = spacing;
    polish();
    emit changed();
}

void EMasonryLayout::watchChildren()
{
    for (const auto &connection : connections_)
        disconnect(connection);
    connections_.clear();
    for (QQuickItem *item : childItems())
    {
        connections_.append(connect(item, &QQuickItem::heightChanged, this, &QQuickItem::polish));
        connections_.append(connect(item, &QQuickItem::implicitHeightChanged, this, &QQuickItem::polish));
        connections_.append(connect(item, &QQuickItem::visibleChanged, this, &QQuickItem::polish));
    }
}

void EMasonryLayout::updatePolish()
{
    if (childrenDirty_)
    {
        childrenDirty_ = false;
        watchChildren();
    }
    QVector<qreal> heights(columns_, 0);
    const qreal columnWidth = qMax<qreal>(0, (width() - spacing_ * (columns_ - 1)) / columns_);
    for (QQuickItem *item : childItems())
    {
        if (!item->isVisible())
            continue;
        item->setWidth(columnWidth);
        if (item->height() <= 0)
            continue;
        const auto lowest = std::min_element(heights.begin(), heights.end());
        const int column  = static_cast<int>(lowest - heights.begin());
        item->setX(column * (columnWidth + spacing_));
        item->setY(heights[column]);
        heights[column] += qMax<qreal>(0, item->height()) + spacing_;
    }
    setImplicitHeight(qMax<qreal>(0, *std::max_element(heights.begin(), heights.end()) - spacing_));
}
