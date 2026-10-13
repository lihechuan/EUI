#pragma once

#include <QColor>
#include <QObject>

class EComponentStateObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool dark MEMBER m_dark)
    Q_PROPERTY(bool plain MEMBER m_plain)
    Q_PROPERTY(bool border MEMBER m_border)
    Q_PROPERTY(QColor color MEMBER m_color)
    Q_PROPERTY(bool hovered MEMBER m_hovered)
    Q_PROPERTY(bool isFound MEMBER m_isFound)
    Q_PROPERTY(bool pressed MEMBER m_pressed)
    Q_PROPERTY(bool textButton MEMBER m_textButton)
    Q_PROPERTY(bool activeFocus MEMBER m_activeFocus)
    Q_PROPERTY(int componentStyle MEMBER m_componentStyle)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabledValue)
    Q_PROPERTY(bool disabled READ disabled WRITE setDisabledValue)
    Q_PROPERTY(bool isOpenBackgroundColor MEMBER m_isOpenBackgroundColor)

public:
    explicit EComponentStateObject(QObject *parent = nullptr);

    void reset();
    bool dark() const;
    bool plain() const;
    bool border() const;
    QColor color() const;
    bool enabled() const;
    bool hovered() const;
    bool isFound() const;
    bool pressed() const;
    bool disabled() const;
    bool textButton() const;
    bool activeFocus() const;
    int componentStyle() const;
    bool isOpenBackgroundColor() const;
    void setEnabledValue(bool enabled);
    void setDisabledValue(bool disabled);
    Q_INVOKABLE EComponentStateObject *setDark(bool d);
    Q_INVOKABLE EComponentStateObject *setPlain(bool p);
    Q_INVOKABLE EComponentStateObject *setBorder(bool b);
    Q_INVOKABLE EComponentStateObject *setEnabled(bool d);
    Q_INVOKABLE EComponentStateObject *setHovered(bool h);
    Q_INVOKABLE EComponentStateObject *setPressed(bool p);
    Q_INVOKABLE EComponentStateObject *setTextButton(bool t);
    Q_INVOKABLE EComponentStateObject *setActiveFocus(bool a);
    Q_INVOKABLE EComponentStateObject *setComponentStyle(int s);
    Q_INVOKABLE EComponentStateObject *setIsFound(bool isFound);
    Q_INVOKABLE EComponentStateObject *setColor(const QColor &c);
    Q_INVOKABLE EComponentStateObject *setIsOpenBackgroundColor(bool isOpen);

private:
    bool m_dark;
    bool m_plain;
    bool m_border;
    QColor m_color;
    bool m_enabled;
    bool m_hovered;
    bool m_isFound;
    bool m_pressed;
    bool m_textButton;
    bool m_activeFocus;
    int m_componentStyle;
    bool m_isOpenBackgroundColor;
};
