#include <QQuickItem>
#include <QQuickWindow>
#include <QGuiApplication>

#include "LayoutUtils.h"

namespace LayoutUtils
{
QQuickItem *toItem(QObject *component, bool &isContentItem)
{
    if (!component)
        return nullptr;

    if (auto *item = qobject_cast<QQuickItem *>(component))
        return item;

    if (component->metaObject()->indexOfProperty("contentItem") != -1)
    {
        QObject *content = component->property("contentItem").value<QObject *>();
        if (auto *contentItem = qobject_cast<QQuickItem *>(content))
        {
            isContentItem = true;
            return contentItem;
        }
    }

    return nullptr;
}

QSizeF getRootWindowSize()
{
    const auto windows = QGuiApplication::allWindows();
    if (windows.isEmpty())
        return QSizeF(0, 0);

    for (QObject *w : windows)
    {
        if (auto *win = qobject_cast<QQuickWindow *>(w))
            return QSizeF(win->width(), win->height());
    }

    return QSizeF(0, 0);
}

QSizeF getComponentSize(QObject *component)
{
    bool isContentItem = false;
    QQuickItem *item   = toItem(component, isContentItem);

    if (isContentItem)
        item = item->parentItem();

    if (!item)
        return QSizeF(0, 0);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    return QSizeF(item->width(), item->height());
#else
    return item->size();
#endif
}
}
