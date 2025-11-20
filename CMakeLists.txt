cmake_minimum_required(VERSION 3.10)

project(SyracuseCalculator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(googletest ${CMAKE_BINARY_DIR}/googletest-build)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -Wextra -Wpedantic -pthread")

include_directories(include)

set(LIB_SOURCES
    src/syracouse_info.cpp
)

add_library(syracuse_lib ${LIB_SOURCES})
target_link_libraries(syracuse_lib PRIVATE pthread)

add_executable(syracuse_main src/main.cpp)
target_link_libraries(syracuse_main PRIVATE syracuse_lib pthread)

set(TEST_SOURCES
    tests/tests.cpp
    tests/test_main.cpp
)

add_executable(syracuse_tests ${TEST_SOURCES})
target_link_libraries(syracuse_tests PRIVATE
    syracuse_lib
    gtest
    gtest_main
    pthread
)

enable_testing()
add_test(NAME SyracuseUnitTests COMMAND syracuse_tests)

add_custom_target(run_tests
    COMMAND ${CMAKE_BINARY_DIR}/syracuse_tests
    DEPENDS syracuse_tests
    COMMENT "Running unit tests"
)

add_custom_target(run_main
    COMMAND ${CMAKE_BINARY_DIR}/syracuse_main
    DEPENDS syracuse_main
    COMMENT "Running main application"
)
