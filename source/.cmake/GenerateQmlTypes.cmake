if(NOT QML_PLUGIN_DUMP OR NOT QML_IMPORT_PATH OR NOT OUTPUT_FILE)
    message(FATAL_ERROR "QML_PLUGIN_DUMP, QML_IMPORT_PATH and OUTPUT_FILE are required")
endif()

execute_process(
    COMMAND "${QML_PLUGIN_DUMP}" -nonrelocatable EUI 1.0 "${QML_IMPORT_PATH}"
    RESULT_VARIABLE dump_result
    OUTPUT_VARIABLE dump_output
    ERROR_VARIABLE dump_diagnostics
    TIMEOUT 45
)

if(NOT dump_result EQUAL 0)
    message(FATAL_ERROR "qmlplugindump failed (${dump_result}):\n${dump_diagnostics}")
endif()

if(NOT dump_output MATCHES "Module[ \t\r\n]*\\{")
    message(FATAL_ERROR "qmlplugindump returned no QML tooling module:\n${dump_diagnostics}")
endif()

file(WRITE "${OUTPUT_FILE}" "${dump_output}")
