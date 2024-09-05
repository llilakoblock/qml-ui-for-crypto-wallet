add_custom_target(
    clean-all
    COMMAND
        ${CMAKE_COMMAND}
            -D BINARY_DIR=${CMAKE_BINARY_DIR}
            -D RUNTIME_OUTPUT_DIRECTORY=${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            -P ${CMAKE_CURRENT_LIST_DIR}/clean-all.cmake
)
