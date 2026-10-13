#include <QQuickWindow>

#include "EWindowActions.h"

EWindowActions::EWindowActions(QObject *parent) : QObject(parent)
{
}

bool EWindowActions::move(QQuickItem *item)
{
    return item && item->window() && item->window()->startSystemMove();
}

bool EWindowActions::resize(QQuickItem *item, int edges)
{
    const int verticalEdges   = edges & (Qt::TopEdge | Qt::BottomEdge);
    const int horizontalEdges = edges & (Qt::LeftEdge | Qt::RightEdge);
    if (!item || !item->window() || !edges || edges & ~(Qt::TopEdge | Qt::LeftEdge | Qt::RightEdge | Qt::BottomEdge) ||
        horizontalEdges == (Qt::LeftEdge | Qt::RightEdge) || verticalEdges == (Qt::TopEdge | Qt::BottomEdge))
        return false;
    return item->window()->startSystemResize(Qt::Edges(edges));
}

void EWindowActions::close(QQuickItem *item)
{
    if (item && item->window())
        item->window()->close();
}

void EWindowActions::minimize(QQuickItem *item)
{
    if (item && item->window())
        item->window()->showMinimized();
}

void EWindowActions::toggleMaximized(QQuickItem *item)
{
    if (!item || !item->window())
        return;
    if (item->window()->visibility() == QWindow::Maximized || item->window()->visibility() == QWindow::FullScreen)
        item->window()->showNormal();
    else
        item->window()->showMaximized();
}

void EWindowActions::toggleFullScreen(QQuickItem *item)
{
    if (!item || !item->window())
        return;
    if (item->window()->visibility() == QWindow::FullScreen || item->window()->visibility() == QWindow::Maximized)
        item->window()->showNormal();
    else
        item->window()->showFullScreen();
}
