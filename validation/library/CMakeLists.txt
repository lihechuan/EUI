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
