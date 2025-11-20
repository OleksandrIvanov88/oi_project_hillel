cmake_minimum_required(VERSION 3.10)

project(SyracuseCalculator)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /O2")


include_directories(include)

set(LIB_SOURCES
    src/syracouse_info.cpp
)

add_library(syracuse_lib ${LIB_SOURCES})

target_compile_definitions(syracuse_lib PRIVATE _WIN32_WINNT=0x0601)

add_executable(syracuse_main src/main.cpp)
target_link_libraries(syracuse_main PRIVATE syracuse_lib)

add_custom_target(run_main
    COMMAND ${CMAKE_BINARY_DIR}\\syracuse_main.exe
    DEPENDS syracuse_main
    COMMENT "Running main application"
)
