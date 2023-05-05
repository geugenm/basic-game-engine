cmake_minimum_required(VERSION 3.25)
project(Basic_Game_Engine VERSION 1.0.0 LANGUAGES CXX C)

message(STATUS "Configuring Basic_Game_Engine")

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

include(${CMAKE_CURRENT_LIST_DIR}/cmake/configure_compiler.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/code_quality.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/build_docs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/check_platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/git_update_submodules.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/check_include_file.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/packages.cmake)

get_directory_property(MY_LIBRARIES_NAMES BUILDSYSTEM_TARGETS)
message(STATUS "Library Names: ${MY_LIBRARIES_NAMES}")

get_property(_allTargets GLOBAL PROPERTY GlobalTargetList)
message(STATUS "GlobalTargetList: ${_allTargets}")

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

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_FLAGS CMAKE_EXE_LINKER_FLAGS CMAKE_BINARY_DIR)

cmake_print_properties(
        TARGETS MY_LIBRARIES_NAMES
        PROPERTIES POSITION_INDEPENDENT_CODE
)

if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/configure_compiler.cmake")
    message(SEND_ERROR "configure_compiler.cmake not found")
endif ()

if (NOT DEFINED MY_LIBRARIES_NAMES)
    message(FATAL_ERROR "MY_LIBRARIES_NAMES not defined")
endif ()
