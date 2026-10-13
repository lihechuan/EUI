#include "EDefaultPalette.h"
#include "EPaletteManager.h"

EDefaultPalette::EDefaultPalette(QObject *parent) : QObject(parent)
{
}

QColor EDefaultPalette::resolveThemeBorderColor() const
{
    return EPaletteManager::getInstance()->getThemePalette()->fetchColorForStyle();
}

QColor EDefaultPalette::resolveThemeBackgroundColor(bool dark) const
{
    return EPaletteManager::getInstance()->getThemePalette()->fetchThemeColor(dark);
}
