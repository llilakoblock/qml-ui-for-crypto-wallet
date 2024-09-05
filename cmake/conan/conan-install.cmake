# Include conan.cmake
include(${CMAKE_SOURCE_DIR}/libs/cmake-conan/conan.cmake)

# Determine OS
if (WIN32)
    include(${CMAKE_CURRENT_LIST_DIR}/conanfile-win.cmake)
elseif (APPLE)
    message(FATAL_ERROR "Not implemented!")
elseif (UNIX AND NOT APPLE)
    include(${CMAKE_CURRENT_LIST_DIR}/conanfile-linux.cmake)
endif()

# Detect settings
conan_cmake_autodetect(CONAN_AUTODETECT_SETTINGS)
message("CONAN_AUTODETECT_SETTINGS: ${CONAN_AUTODETECT_SETTINGS}")

# Install
conan_cmake_install(
    PATH_OR_REFERENCE
        .
    BUILD
        missing
    REMOTE
        conancenter
    SETTINGS
        ${CONAN_AUTODETECT_SETTINGS}
    INSTALL_FOLDER
        ${CMAKE_BINARY_DIR}/conan-dependencies
)

# Include conan paths
include(${CMAKE_BINARY_DIR}/conan-dependencies/conan_paths.cmake)

