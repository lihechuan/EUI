#include <qchar.h>

#include "EPaletteManager.h"

EPaletteManager::EPaletteManager(QObject *parent)
    : QObject(parent), basePalette(EBasePalette::getInstance(this)), statePalette(EStatePalette::getInstance(this)),
      themePalette(EThemePalette::getInstance(this)), defaultPalette(EDefaultPalette::getInstance(this))
{
}

EComponentStateObject *EPaletteManager::createStateObject(QObject *parent, int role)
{
    if (!parent)
        return nullptr;

    auto parentIt = m_stateObjects.find(parent);
    if (parentIt != m_stateObjects.end())
    {
        auto stateIt = parentIt->find(role);
        if (stateIt != parentIt->end())
        {
            (*stateIt)->reset();
            return *stateIt;
        }
    }

    auto *stateObject = new EComponentStateObject(parent);

    // 每个组件只建立一次销毁连接，避免组件使用多个颜色角色时重复连接。
    if (parentIt == m_stateObjects.end())
    {
        parentIt = m_stateObjects.insert(parent, {});
        QObject::connect(parent, &QObject::destroyed, this, [this, parent]() { m_stateObjects.remove(parent); });
    }

    parentIt->insert(role, stateObject);
    return stateObject;
}

EBasePalette *EPaletteManager::getBasePalette()
{
    return basePalette;
}

EStatePalette *EPaletteManager::getStatePalette()
{
    return statePalette;
}

EThemePalette *EPaletteManager::getThemePalette()
{
    return themePalette;
}

EDefaultPalette *EPaletteManager::getDefaultPalette()
{
    return defaultPalette;
}
