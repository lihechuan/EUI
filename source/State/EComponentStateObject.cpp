#include "EComponentStateObject.h"

EComponentStateObject::EComponentStateObject(QObject *parent)
    : QObject(parent), m_dark(false), m_plain(false), m_border(false), m_enabled(true), m_hovered(false),
      m_isFound(false), m_pressed(false), m_textButton(false), m_activeFocus(false), m_componentStyle(0),
      m_isOpenBackgroundColor(false)
{
}

EComponentStateObject *EComponentStateObject::setDark(bool d)
{
    m_dark = d;
    return this;
}

EComponentStateObject *EComponentStateObject::setHovered(bool h)
{
    m_hovered = h;
    return this;
}

EComponentStateObject *EComponentStateObject::setPressed(bool p)
{
    m_pressed = p;
    return this;
}

EComponentStateObject *EComponentStateObject::setEnabled(bool d)
{
    m_enabled = d;
    return this;
}

EComponentStateObject *EComponentStateObject::setPlain(bool p)
{
    m_plain = p;
    return this;
}

EComponentStateObject *EComponentStateObject::setColor(const QColor &c)
{
    m_color = c;
    return this;
}

EComponentStateObject *EComponentStateObject::setComponentStyle(int s)
{
    m_componentStyle = s;
    return this;
}

EComponentStateObject *EComponentStateObject::setTextButton(bool t)
{
    m_textButton = t;
    return this;
}

EComponentStateObject *EComponentStateObject::setBorder(bool b)
{
    m_border = b;
    return this;
}

EComponentStateObject *EComponentStateObject::setActiveFocus(bool a)
{
    m_activeFocus = a;
    return this;
}

EComponentStateObject *EComponentStateObject::setIsOpenBackgroundColor(bool isOpen)
{
    m_isOpenBackgroundColor = isOpen;
    return this;
}

EComponentStateObject *EComponentStateObject::setIsFound(bool isFound)
{
    m_isFound = isFound;
    return this;
}

void EComponentStateObject::reset()
{
    m_dark                  = false;
    m_hovered               = false;
    m_pressed               = false;
    m_enabled               = true;
    m_plain                 = false;
    m_color                 = QColor();
    m_componentStyle        = 0;
    m_textButton            = false;
    m_border                = false;
    m_activeFocus           = false;
    m_isOpenBackgroundColor = false;
    m_isFound               = false;
}

bool EComponentStateObject::dark() const
{
    return m_dark;
}

bool EComponentStateObject::hovered() const
{
    return m_hovered;
}

bool EComponentStateObject::pressed() const
{
    return m_pressed;
}

bool EComponentStateObject::enabled() const
{
    return m_enabled;
}

bool EComponentStateObject::disabled() const
{
    return !m_enabled;
}

void EComponentStateObject::setEnabledValue(bool enabled)
{
    m_enabled = enabled;
}

void EComponentStateObject::setDisabledValue(bool disabled)
{
    m_enabled = !disabled;
}

bool EComponentStateObject::plain() const
{
    return m_plain;
}

QColor EComponentStateObject::color() const
{
    return m_color;
}

int EComponentStateObject::componentStyle() const
{
    return m_componentStyle;
}

bool EComponentStateObject::textButton() const
{
    return m_textButton;
}

bool EComponentStateObject::border() const
{
    return m_border;
}

bool EComponentStateObject::activeFocus() const
{
    return m_activeFocus;
}

bool EComponentStateObject::isOpenBackgroundColor() const
{
    return m_isOpenBackgroundColor;
}

bool EComponentStateObject::isFound() const
{
    return m_isFound;
}
