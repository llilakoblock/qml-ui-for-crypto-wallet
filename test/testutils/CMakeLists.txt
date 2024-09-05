file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_LIST_DIR}/private/*.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_LIST_DIR}/public/*.h)

add_library(testutils STATIC ${SOURCES} ${HEADERS})

target_include_directories(testutils PRIVATE ${CMAKE_CURRENT_LIST_DIR}/private)
target_include_directories(testutils PUBLIC ${CMAKE_CURRENT_LIST_DIR}/public)

target_include_directories(testutils PUBLIC ${CONAN_GTEST_ROOT}/include)

target_link_libraries(
    testutils
    PUBLIC
        core
        GTest::gtest
)
