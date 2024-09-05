# Check version
cmake_minimum_required(VERSION 3.6.3 FATAL_ERROR)

# IDE settings
include(${CMAKE_SOURCE_DIR}/cmake/ide/ide.cmake)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)

# Conan setup
if(WIN32)
    set(CONAN_INSTALL_ARGS "--build=missing;--deployer-package=xwallet/1.0;--deployer-folder=${CMAKE_INSTALL_PREFIX}")
endif()

# Conan package provider
set(CMAKE_PROJECT_TOP_LEVEL_INCLUDES cmake/conan/conan-provider.cmake)

# Disable in-source builds to prevent source tree corruption
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "FATAL: In-source builds are not allowed. You should create a separate directory for build files.")
endif()

project(freedom
    VERSION
        0.3.1
    DESCRIPTION
        "Freedom DEX desktop client https://bitcoinatom.io."
    LANGUAGES
        CXX
        C
)

link_directories("/usr/local/opt/icu4c/lib")

# Set output dirs
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
#     string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
#     set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
#     set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
#     set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
# endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

# Conan install
# include(${CMAKE_SOURCE_DIR}/cmake/conan/conan-install.cmake)

# Setup C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_policy(SET CMP0074 NEW)
cmake_policy(SET CMP0048 NEW)

# Set compiler options
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options("-Wno-unused-function")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-Wno-unused-function")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_definitions(_HAS_STD_BYTE=1)
    add_compile_options("/MP" "/wd5040")
endif()

# QT link and setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum OS X deployment version")

set(Boost_USE_STATIC_LIBS ON)

option(BUILD_ORDERSERVER "Flag to enable/disable building orderserver" ON)

# Find packages
find_package(Boost 1.76.0
    REQUIRED COMPONENTS
        headers
        atomic
        chrono
        date_time
        filesystem
        iostreams
        locale
        log
        program_options
        regex
        system
        thread
        # test
)

find_package(Qt6
    REQUIRED COMPONENTS
        Core
        Gui
        Qml
        Sql
        Concurrent
        WebSockets
        WebEngineCore
        HttpServer
)

find_package(nlohmann_json REQUIRED)
find_package(plog REQUIRED)
find_package(Protobuf REQUIRED)
find_package(OpenSSL REQUIRED)

set(QT_QML_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/qml)

# Setup plog
add_compile_definitions(PLOG_OMIT_LOG_DEFINES)

# Add miniupnpc subdirectory
SET(UPNPC_BUILD_STATIC ON CACHE BOOL "" FORCE)
SET(UPNPC_BUILD_SHARED OFF CACHE BOOL "" FORCE)
SET(UPNPC_BUILD_TESTS OFF CACHE BOOL "" FORCE)
SET(UPNPC_BUILD_SAMPLE OFF CACHE BOOL "" FORCE)
SET(UPNPC_NO_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/miniupnp/miniupnpc)

# Add natpmp subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/libnatpmp)

# Add libbitcoin-system subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/libs/libbitcoin-system/builds/cmake)
target_compile_definitions(bitcoin-system
    PUBLIC
        BOOST_ALL_NO_LIB
        BC_STATIC
        WITH_ICU
        WIN32_LEAN_AND_MEAN
        NOMINMAX
        NDEBUG
        _WIN32_WINNT=0x0600
        _HAS_STD_BYTE
)

# Add api subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/api)

# Add common subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/common)

# Add config subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/config)

# Add core subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/src/core)

# Add orderbook subdirectory

add_subdirectory(${CMAKE_SOURCE_DIR}/src/orderbook)

# ##############################
# # Add portforwarding subdirectory
# add_subdirectory(${CMAKE_SOURCE_DIR}/src/portforwarding)

add_subdirectory(application)

# Add test subdirectory
# add_subdirectory(${CMAKE_SOURCE_DIR}/test)
