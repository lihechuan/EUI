#include <algorithm>
#include <QQuickItem>
#include <QQuickWindow>

#include "ELayout.h"
#include "enum/EPlacement.h"
#include "utils/LayoutUtils.h"

ELayout::ELayout(QObject *parent) : QObject(parent)
{
}

QPointF ELayout::calculateBasePos(int placement, QObject *component) const
{
    bool contentItem    = false;
    const auto *item    = LayoutUtils::toItem(component, contentItem);
    const QSizeF window = item && item->window() ? item->window()->size() : LayoutUtils::getRootWindowSize();
    const QSizeF size   = LayoutUtils::getComponentSize(component);
    qreal winW          = window.width();
    qreal winH          = window.height();
    qreal compW         = size.width();
    qreal compH         = size.height();

    QPointF pos(0, 0);

    switch (static_cast<EPlacement::Position>(placement))
    {
    case EPlacement::Position::TopLeft:
        pos = QPointF(margin, margin);
        break;
    case EPlacement::Position::Top:
        pos = QPointF(winW / 2 - compW / 2, margin);
        break;
    case EPlacement::Position::TopRight:
        pos = QPointF(winW - compW - margin, margin);
        break;
    case EPlacement::Position::Left:
        pos = QPointF(margin, winH / 2 - compH / 2);
        break;
    case EPlacement::Position::Center:
        pos = QPointF(winW / 2 - compW / 2, winH / 2 - compH / 2);
        break;
    case EPlacement::Position::Right:
        pos = QPointF(winW - compW - margin, winH / 2 - compH / 2);
        break;
    case EPlacement::Position::BottomLeft:
        pos = QPointF(margin, winH - compH - margin);
        break;
    case EPlacement::Position::Bottom:
        pos = QPointF(winW / 2 - compW / 2, winH - compH - margin);
        break;
    case EPlacement::Position::BottomRight:
        pos = QPointF(winW - compW - margin, winH - compH - margin);
        break;
    default:
        pos = QPointF(0, 0);
        break;
    }

    return pos;
}

QPointF ELayout::calculatePosition(int placement, QObject *component)
{
    if (!component)
        return QPointF();

    auto &components = componentMap[placement];
    components.erase(std::remove_if(components.begin(),
                                    components.end(),
                                    [](const QPointer<QObject> &entry) { return entry.isNull(); }),
                     components.end());
    bool contentItem = false;
    const auto *item = LayoutUtils::toItem(component, contentItem);
    QPointF pos      = calculateBasePos(placement, component);
    qreal offset     = 0;
    bool registered  = false;
    for (const auto &entry : components)
    {
        if (entry == component)
        {
            registered = true;
            break;
        }
        bool entryContent     = false;
        const auto *entryItem = LayoutUtils::toItem(entry, entryContent);
        if (!item || !entryItem || item->window() == entryItem->window())
            offset += LayoutUtils::getComponentSize(entry).height() + margin;
    }

    bool isBottom = (static_cast<EPlacement::Position>(placement) == EPlacement::Position::Bottom ||
                     static_cast<EPlacement::Position>(placement) == EPlacement::Position::BottomLeft ||
                     static_cast<EPlacement::Position>(placement) == EPlacement::Position::BottomRight);

    if (isBottom)
        pos.setY(pos.y() - offset);
    else
        pos.setY(pos.y() + offset);

    if (!registered)
        components.append(component);

    return pos;
}

QList<QObject *> ELayout::releasePosition(int placement, QObject *component)
{
    QList<QObject *> affected;
    auto &components = componentMap[placement];
    components.erase(std::remove_if(components.begin(),
                                    components.end(),
                                    [component](const QPointer<QObject> &entry)
                                    { return entry.isNull() || entry == component; }),
                     components.end());
    QMap<QQuickWindow *, qreal> offsets;
    const bool isBottom = (static_cast<EPlacement::Position>(placement) == EPlacement::Position::Bottom ||
                           static_cast<EPlacement::Position>(placement) == EPlacement::Position::BottomLeft ||
                           static_cast<EPlacement::Position>(placement) == EPlacement::Position::BottomRight);

    for (const auto &entry : components)
    {
        QObject *obj = entry.data();
        if (!obj)
            continue;
        bool contentItem  = false;
        const auto *item  = LayoutUtils::toItem(obj, contentItem);
        qreal &offset     = offsets[item ? item->window() : nullptr];
        const QSizeF size = LayoutUtils::getComponentSize(obj);
        QPointF pos       = calculateBasePos(placement, obj);
        pos.setY(isBottom ? pos.y() - offset : pos.y() + offset);
        offset += size.height() + margin;
        obj->setProperty("targetX", pos.x());
        obj->setProperty("targetY", pos.y());
        affected.append(obj);
    }

    return affected;
}
