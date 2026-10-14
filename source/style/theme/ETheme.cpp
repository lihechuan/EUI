#include "ETheme.h"

ETheme::ETheme(QObject *parent) : QObject(parent), darkMode_(EStyle::System::Light)
{
}

bool ETheme::dark() const
{
    return darkMode_ == EStyle::System::Dark;
}

void ETheme::changeTheme()
{
    setDark(!dark());
}

void ETheme::setDark(bool dark)
{
    const EStyle::System nextMode = dark ? EStyle::System::Dark : EStyle::System::Light;
    if (darkMode_ == nextMode)
        return;

    darkMode_ = nextMode;
    emit darkChanged();
}
