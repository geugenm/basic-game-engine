# Set minimum CMake version and project details
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

cmake_policy(SET CMP0048 NEW)

project(basic_game_engine VERSION 1.0.0 LANGUAGES CXX C)

message(STATUS "Configuring ${PROJECT_NAME}...")

# Set default build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting default build type to Release...")
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Setting default build type to Debug. Enabling sanitizer...")
    #    add_compile_options(-fsanitize=address)
    #    add_link_options(-fsanitize=address)
endif ()

# Include necessary CMake modules and scripts
message(STATUS "Including necessary CMake modules...")
include(${CMAKE_SOURCE_DIR}/cmake/verify_cpp_support.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/git_update_submodules.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/packages.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/copy_resources.cmake)

option(ENABLE_CODE_QUALITY_TARGETS "Enable code quality tools" OFF)
if (ENABLE_CODE_QUALITY_TARGETS)
    message(STATUS "Adding code quality tools targets...")
    include(${CMAKE_SOURCE_DIR}/cmake/code_quality.cmake)
endif ()

option(BUILD_DOCS_TARGET "Build documentation" OFF)
if (BUILD_DOCS_TARGET)
    message(STATUS "Adding build documentation targets...")
    include(${CMAKE_SOURCE_DIR}/cmake/build_docs.cmake)
endif ()


# List library names
get_directory_property(MY_LIBRARIES_NAMES BUILDSYSTEM_TARGETS)
message(STATUS "Library names: ${MY_LIBRARIES_NAMES}")

# List global targets
get_property(_allTargets GLOBAL PROPERTY GlobalTargetList)
message(STATUS "GlobalTargetList: ${_allTargets}")

message(STATUS "Building ${PROJECT_NAME}...")
add_subdirectory(include)
add_subdirectory(src)
add_subdirectory(test)

# Print build information
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_FLAGS CMAKE_EXE_LINKER_FLAGS CMAKE_BINARY_DIR)

cmake_print_properties(TARGETS MY_LIBRARIES_NAMES
                       PROPERTIES POSITION_INDEPENDENT_CODE)

if (NOT DEFINED MY_LIBRARIES_NAMES)
    message(FATAL_ERROR "MY_LIBRARIES_NAMES not defined")
endif ()