# Set minimum CMake version and project details
cmake_minimum_required(VERSION 3.23)
project(basic_game_engine
        VERSION 1.0.0
        LANGUAGES CXX C)

message(STATUS "Configuring ${PROJECT_NAME}...")

# Set default build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting default build type to Release...")
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Setting default build type to Debug. Enabling sanitizer...")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()

# Include necessary CMake modules and scripts
message(STATUS "Including necessary CMake modules...")
include(${CMAKE_CURRENT_LIST_DIR}/cmake/verify_cpp_support.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/code_quality.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/build_docs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/git_update_submodules.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/packages.cmake)

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
