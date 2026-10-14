#pragma once

#include <QColor>
#include <QObject>

#include "macros/singleton.h"
#include "style/palettes/EColorScale.h"
#include "state/EComponentStateObject.h"

class EStatePalette : public QObject
{
    Q_OBJECT
    friend class EPaletteManager;
    enum class Interaction
    {
        Normal,
        Hovered,
        Pressed,
        Disabled,
    };
    enum class Variant
    {
        Text,
        Plain,
        Solid,
    };
    enum class Role
    {
        Border,
        Background,
        Foreground,
    };
    SINGLETON(EStatePalette)

    explicit EStatePalette(QObject *parent = nullptr);

    Q_INVOKABLE QColor validationColor(int validationState);
    Variant variant(const EComponentStateObject *state) const;
    Q_INVOKABLE QColor computeTextColor(EComponentStateObject *state);
    Q_INVOKABLE QColor defaultTextColor(EComponentStateObject *state);
    Interaction interaction(const EComponentStateObject *state) const;
    Q_INVOKABLE QColor computeBorderColor(EComponentStateObject *state);
    Q_INVOKABLE QColor resolveSwitchOffColor(EComponentStateObject *state);
    Q_INVOKABLE QColor computeBackgroundColor(EComponentStateObject *state);
    Q_INVOKABLE QColor resolveRectBorderColor(EComponentStateObject *state);
    Q_INVOKABLE QColor computeEffectiveTextColor(EComponentStateObject *state);
    Q_INVOKABLE QColor resolveEffectiveTextColor(EComponentStateObject *state);
    Q_INVOKABLE QColor adjustPlainTextButtonColor(EComponentStateObject *state);
    Q_INVOKABLE QColor resolveRectBackgroundColor(EComponentStateObject *state);
    Q_INVOKABLE QColor resolveEffectiveBorderColor(EComponentStateObject *state);
    EColorTone toneFor(Variant variant, Interaction interaction, Role role) const;
    Q_INVOKABLE QColor resolveEffectiveBackgroundColor(EComponentStateObject *state);
    QColor resolveTextColor(const EComponentStateObject *state, int style, const QColor &color) const;
    QColor resolveDefaultColor(const EComponentStateObject *state, Interaction interaction, Role role) const;
    QColor resolveStateColor(const EComponentStateObject *state, int style, const QColor &color, Role role) const;
};
