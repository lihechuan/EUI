#pragma once

#include <QObject>
#include <QString>
#include <QtQml/qqml.h>

#include "Macros/singleton.h"

class EDefaultConfig : public QObject
{
    Q_OBJECT
    SINGLETON(EDefaultConfig)

    Q_PROPERTY(QString font READ font CONSTANT)
    Q_PROPERTY(QString fontLib READ fontLib CONSTANT)
    Q_PROPERTY(QString primaryFontFamily READ primaryFontFamily CONSTANT)
    Q_PROPERTY(QString secondaryFontFamily READ secondaryFontFamily CONSTANT)

    QString font() const;
    QString fontLib() const;
    int fontPixelSize() const;
    QString primaryFontFamily() const;
    QString secondaryFontFamily() const;
    Q_INVOKABLE int iconSize(int size) const;
    Q_INVOKABLE int switchHeight(int size) const;
    Q_INVOKABLE int controlHeight(int size) const;
    Q_INVOKABLE int sliderTrackSize(int size) const;
    Q_INVOKABLE int sliderHandleSize(int size) const;
    Q_INVOKABLE int horizontalPadding(int size) const;
    Q_INVOKABLE int controlFontPixelSize(int size) const;
    Q_INVOKABLE QString iconGraphic(int iconSource) const;
    Q_INVOKABLE int selectionIndicatorSize(int size) const;

private:
    static const QString font_;
    static const QString fontLib_;
    static const int fontPixeSize_;
};
