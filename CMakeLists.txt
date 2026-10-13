cmake_minimum_required(VERSION 3.20)

project(EUI VERSION 1.0.0)

if(MSVC)
    # 使用 UTF-8 解析源文件，避免包含中文注释的文件触发 C4819
    add_compile_options(/utf-8)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
elseif(APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

if(EMSCRIPTEN)
    set(EUI_BUILD_EXAMPLES_DEFAULT OFF)
    set(EUI_BUILD_STATIC_LIB_DEFAULT ON)
    set(EUI_BUILD_TESTS_DEFAULT OFF)
else()
    set(EUI_BUILD_EXAMPLES_DEFAULT ON)
    set(EUI_BUILD_STATIC_LIB_DEFAULT OFF)
    set(EUI_BUILD_TESTS_DEFAULT ON)
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        set(EUI_BUILD_TESTS_DEFAULT OFF)
    endif()
endif()

option(EUI_BUILD_TESTS "Build and register EUI tests." ${EUI_BUILD_TESTS_DEFAULT})
option(EUI_BUILD_STATIC_LIB "Build static library." ${EUI_BUILD_STATIC_LIB_DEFAULT})
option(EUI_BUILD_EXAMPLES "Build EUI demo applications." ${EUI_BUILD_EXAMPLES_DEFAULT})

if(EMSCRIPTEN AND NOT EUI_BUILD_STATIC_LIB)
    message(FATAL_ERROR "EUI_BUILD_STATIC_LIB must be ON for WebAssembly builds")
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(QT_SDK_DIR "${Qt${QT_VERSION_MAJOR}_DIR}")
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)

set(EUI_QML_PLUGIN_DIRECTORY "" CACHE PATH "Path to EUI plugin")

add_subdirectory(source)

if(EUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

target_compile_definitions(euiplugin
    PRIVATE
    QT_MESSAGELOGCONTEXT
)

if (EUI_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

message("------------------------ EUI ------------------------")
message("Build EUI demo applications.: ${EUI_BUILD_EXAMPLES}")
message("Build static library.: ${EUI_BUILD_STATIC_LIB}")
message("Build EUI tests.: ${EUI_BUILD_TESTS}")
message("Path to EUI plugin.: ${EUI_QML_PLUGIN_DIRECTORY}")
