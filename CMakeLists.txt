cmake_minimum_required(VERSION 3.20)

project(EUI VERSION 1.3.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EUI_GALLERY_VERSION "${PROJECT_VERSION}")

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

set(EUI_QUICK3D_AVAILABLE OFF)
if(QT_VERSION_MAJOR EQUAL 6 AND Qt6_VERSION VERSION_GREATER_EQUAL 6.8)
    find_package(Qt6 QUIET COMPONENTS Quick3D QuickTimeline)
    if(TARGET Qt6::Quick3D AND TARGET Qt6::QuickTimeline)
        set(EUI_QUICK3D_AVAILABLE ON)
    endif()
endif()

if(EUI_BUILD_TESTS OR EUI_BUILD_EXAMPLES)
    file(GLOB_RECURSE EUI_GALLERY_SOURCES CONFIGURE_DEPENDS RELATIVE "${CMAKE_SOURCE_DIR}"
        example/*.h example/*.cpp example/*.qml example/*.js example/*.json example/*.qrc
        source/*.h source/*.cpp source/*.qml source/*.js source/*.json source/*.qrc
    )
    set(EUI_GALLERY_SIGNATURE "${EUI_GALLERY_VERSION}\n")
    foreach(file IN LISTS EUI_GALLERY_SOURCES)
        file(SHA256 "${CMAKE_SOURCE_DIR}/${file}" digest)
        string(APPEND EUI_GALLERY_SIGNATURE "${file}:${digest}\n")
    endforeach()
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${EUI_GALLERY_SOURCES})
    string(SHA256 EUI_GALLERY_REVISION "${EUI_GALLERY_SIGNATURE}")
    file(CONFIGURE OUTPUT "${CMAKE_BINARY_DIR}/gallery-build.json"
        CONTENT "{\"galleryVersion\":\"${EUI_GALLERY_VERSION}\",\"galleryRevision\":\"${EUI_GALLERY_REVISION}\"}\n"
    )
    set(EUI_QUICK3D_SOURCE_RESOURCE "${CMAKE_CURRENT_BINARY_DIR}/quick3d-source/quick3d-source.qrc")
    set(EUI_QUICK3D_SOURCE_CONTENT "<RCC>\n    <qresource prefix=\"/example/quick3d-source\">\n")
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/quick3d-source/assets")
    add_custom_target(eui_quick3d_sources)
    foreach(file TokyoView.qml TokyoScene.qml assets/LittlestTokyo.qml)
        add_custom_command(TARGET eui_quick3d_sources POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_CURRENT_SOURCE_DIR}/example/quick3d/littlest-tokyo/${file}"
                "${CMAKE_CURRENT_BINARY_DIR}/quick3d-source/${file}.txt"
            BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/quick3d-source/${file}.txt"
            VERBATIM
        )
        string(APPEND EUI_QUICK3D_SOURCE_CONTENT "        <file>${file}.txt</file>\n")
    endforeach()
    string(APPEND EUI_QUICK3D_SOURCE_CONTENT "    </qresource>\n</RCC>\n")
    file(WRITE "${EUI_QUICK3D_SOURCE_RESOURCE}" "${EUI_QUICK3D_SOURCE_CONTENT}")
endif()

set(QT_SDK_DIR "${Qt${QT_VERSION_MAJOR}_DIR}")
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)
cmake_path(GET QT_SDK_DIR PARENT_PATH QT_SDK_DIR)

set(EUI_QML_PLUGIN_DIRECTORY "" CACHE PATH "Path to EUI plugin")

add_subdirectory(source)

if(EUI_BUILD_TESTS)
    enable_testing()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
        add_subdirectory(tests)
    endif()
    add_subdirectory(validation/library)
endif()

target_compile_definitions(euiplugin
    PRIVATE
    QT_MESSAGELOGCONTEXT
)

if (EUI_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(WIN32 AND QT_VERSION_MAJOR EQUAL 5 AND (EUI_BUILD_EXAMPLES OR EUI_BUILD_TESTS))
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(EUI_TLS_ARCH x64)
        set(EUI_TLS_SUFFIX -x64)
    else()
        set(EUI_TLS_ARCH x86)
        set(EUI_TLS_SUFFIX "")
    endif()
    set(EUI_TLS_RUNTIME_DIR "")
    foreach(directory
        "${QT_SDK_DIR}/bin"
        "${QT_SDK_DIR}/../../Tools/OpenSSL/Win_${EUI_TLS_ARCH}/bin"
        "${CMAKE_CURRENT_SOURCE_DIR}/build/packaging-tools/openssl/openssl-1.1/${EUI_TLS_ARCH}/bin"
    )
        if(EXISTS "${directory}/libssl-1_1${EUI_TLS_SUFFIX}.dll"
            AND EXISTS "${directory}/libcrypto-1_1${EUI_TLS_SUFFIX}.dll")
            set(EUI_TLS_RUNTIME_DIR "${directory}")
            break()
        endif()
    endforeach()
    if(EUI_TLS_RUNTIME_DIR)
        add_custom_target(deploy_gallery_tls ALL)
        foreach(target example eui_update_tests eui_gallery_tests)
            if(TARGET ${target})
                add_custom_command(TARGET deploy_gallery_tls POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:${target}>"
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${EUI_TLS_RUNTIME_DIR}/libssl-1_1${EUI_TLS_SUFFIX}.dll"
                        "${EUI_TLS_RUNTIME_DIR}/libcrypto-1_1${EUI_TLS_SUFFIX}.dll"
                        "$<TARGET_FILE_DIR:${target}>"
                    VERBATIM
                )
                add_dependencies(${target} deploy_gallery_tls)
            endif()
        endforeach()
    else()
        message(WARNING "OpenSSL 1.1 ${EUI_TLS_ARCH} runtime DLLs were not found. Qt 5 builds using OpenSSL need these DLLs for update checks.")
    endif()
endif()

message("------------------------ EUI ------------------------")
message("Build EUI demo applications.: ${EUI_BUILD_EXAMPLES}")
message("Build static library.: ${EUI_BUILD_STATIC_LIB}")
message("Build EUI tests.: ${EUI_BUILD_TESTS}")
message("Quick 3D example.: ${EUI_QUICK3D_AVAILABLE}")
message("Path to EUI plugin.: ${EUI_QML_PLUGIN_DIRECTORY}")
