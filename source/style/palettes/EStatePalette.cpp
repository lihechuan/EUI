#include "enum/EStyle.h"
#include "EStatePalette.h"
#include "enum/EPalette.h"
#include "EPaletteManager.h"
#include "enum/EValidationState.h"

namespace
{
bool hasCustomColor(const QColor &color)
{
    return color.isValid() && color.alpha() > 0;
}
}

EStatePalette::EStatePalette(QObject *parent) : QObject(parent)
{
}

QColor EStatePalette::validationColor(int validationState)
{
    EThemePalette *theme = EPaletteManager::getInstance()->getThemePalette();

    switch (static_cast<EValidationState::State>(validationState))
    {
    case EValidationState::State::Success:
        return theme->fetchColorForStyle(static_cast<int>(EStyle::Component::Success));
    case EValidationState::State::Warning:
        return theme->fetchColorForStyle(static_cast<int>(EStyle::Component::Warning));
    case EValidationState::State::Error:
        return theme->fetchColorForStyle(static_cast<int>(EStyle::Component::Danger));
    default:
        return QColor(EPalette::Base::Transparent);
    }
}

QColor EStatePalette::resolveRectBorderColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    EThemePalette *theme       = EPaletteManager::getInstance()->getThemePalette();
    const EThemeTokens &tokens = theme->tokens(state->dark());

    if (!state->enabled())
        return tokens.borderDisabled;
    if (state->activeFocus())
        return theme->fetchColorForStyle(state->componentStyle());
    return state->hovered() ? tokens.borderHover : tokens.borderDefault;
}

QColor EStatePalette::resolveRectBackgroundColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    const EThemeTokens &tokens = EPaletteManager::getInstance()->getThemePalette()->tokens(state->dark());
    return state->enabled() ? tokens.surfaceComponent : tokens.surfaceDisabled;
}

QColor EStatePalette::resolveSwitchOffColor(EComponentStateObject *state)
{
    return resolveRectBorderColor(state);
}

QColor EStatePalette::resolveEffectiveBorderColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    const int style    = state->isFound() ? state->componentStyle() : static_cast<int>(EStyle::Component::Default);
    const QColor color = state->isFound() ? state->color() : QColor(EPalette::Base::Transparent);
    return resolveStateColor(state, style, color, Role::Border);
}

QColor EStatePalette::computeTextColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    const int style    = state->isFound() ? state->componentStyle() : static_cast<int>(EStyle::Component::Default);
    const QColor color = state->isFound() ? state->color() : QColor(EPalette::Base::Transparent);
    return resolveTextColor(state, style, color);
}

QColor EStatePalette::resolveEffectiveBackgroundColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    const int style    = state->isFound() ? state->componentStyle() : static_cast<int>(EStyle::Component::Default);
    const QColor color = state->isFound() ? state->color() : QColor(EPalette::Base::Transparent);
    const Role role    = state->border() ? Role::Border : Role::Background;
    return resolveStateColor(state, style, color, role);
}

QColor EStatePalette::computeEffectiveTextColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    return resolveTextColor(state, state->componentStyle(), state->color());
}

QColor EStatePalette::defaultTextColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    const EThemeTokens &tokens = EPaletteManager::getInstance()->getThemePalette()->tokens(state->dark());
    return state->enabled() ? tokens.textRegular : tokens.textDisabled;
}

QColor EStatePalette::computeBorderColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    return resolveStateColor(state, state->componentStyle(), state->color(), Role::Border);
}

QColor EStatePalette::computeBackgroundColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    const Role role = state->border() ? Role::Border : Role::Background;
    return resolveStateColor(state, state->componentStyle(), state->color(), role);
}

QColor EStatePalette::adjustPlainTextButtonColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();
    if (!state->enabled())
        return state->isOpenBackgroundColor() ? state->color() : QColor(EPalette::Base::Transparent);

    const EThemeTokens &tokens = EPaletteManager::getInstance()->getThemePalette()->tokens(state->dark());

    if (state->pressed())
        return tokens.surfacePressed;
    if (state->hovered() || state->isOpenBackgroundColor())
        return tokens.surfaceHover;
    return QColor(EPalette::Base::Transparent);
}

QColor EStatePalette::resolveEffectiveTextColor(EComponentStateObject *state)
{
    if (!state)
        return QColor();

    EThemePalette *theme = EPaletteManager::getInstance()->getThemePalette();
    if (hasCustomColor(state->color()))
        return state->color();
    if (state->componentStyle() != static_cast<int>(EStyle::Component::Default))
        return theme->fetchColorForStyle(state->componentStyle());
    return theme->tokens(state->dark()).textPrimary;
}

EStatePalette::Interaction EStatePalette::interaction(const EComponentStateObject *state) const
{
    if (!state->enabled())
        return Interaction::Disabled;
    if (state->pressed())
        return Interaction::Pressed;
    if (state->hovered() || state->activeFocus())
        return Interaction::Hovered;
    return Interaction::Normal;
}

EStatePalette::Variant EStatePalette::variant(const EComponentStateObject *state) const
{
    if (state->textButton())
        return Variant::Text;
    return state->plain() ? Variant::Plain : Variant::Solid;
}

EColorTone EStatePalette::toneFor(Variant componentVariant, Interaction componentInteraction, Role role) const
{
    static constexpr EColorTone solidBackground[] = {
        EColorTone::Base,
        EColorTone::Soft3,
        EColorTone::Strong2,
        EColorTone::Soft5,
    };
    static constexpr EColorTone plainBackground[] = {
        EColorTone::Soft9,
        EColorTone::Base,
        EColorTone::Strong2,
        EColorTone::Soft9,
    };
    static constexpr EColorTone plainBorder[] = {
        EColorTone::Soft5,
        EColorTone::Base,
        EColorTone::Strong2,
        EColorTone::Soft8,
    };
    static constexpr EColorTone plainForeground[] = {
        EColorTone::Base,
        EColorTone::Base,
        EColorTone::Strong2,
        EColorTone::Soft5,
    };

    const int index = static_cast<int>(componentInteraction);
    if (componentVariant == Variant::Solid)
        return solidBackground[index];
    if (role == Role::Border)
        return plainBorder[index];
    if (role == Role::Foreground || componentVariant == Variant::Text)
        return plainForeground[index];
    return plainBackground[index];
}

QColor EStatePalette::resolveTextColor(const EComponentStateObject *state, int style, const QColor &color) const
{
    EThemePalette *theme                   = EPaletteManager::getInstance()->getThemePalette();
    const EThemeTokens &tokens             = theme->tokens(state->dark());
    const Interaction componentInteraction = interaction(state);
    const Variant componentVariant         = variant(state);
    const bool custom                      = hasCustomColor(color);
    const bool semantic                    = custom || style != static_cast<int>(EStyle::Component::Default);

    if (!semantic)
    {
        if (componentInteraction == Interaction::Disabled)
            return tokens.textDisabled;
        if (componentInteraction == Interaction::Hovered || componentInteraction == Interaction::Pressed)
            return theme->fetchColorForStyle(static_cast<int>(EStyle::Component::Primary));
        return tokens.textRegular;
    }

    const EColorScale scale =
        custom ? theme->scaleForColor(color, state->dark()) : theme->scaleForStyle(style, state->dark());
    const bool filled = componentVariant == Variant::Solid ||
                        ((componentVariant == Variant::Plain) && componentInteraction != Interaction::Normal &&
                         componentInteraction != Interaction::Disabled);
    if (filled)
        return tokens.onDark;

    return scale.tone(toneFor(componentVariant, componentInteraction, Role::Foreground));
}

QColor
EStatePalette::resolveStateColor(const EComponentStateObject *state, int style, const QColor &color, Role role) const
{
    const Interaction componentInteraction = interaction(state);
    const bool custom                      = hasCustomColor(color);
    const bool semantic                    = custom || style != static_cast<int>(EStyle::Component::Default);

    if (!semantic)
        return resolveDefaultColor(state, componentInteraction, role);

    EThemePalette *theme = EPaletteManager::getInstance()->getThemePalette();
    const EColorScale scale =
        custom ? theme->scaleForColor(color, state->dark()) : theme->scaleForStyle(style, state->dark());
    return scale.tone(toneFor(variant(state), componentInteraction, role));
}

QColor EStatePalette::resolveDefaultColor(const EComponentStateObject *state,
                                          Interaction componentInteraction,
                                          Role role) const
{
    EThemePalette *theme       = EPaletteManager::getInstance()->getThemePalette();
    const EThemeTokens &tokens = theme->tokens(state->dark());
    const EColorScale &primary = theme->scaleForStyle(static_cast<int>(EStyle::Component::Primary), state->dark());

    if (role == Role::Foreground)
    {
        if (componentInteraction == Interaction::Disabled)
            return tokens.textDisabled;
        if (componentInteraction == Interaction::Hovered || componentInteraction == Interaction::Pressed)
            return primary.tone(EColorTone::Base);
        return tokens.textRegular;
    }

    if (role == Role::Border)
    {
        if (componentInteraction == Interaction::Disabled)
            return tokens.borderDisabled;
        if (componentInteraction == Interaction::Pressed)
            return primary.tone(EColorTone::Strong2);
        if (componentInteraction == Interaction::Hovered)
            return primary.tone(EColorTone::Base);
        return tokens.borderDefault;
    }

    if (componentInteraction == Interaction::Disabled)
        return tokens.surfaceDisabled;
    if (componentInteraction == Interaction::Pressed)
        return primary.tone(EColorTone::Soft8);
    if (componentInteraction == Interaction::Hovered)
        return primary.tone(EColorTone::Soft9);
    return tokens.surfaceComponent;
}
