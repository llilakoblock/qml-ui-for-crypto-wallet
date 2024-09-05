file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/*.h)

add_library(core_test STATIC ${SOURCES} ${HEADERS})

target_link_libraries(
    core_test
    PUBLIC
        testutils
)
