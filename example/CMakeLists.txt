cmake_minimum_required(VERSION 3.20)

project(example VERSION 1.0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
endif()

set(APPLICATION_DIR_PATH ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Quick Gui Qml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Quick Gui Qml)
if(QT_VERSION_MAJOR EQUAL 5)
    find_package(Qt5 REQUIRED COMPONENTS WebEngine WebChannel)
else()
    find_package(Qt6 REQUIRED COMPONENTS WebEngineQuick WebChannel)
endif()

file(GLOB_RECURSE CPP_FILES *.cpp *.h)
foreach(filepath ${CPP_FILES})
    string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" filename ${filepath})
    list(APPEND sources_files ${filename})
endforeach(filepath)

if (WIN32)
    set(EUI_APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../assets/eui-icon.ico")
    configure_file(app-icon.rc.in app-icon.rc @ONLY)
    set_source_files_properties("${CMAKE_CURRENT_BINARY_DIR}/app-icon.rc" PROPERTIES
        OBJECT_DEPENDS "${EUI_APPLICATION_ICON}"
    )
    add_executable(example
        ${sources_files}
        "${CMAKE_CURRENT_BINARY_DIR}/app-icon.rc"
        ${EXAMPLE_VERSION_RC_PATH}
    )
    set_target_properties(example PROPERTIES WIN32_EXECUTABLE TRUE)
else ()
    add_executable(example
        ${sources_files}
    )
endif ()

if(APPLE)
    set(EUI_APPLICATION_ICON "${CMAKE_CURRENT_SOURCE_DIR}/../assets/eui-icon.icns")
    set_source_files_properties("${EUI_APPLICATION_ICON}" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    target_sources(example PRIVATE "${EUI_APPLICATION_ICON}")
    set_target_properties(example PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "EUI Gallery"
        MACOSX_BUNDLE_GUI_IDENTIFIER "local.eui.gallery"
        MACOSX_BUNDLE_ICON_FILE eui-icon.icns
    )
endif()

target_include_directories(example PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(example PRIVATE
    EUI_GALLERY_VERSION="${EUI_GALLERY_VERSION}"
    EUI_GALLERY_REVISION="${EUI_GALLERY_REVISION}"
    EUI_QUICK3D_PAYLOAD_DIR="${CMAKE_SOURCE_DIR}/build/quick3d-runtime"
)

target_sources(example PRIVATE example.qrc "${EUI_QUICK3D_SOURCE_RESOURCE}" ../source/utils/ECodeTools.cpp)
if(EUI_QUICK3D_AVAILABLE)
    target_sources(example PRIVATE quick3d/littlest-tokyo/littlest-tokyo.qrc)
    target_compile_definitions(example PRIVATE EUI_HAS_QUICK3D)
    target_link_libraries(example PRIVATE Qt6::Quick3D Qt6::QuickTimeline)
endif()
if(NOT EUI_BUILD_STATIC_LIB)
    target_sources(example PRIVATE ../source/enum/EIcons.h)
endif()

target_link_libraries(example PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network
)
if(NOT APPLE OR EUI_BUILD_STATIC_LIB)
    target_link_libraries(example PRIVATE euiplugin)
endif()
if(QT_VERSION_MAJOR EQUAL 5)
    target_link_libraries(example PRIVATE Qt5::WebEngine)
else()
    target_link_libraries(example PRIVATE Qt6::WebEngineQuick)
endif()

add_custom_target(deploy_eui_to_example ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:euiplugin>/EUI
        $<TARGET_FILE_DIR:example>/EUI
    DEPENDS euiplugin
    COMMENT "Deploying the current EUI QML module beside the example executable"
)

add_custom_target(deploy_eui_to_qt ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        $<TARGET_FILE_DIR:euiplugin>/EUI
        ${QT_SDK_DIR}/qml/EUI
    DEPENDS euiplugin
    COMMENT "Deploying the current EUI QML module to ${QT_SDK_DIR}/qml/EUI"
)

if (TARGET Script-Generate-QmlTypes)
    add_dependencies(deploy_eui_to_qt Script-Generate-QmlTypes)
    add_dependencies(deploy_eui_to_example Script-Generate-QmlTypes)
endif()

add_dependencies(example eui_quick3d_sources deploy_eui_to_qt deploy_eui_to_example)

if(WIN32 AND QT_VERSION_MAJOR EQUAL 5 AND CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(EUI_QUICK3D_BUILD_DIRECTORY "${CMAKE_SOURCE_DIR}/build/quick3d-gallery-runtime")
    set(EUI_QUICK3D_RUNTIME_DIRECTORY "${CMAKE_SOURCE_DIR}/build/quick3d-runtime")
    if(EXISTS "${EUI_QUICK3D_BUILD_DIRECTORY}/CMakeCache.txt"
        AND EXISTS "${EUI_QUICK3D_RUNTIME_DIRECTORY}/manifest.json")
        file(STRINGS "${EUI_QUICK3D_BUILD_DIRECTORY}/CMakeCache.txt" EUI_QUICK3D_QT_DIRECTORY
            REGEX "^Qt6_DIR:(PATH|UNINITIALIZED)=")
        string(REGEX REPLACE "^[^=]+=" "" EUI_QUICK3D_QT_DIRECTORY "${EUI_QUICK3D_QT_DIRECTORY}")
        foreach(level RANGE 1 3)
            cmake_path(GET EUI_QUICK3D_QT_DIRECTORY PARENT_PATH EUI_QUICK3D_QT_DIRECTORY)
        endforeach()
        if(EXISTS "${EUI_QUICK3D_QT_DIRECTORY}/bin/windeployqt.exe")
            add_custom_target(sync_quick3d_runtime
                COMMAND powershell.exe -NoProfile -ExecutionPolicy Bypass
                    -File "${CMAKE_SOURCE_DIR}/packaging/prepare-quick3d-runtime.ps1"
                    -QtDir "${EUI_QUICK3D_QT_DIRECTORY}" -BuildDir "${EUI_QUICK3D_BUILD_DIRECTORY}"
                    -OutputDir "${EUI_QUICK3D_RUNTIME_DIRECTORY}" -Refresh
                    -GalleryBuildFile "${CMAKE_BINARY_DIR}/gallery-build.json" -CMakeProgram "${CMAKE_COMMAND}"
                COMMENT "Synchronizing the prepared Qt 6 runtime with the current Gallery sources"
                VERBATIM
            )
            add_dependencies(example sync_quick3d_runtime)
        endif()
    endif()
endif()
