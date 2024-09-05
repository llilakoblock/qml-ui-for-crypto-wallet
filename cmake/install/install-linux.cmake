# CMake install prefix
string(LENGTH ${CMAKE_BINARY_DIR} CMAKE_BINARY_DIR_LENGTH)
string(SUBSTRING ${CMAKE_INSTALL_PREFIX} 0 ${CMAKE_BINARY_DIR_LENGTH} CMAKE_INSTALL_PREFIX_TMP)
if (NOT ${CMAKE_INSTALL_PREFIX_TMP} STREQUAL ${CMAKE_BINARY_DIR})
    set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install CACHE STRING "CMAKE_INSTALL_PREFIX" FORCE)
endif()
message("Install - CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})

# Find Qt
message("Install - CONAN_QT_ROOT: " ${CONAN_QT_ROOT})

# Install: copy binary
install(
    TARGETS
        ${PROJECT_NAME}
    CONFIGURATIONS
        ${CMAKE_BUILD_TYPE}
    RUNTIME
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)

# Install: copy config.ini
install(
    FILES
        ${CMAKE_SOURCE_DIR}/config.ini
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}
)

# Install: create AppDir folder and copy binary
install(
    FILES
        ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}
    PERMISSIONS
        OWNER_EXECUTE
        OWNER_WRITE
        OWNER_READ
        GROUP_READ
        GROUP_EXECUTE
        WORLD_READ
        WORLD_EXECUTE
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/AppDir
)

# Install: copy and configure appimagebuilder.yml
install(
    CODE "
    set(AIB_PATH ${CMAKE_INSTALL_PREFIX}/AppDir)
    set(AIB_APP_INFO_ID ${CMAKE_PROJECT_NAME})
    set(AIB_APP_INFO_NAME ${CMAKE_PROJECT_NAME})
    set(AIB_APP_INFO_EXEC ${CMAKE_PROJECT_NAME})
    set(AIB_APP_INFO_VERSION ${CMAKE_PROJECT_VERSION})
    set(AIB_APP_INFO_EXEC_ARGS \"$@\")
    set(AIB_APP_INFO_ICON \"atom\")
    set(AIB_INCLUDE_QT_BIN_PATH ${CONAN_QT_ROOT}/bin)
    set(AIB_INCLUDE_QT_LIB_PATH ${CONAN_QT_ROOT}/lib)
    set(AIB_INCLUDE_SYS_LIB_PATH \"/usr/lib/x86_64-linux-gnu\")

    configure_file(${CMAKE_SOURCE_DIR}/cmake/install/appimagebuilder.yml ${CMAKE_INSTALL_PREFIX}/appimagebuilder.yml)

    unset(AIB_PATH)
    unset(AIB_APP_INFO_ID)
    unset(AIB_APP_INFO_NAME)
    unset(AIB_APP_INFO_VERSION)
    unset(AIB_APP_INFO_EXEC)
    unset(AIB_APP_INFO_EXEC_ARGS)
    unset(AIB_APP_INFO_ICON)
    unset(AIB_INCLUDE_QT_BIN_PATH)
    unset(AIB_INCLUDE_QT_LIB_PATH)
    unset(AIB_INCLUDE_SYS_LIB_PATH)
    "
)

# Install: copy atom.png
install(
    FILES
        ${CMAKE_SOURCE_DIR}/res/atom.png
    DESTINATION
        ${CMAKE_INSTALL_PREFIX}/AppDir/usr/share/icons/48x48
)

# Install: run appimage-builder
install(
    CODE
    "execute_process(
        COMMAND
            appimage-builder --skip-tests --recipe appimagebuilder.yml
        WORKING_DIRECTORY
            ${CMAKE_INSTALL_PREFIX}
        RESULT_VARIABLE
            APP_IMAGE_BUILDER_RESULT
        OUTPUT_VARIABLE
            APP_IMAGE_BUILDER_OUTPUT
    )
    message(\"Install - APP_IMAGE_BUILDER_RESULT: \" \${APP_IMAGE_BUILDER_RESULT})"
)
