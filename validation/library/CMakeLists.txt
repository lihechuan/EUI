if(NOT TARGET eui_gallery_tests)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Qml Quick QuickTest Network)
    add_executable(eui_gallery_tests
        gallery_main.cpp
        ../../example/example.qrc
        "${EUI_QUICK3D_SOURCE_RESOURCE}"
        ../../source/utils/ECodeTools.cpp
        ../../example/src/ExampleData.cpp
        ../../example/src/ExampleData.h
        ../../example/src/ExampleCatalog.cpp
        ../../example/src/ExampleCatalog.h
        ../../example/src/ExampleUpdater.cpp
        ../../example/src/ExampleUpdater.h
        ../../example/src/ExampleClipboard.cpp
        ../../example/src/ExampleClipboard.h
        ../../example/src/Example3DRuntime.cpp
        ../../example/src/Example3DRuntime.h
    )
    set_target_properties(eui_gallery_tests PROPERTIES AUTOMOC ON AUTORCC ON)
    target_include_directories(eui_gallery_tests PRIVATE ../../example/src)
    target_compile_definitions(eui_gallery_tests PRIVATE
        EUI_GALLERY_VERSION="${EUI_GALLERY_VERSION}"
        EUI_GALLERY_REVISION="${EUI_GALLERY_REVISION}"
        EUI_QUICK3D_PAYLOAD_DIR="${CMAKE_SOURCE_DIR}/build/quick3d-runtime"
    )
    target_link_libraries(eui_gallery_tests PRIVATE
        Qt${QT_VERSION_MAJOR}::Qml
        Qt${QT_VERSION_MAJOR}::Quick
        Qt${QT_VERSION_MAJOR}::QuickTest
        Qt${QT_VERSION_MAJOR}::Network
    )
    if(QT_VERSION_MAJOR EQUAL 5)
        find_package(Qt5 REQUIRED COMPONENTS WebEngine)
        target_link_libraries(eui_gallery_tests PRIVATE Qt5::WebEngine)
    else()
        find_package(Qt6 REQUIRED COMPONENTS WebEngineQuick)
        target_link_libraries(eui_gallery_tests PRIVATE Qt6::WebEngineQuick)
    endif()
    if(EUI_QUICK3D_AVAILABLE)
        target_sources(eui_gallery_tests PRIVATE ../../example/quick3d/littlest-tokyo/littlest-tokyo.qrc)
        target_compile_definitions(eui_gallery_tests PRIVATE EUI_HAS_QUICK3D)
        target_link_libraries(eui_gallery_tests PRIVATE Qt6::Quick3D Qt6::QuickTimeline)
    endif()
    if(EUI_BUILD_STATIC_LIB)
        target_link_libraries(eui_gallery_tests PRIVATE euiplugin)
    else()
        target_sources(eui_gallery_tests PRIVATE ../../source/enum/EIcons.h)
    endif()
    add_dependencies(eui_gallery_tests euiplugin eui_quick3d_sources)
endif()

find_package(Python3 QUIET COMPONENTS Interpreter)
if(Python3_Interpreter_FOUND)
    find_program(EUI_LIBRARY_QML_RUNNER NAMES qmltestrunner HINTS "${QT_SDK_DIR}/bin" REQUIRED NO_DEFAULT_PATH)
    set(EUI_LIBRARY_ARGUMENTS
        --runner "${EUI_LIBRARY_QML_RUNNER}"
        --qt-bin "${QT_SDK_DIR}/bin"
        --imports "$<TARGET_FILE_DIR:euiplugin>"
        --output "${CMAKE_BINARY_DIR}/library-validation"
    )
    if(TARGET eui_gallery_tests)
        list(APPEND EUI_LIBRARY_ARGUMENTS --gallery-runner "$<TARGET_FILE:eui_gallery_tests>")
    endif()
    if(WIN32 AND CMAKE_SIZEOF_VOID_P EQUAL 8 AND QT_VERSION_MAJOR EQUAL 5 AND Qt5Core_VERSION VERSION_LESS "5.15.3")
        list(APPEND EUI_LIBRARY_ARGUMENTS --qt5-interpreter)
    endif()
    add_test(NAME EUI.LibraryRegression
        COMMAND "${Python3_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/run.py" ${EUI_LIBRARY_ARGUMENTS}
    )
    set_tests_properties(EUI.LibraryRegression PROPERTIES TIMEOUT 900 RUN_SERIAL TRUE)
endif()
