cmake_minimum_required(VERSION 3.22)
project(EUIComponentValidation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(EUI_IMPORT_PATH "" CACHE PATH "Parent directory of the built EUI QML module")

if(NOT IS_DIRECTORY "${EUI_IMPORT_PATH}/EUI")
    message(FATAL_ERROR "Set EUI_IMPORT_PATH to the parent of the built EUI module directory")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test Network QuickTest)
get_target_property(EUI_QMAKE Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
get_filename_component(EUI_QT_BIN "${EUI_QMAKE}" DIRECTORY)
find_program(EUI_QML_RUNNER qmltestrunner HINTS "${EUI_QT_BIN}" REQUIRED NO_DEFAULT_PATH)

enable_testing()
file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../build/primevue-completion")
add_executable(eui_image_io_validation
    tst_image_io.cpp
    ../../source/utils/EImageIO.h
    ../../source/utils/EImageIO.cpp
)
target_include_directories(eui_image_io_validation PRIVATE ../../source)
target_link_libraries(eui_image_io_validation PRIVATE Qt${QT_VERSION_MAJOR}::Test Qt${QT_VERSION_MAJOR}::Network)
if(MSVC)
    target_compile_options(eui_image_io_validation PRIVATE /utf-8)
endif()

add_test(NAME EUI.ImageIO COMMAND eui_image_io_validation -o "${CMAKE_CURRENT_BINARY_DIR}/image-io-results.txt,txt")
add_test(NAME EUI.Components
    COMMAND "${CMAKE_COMMAND}"
        "-DEUI_QML_RUNNER=${EUI_QML_RUNNER}"
        "-DEUI_IMPORT_PATH=${EUI_IMPORT_PATH}"
        "-DEUI_SUITE_FILE=${CMAKE_CURRENT_SOURCE_DIR}/tst_primevue_components.qml"
        "-DEUI_RESULT_FILE=${CMAKE_CURRENT_BINARY_DIR}/component-results.txt"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/run_gallery_interactions.cmake"
)
add_test(NAME EUI.GalleryInteractions
    COMMAND "${CMAKE_COMMAND}"
        "-DEUI_QML_RUNNER=${EUI_QML_RUNNER}"
        "-DEUI_IMPORT_PATH=${EUI_IMPORT_PATH}"
        "-DEUI_RESULT_FILE=${CMAKE_CURRENT_BINARY_DIR}/gallery-interaction-results.txt"
        -P "${CMAKE_CURRENT_SOURCE_DIR}/run_gallery_interactions.cmake"
)
set_tests_properties(EUI.ImageIO EUI.Components EUI.GalleryInteractions PROPERTIES
    ENVIRONMENT "QT_QUICK_BACKEND=software;QT_QUICK_CONTROLS_STYLE=Basic"
    ENVIRONMENT_MODIFICATION "PATH=path_list_prepend:${EUI_QT_BIN}"
    TIMEOUT 90
)
if(WIN32)
    set_property(TEST EUI.Components EUI.GalleryInteractions APPEND PROPERTY ENVIRONMENT "QT_QPA_FONTDIR=$ENV{WINDIR}/Fonts")
endif()

if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND QT_VERSION_MAJOR EQUAL 5 AND Qt5Core_VERSION VERSION_LESS "5.15.3")
    message(STATUS "Qt 5.15.2 Win64 JIT workaround for QTBUG-89513: component validation uses the interpreter")
    set_property(TEST EUI.Components EUI.GalleryInteractions APPEND PROPERTY ENVIRONMENT "QV4_FORCE_INTERPRETER=1")
endif()
