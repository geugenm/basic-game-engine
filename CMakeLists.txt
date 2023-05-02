cmake_minimum_required(VERSION 3.25)

project(Basic_Game_Engine VERSION 1.0.0 LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

include(cmake/configure_compiler.cmake)
include(cmake/code_quality.cmake)
include(cmake/check_platform.cmake)

include(cmake/git_update_submodules.cmake)
include(cmake/check_include_file.cmake)

add_subdirectory(src)
add_subdirectory(include)

option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")

message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")

message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")