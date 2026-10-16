QT += quick qml quickcontrols2 qml-private gui-private network widgets printsupport
CONFIG += c++17
CONFIG -= qtquickcompiler
TARGET = eui-playground
INCLUDEPATH += ../../source $$OUT_PWD
SOURCES += main.cpp \
    ../../source/config/ELocale.cpp \
    ../../source/utils/ColorUtils.cpp \
    ../../source/utils/EDataUtils.cpp \
    ../../source/config/EDefaultConfig.cpp \
    ../../source/style/theme/ETheme.cpp \
    ../../source/state/EComponentStateObject.cpp \
    ../../source/component/EAdvancedRectangle.cpp \
    $$files(../../source/style/palettes/*.cpp)
SOURCES += ../../source/layout/ELayout.cpp \
    ../../example/src/ExampleData.cpp \
    ../../source/utils/LayoutUtils.cpp \
    ../../source/utils/RandomUtils.cpp \
    ../../source/utils/EAnimation.cpp \
    ../../source/utils/ECodeTools.cpp \
    ../../source/utils/EImageIO.cpp \
    ../../source/utils/ETextDocument.cpp \
    ../../source/utils/ETableIO.cpp \
    ../../source/utils/ETableWorkbook.cpp \
    ../../source/config/EValueUtils.cpp \
    ../../source/component/EChartRenderer.cpp \
    ../../source/component/EMasonryLayout.cpp \
    ../../source/component/EQrCodeRenderer.cpp \
    ../../source/component/EShadowRenderer.cpp \
    ../../source/component/ECaptchaRenderer.cpp \
    ../../source/component/EWatermarkRenderer.cpp \
    ../../source/component/EBackdropRenderer.cpp \
    ../../source/component/EWindowActions.cpp \
    ../../source/utils/EImageEffects.cpp \
    ../../source/utils/EFilePickerUtils.cpp \
    ../../source/thirdparty/qrcodegen/qrcodegen.cpp
HEADERS += ../../source/enum/EIcons.h \
    ../../source/enum/EStyle.h \
    ../../source/enum/EPlacement.h \
    ../../source/enum/EValidationState.h \
    ../../source/config/ELocale.h \
    ../../source/utils/ColorUtils.h \
    ../../source/utils/EDataUtils.h \
    ../../source/config/EDefaultConfig.h \
    ../../source/style/theme/ETheme.h \
    ../../source/state/EComponentStateObject.h \
    ../../source/component/EAdvancedRectangle.h \
    $$files(../../source/style/palettes/*.h)
HEADERS += ../../source/enum/EOrientation.h \
    ../../example/src/ExampleData.h \
    ../../source/layout/ELayout.h \
    ../../source/utils/RandomUtils.h \
    ../../source/utils/EAnimation.h \
    ../../source/utils/ECodeTools.h \
    ../../source/utils/EImageIO.h \
    ../../source/utils/ETextDocument.h \
    ../../source/utils/ETableIO.h \
    ../../source/config/EValueUtils.h \
    ../../source/component/EChartRenderer.h \
    ../../source/component/EMasonryLayout.h \
    ../../source/component/EQrCodeRenderer.h \
    ../../source/component/EShadowRenderer.h \
    ../../source/component/ECaptchaRenderer.h \
    ../../source/component/EWatermarkRenderer.h \
    ../../source/component/EBackdropRenderer.h \
    ../../source/component/EWindowActions.h \
    ../../source/utils/EFilePickerUtils.h
RESOURCES += $$OUT_PWD/resources.qrc
QML_IMPORT_PATH += $$OUT_PWD/imports
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -Os
QMAKE_LFLAGS_RELEASE -= -O2
QMAKE_LFLAGS_RELEASE += -Os
QMAKE_LFLAGS += -s TOTAL_MEMORY=33554432
