#pragma once

#include <QHash>
#include <QObject>

#include "EBasePalette.h"
#include "EStatePalette.h"
#include "EThemePalette.h"
#include "EDefaultPalette.h"
#include "Macros/singleton.h"

class EPaletteManager : public QObject
{
    Q_OBJECT
    SINGLETON(EPaletteManager)

    Q_PROPERTY(EBasePalette *BasePalette READ getBasePalette CONSTANT)
    Q_PROPERTY(EStatePalette *StatePalette READ getStatePalette CONSTANT)
    Q_PROPERTY(EThemePalette *ThemePalette READ getThemePalette CONSTANT)
    Q_PROPERTY(EDefaultPalette *DefaultPalette READ getDefaultPalette CONSTANT)

private:
    explicit EPaletteManager(QObject *parent = nullptr);

public:
    EBasePalette *getBasePalette();
    EStatePalette *getStatePalette();
    EThemePalette *getThemePalette();
    EDefaultPalette *getDefaultPalette();
    Q_INVOKABLE EComponentStateObject *createStateObject(QObject *parent,
                                                         int role = static_cast<int>(EStyle::Role::Uncertain));

private:
    EBasePalette *basePalette;
    EStatePalette *statePalette;
    EThemePalette *themePalette;
    EDefaultPalette *defaultPalette;
    QHash<QObject *, QHash<int, EComponentStateObject *>> m_stateObjects;
};
