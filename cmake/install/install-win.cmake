# CMake install prefix
string(LENGTH ${CMAKE_BINARY_DIR} CMAKE_BINARY_DIR_LENGTH)
string(SUBSTRING ${CMAKE_INSTALL_PREFIX} 0 ${CMAKE_BINARY_DIR_LENGTH} CMAKE_INSTALL_PREFIX_TMP)
if (NOT ${CMAKE_INSTALL_PREFIX_TMP} STREQUAL ${CMAKE_BINARY_DIR})
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "CMAKE_INSTALL_PREFIX" FORCE)
endif()
message("Install - CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

# Find windeployqt tool
message("Install - CONAN_QT_ROOT: " ${CONAN_QT_ROOT})
file(GLOB_RECURSE WINDEPLOYQT_TOOL ${CONAN_QT_ROOT}/bin/windeployqt.exe)
message("Install - WINDEPLOYQT_TOOL: " ${WINDEPLOYQT_TOOL})

# Determining windeployqt working directory
get_filename_component(WINDEPLOYQT_TOOL_WORKING_DIRECTORY ${WINDEPLOYQT_TOOL} DIRECTORY)
message("Install - WINDEPLOYQT_TOOL_WORKING_DIRECTORY: " ${WINDEPLOYQT_TOOL_WORKING_DIRECTORY})

# Install: binary
install(
    TARGETS
        ${PROJECT_NAME}
    CONFIGURATIONS
        ${CMAKE_BUILD_TYPE}
    RUNTIME
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)

# Install: PDB
install(
    FILES
        $<TARGET_PDB_FILE:${PROJECT_NAME}>
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
    OPTIONAL
)

# Install: windeployqt
install(
    CODE
    "execute_process(
        COMMAND
            ${WINDEPLOYQT_TOOL} --verbose=2 --compiler-runtime --pdb ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}.exe
        WORKING_DIRECTORY
            ${WINDEPLOYQT_TOOL_WORKING_DIRECTORY}
        RESULT_VARIABLE
            WINDEPLOYQT_TOOL_RESULT
        OUTPUT_VARIABLE
            WINDEPLOYQT_TOOL_OUTPUT
    )
    message(\"Install - WINDEPLOYQT_TOOL_RESULT: \" \${WINDEPLOYQT_TOOL_RESULT})
    message(\"Install - WINDEPLOYQT_TOOL_OUTPUT: \n\" \${WINDEPLOYQT_TOOL_OUTPUT})"
)

# Install: OpenSSL
message("Install - CONAN_OPENSSL_ROOT: " ${CONAN_OPENSSL_ROOT})
file(
    GLOB_RECURSE
        OPEN_SLL_DLL
    ${CONAN_OPENSSL_ROOT}/*.dll
)

install(
    FILES
        ${OPEN_SLL_DLL}
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)

# Install: config.ini
install(
    FILES
        ${CMAKE_SOURCE_DIR}/config.ini
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)
