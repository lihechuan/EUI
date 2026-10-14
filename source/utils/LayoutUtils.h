#pragma once

#include <QSizeF>

class QObject;
class QQuickItem;

namespace LayoutUtils
{
QSizeF getRootWindowSize();
QSizeF getComponentSize(QObject *component);
QQuickItem *toItem(QObject *component, bool &isContentItem);
}
