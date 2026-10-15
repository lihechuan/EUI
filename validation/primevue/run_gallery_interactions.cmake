if(NOT EUI_SUITE_FILE)
    set(EUI_SUITE_FILE "${CMAKE_CURRENT_LIST_DIR}/tst_gallery_interactions.qml")
endif()
execute_process(
    COMMAND "${EUI_QML_RUNNER}" -input "${EUI_SUITE_FILE}"
        -import "${EUI_IMPORT_PATH}" -platform offscreen -o "${EUI_RESULT_FILE},txt"
    RESULT_VARIABLE result
)
file(READ "${EUI_RESULT_FILE}" output)
message("${output}")
if(NOT result EQUAL 0 OR output MATCHES "Binding loop|TypeError|ReferenceError|SyntaxError|RangeError|Unable to assign|Cannot assign|Uncaught exception|Injection of parameters into signal handlers")
    message(FATAL_ERROR "QML checks reported a failure or runtime error")
endif()
