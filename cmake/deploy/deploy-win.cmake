# Find windeployqt tool
message("Deploy - CONAN_QT_ROOT: " ${CONAN_QT_ROOT})
file(GLOB_RECURSE WINDEPLOYQT_TOOL ${CONAN_QT_ROOT}/bin/windeployqt.exe)
message("Deploy - WINDEPLOYQT_TOOL: " ${WINDEPLOYQT_TOOL})

# Determining windeployqt working directory
get_filename_component(WINDEPLOYQT_TOOL_WORKING_DIRECTORY ${WINDEPLOYQT_TOOL} DIRECTORY)
message("Deploy - WINDEPLOYQT_TOOL_WORKING_DIRECTORY: " ${WINDEPLOYQT_TOOL_WORKING_DIRECTORY})

# Post build command: windeployqt
add_custom_command(
    TARGET
        ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        ${WINDEPLOYQT_TOOL} --verbose=2 --compiler-runtime --pdb ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME}.exe
    WORKING_DIRECTORY
        ${WINDEPLOYQT_TOOL_WORKING_DIRECTORY}
)

# Post build command: deploy OpenSSL
message("Deploy - CONAN_OPENSSL_ROOT: " ${CONAN_OPENSSL_ROOT})
file(
    GLOB_RECURSE
        OPEN_SLL_DLL
    ${CONAN_OPENSSL_ROOT}/*.dll
)

add_custom_command(
    TARGET
        ${PROJECT_NAME}
    POST_BUILD
    COMMAND
        COMMAND ${CMAKE_COMMAND} -E copy ${OPEN_SLL_DLL} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
    WORKING_DIRECTORY
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
)
