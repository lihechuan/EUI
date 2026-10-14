cmake_minimum_required(VERSION 3.20)

project(euiplugin VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if (EUI_BUILD_STATIC_LIB)
    add_definitions(-DEUI_BUILD_STATIC_LIB -DQT_STATICPLUGIN)
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick Qml Widgets Network Concurrent PrintSupport)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick Qml Widgets Network Concurrent PrintSupport)

set(EUI_QML_IMPORTS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Qt5/imports")
set(EUI_RESOURCE_FILE "${EUI_QML_IMPORTS_DIRECTORY}/eui.qrc")
if(QT_VERSION_MAJOR EQUAL 6)
    find_package(Python3 REQUIRED COMPONENTS Interpreter)
    find_package(Qt6 6.5 REQUIRED COMPONENTS Core5Compat ShaderTools)
    file(GLOB_RECURSE EUI_QML_INPUTS CONFIGURE_DEPENDS "${EUI_QML_IMPORTS_DIRECTORY}/*")
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
        ${EUI_QML_INPUTS} "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/prepare_qt6.py"
    )
    set(EUI_QT6_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qt6")
    execute_process(
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/prepare_qt6.py"
            "${CMAKE_CURRENT_SOURCE_DIR}/Qt5" "${EUI_QT6_DIRECTORY}"
        COMMAND_ERROR_IS_FATAL ANY
    )
    set(EUI_RESOURCE_FILE "${EUI_QT6_DIRECTORY}/eui.qrc")
    set(EUI_QML_IMPORTS_DIRECTORY "${EUI_QT6_DIRECTORY}/imports")
endif()

file(GLOB_RECURSE CPP_FILES CONFIGURE_DEPENDS *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)

if (EUI_BUILD_STATIC_LIB)
    set(LIB_TYPE "STATIC")
else()
    set(LIB_TYPE "SHARED")
endif()

if (EUI_BUILD_STATIC_LIB)
    set(PLUGIN_TARGET_NAME "")
else()
    set(PLUGIN_TARGET_NAME ${PROJECT_NAME})
endif()

set(EUI_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})

list(APPEND CMAKE_MODULE_PATH ${EUI_DIRECTORY}/.cmake)
include(QmlPlugin)
add_qmlplugin(${PROJECT_NAME}
    URI "EUI"
    VERSION 1.0
    SOURCES ${sources_files} ${EUI_VERSION_RC_PATH} ${EUI_RESOURCE_FILE} Charts.qrc
    QMLFILES ${qml_files}
    QMLDIR imports/EUI
    BINARY_DIR ${EUI_QML_PLUGIN_DIRECTORY}
    LIBTYPE ${LIB_TYPE}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_add_shaders(${PROJECT_NAME} eui_shaders
        PREFIX "/qt/qml/EUI/Shaders"
        BASE "${EUI_QT6_DIRECTORY}/shaders"
        FILES
            "${EUI_QT6_DIRECTORY}/shaders/motion-blur.frag"
            "${EUI_QT6_DIRECTORY}/shaders/theme-reveal.frag"
            "${EUI_QT6_DIRECTORY}/shaders/theme-switch.frag"
    )
endif()

if(MINGW)
    set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "d")
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Concurrent
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::PrintSupport
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::GuiPrivate Qt${QT_VERSION_MAJOR}::QmlPrivate)

find_program(QML_PLUGIN_DUMP NAMES qmlplugindump)
if(QML_PLUGIN_DUMP AND QT_VERSION_MAJOR EQUAL 5 AND NOT EUI_BUILD_STATIC_LIB)
    set(EUI_QMLTYPES_FILE ${CMAKE_CURRENT_SOURCE_DIR}/Qt5/imports/EUI/plugins.qmltypes)
    add_custom_target(Script-Generate-QmlTypes ALL
        COMMAND ${CMAKE_COMMAND}
            -DQML_PLUGIN_DUMP=${QML_PLUGIN_DUMP}
            -DQML_IMPORT_PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>
            -DOUTPUT_FILE=${EUI_QMLTYPES_FILE}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/.cmake/GenerateQmlTypes.cmake
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${EUI_QMLTYPES_FILE}
            $<TARGET_FILE_DIR:${PROJECT_NAME}>/EUI/plugins.qmltypes
        DEPENDS ${PROJECT_NAME}
        COMMENT "Generating EUI/plugins.qmltypes"
        VERBATIM
    )
endif()

# CMake install 与日常构建解耦；安装时再将库和公开 QML 文件写入前缀目录。
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION imports/EUI
    LIBRARY DESTINATION imports/EUI
    ARCHIVE DESTINATION lib
)
install(DIRECTORY ${EUI_QML_IMPORTS_DIRECTORY}/EUI/
    DESTINATION imports/EUI
)
