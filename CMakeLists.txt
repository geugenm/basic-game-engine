# Set minimum CMake version and project details
cmake_minimum_required(VERSION 3.23)
project(
        project.basic_game_engine
        VERSION 1.0.0
        LANGUAGES CXX C)

message(STATUS "Configuring ${PROJECT_NAME}...")

# Set default build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-fsanitize=address)
    add_link_options(-fsanitize=address)
endif ()

# Include necessary CMake modules and scripts
include(${CMAKE_CURRENT_LIST_DIR}/cmake/configure_compiler.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/code_quality.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/build_docs.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/check_platform.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/git_update_submodules.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/check_include_file.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/packages.cmake)

# List library names
get_directory_property(MY_LIBRARIES_NAMES BUILDSYSTEM_TARGETS)
message(STATUS "Library Names: ${MY_LIBRARIES_NAMES}")

# List global targets
get_property(_allTargets GLOBAL PROPERTY GlobalTargetList)
message(STATUS "GlobalTargetList: ${_allTargets}")

add_subdirectory(include)
add_subdirectory(src)

# Enable/disable building tests
option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    function(add_test_executable target_name source_file)
        add_executable(${target_name} ${source_file})
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                "${CMAKE_BINARY_DIR}/test")
        target_link_libraries(${target_name} PRIVATE GTest::GTest)
    endfunction()

    enable_testing()

    add_test_executable(test_imgui_interaction test/test_imgui_engine_interaction.cxx)
    target_link_libraries(test_imgui_interaction PRIVATE engine_sdk)

    set(NON_CPP_EXTENSIONS glsl png)

    foreach (EXTENSION ${NON_CPP_EXTENSIONS})
        file(GLOB_RECURSE NON_CPP_FILES "test/*.${EXTENSION}")
        foreach (NON_CPP_FILE ${NON_CPP_FILES})
            file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR}/test ${NON_CPP_FILE})
            configure_file(${NON_CPP_FILE} "${CMAKE_BINARY_DIR}/test/${REL_PATH}"
                    COPYONLY)
        endforeach ()
    endforeach ()
endif ()

# Print build information
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_FLAGS CMAKE_EXE_LINKER_FLAGS CMAKE_BINARY_DIR)

cmake_print_properties(TARGETS MY_LIBRARIES_NAMES
        PROPERTIES POSITION_INDEPENDENT_CODE)

# Check for required files and definitions
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/configure_compiler.cmake")
    message(SEND_ERROR "configure_compiler.cmake not found")
endif ()

if (NOT DEFINED MY_LIBRARIES_NAMES)
    message(FATAL_ERROR "MY_LIBRARIES_NAMES not defined")
endif ()
