# Find packages
find_package(GTest REQUIRED)

# Add testing modules
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/testutils)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/core)

# Set sources
set(SOURCES
    main.cpp)

# Set test libs
set(LIBS_TO_LINK
    testutils
    core_test)

# Add general test binary
add_executable(
    tests
    ${SOURCES}
)

# Target link libraries
target_link_libraries(
    tests
    PRIVATE
        ${LIBS_TO_LINK}
)

# Set target properties
set_target_properties(
    tests
    PROPERTIES
        LINKER_LANGUAGE CXX
)

# Set linker options
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Not implemented!")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(ARCHIVE_LIST)
    foreach(LIB ${LIBS_TO_LINK})
        list(APPEND ARCHIVE_LIST ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/lib${LIB}.a)
    endforeach(LIB LIBS_TO_LINK)
    target_link_options(tests PRIVATE -Wl,--whole-archive ${ARCHIVE_LIST} -Wl,--no-whole-archive)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    foreach(LIB ${LIBS_TO_LINK})
        target_link_options(tests PRIVATE /WHOLEARCHIVE:${LIB})
    endforeach(LIB LIBS_TO_LINK)
endif()

# Testing
enable_testing()
