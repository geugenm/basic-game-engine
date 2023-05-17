# Set minimum CMake version and project details
cmake_minimum_required(VERSION 3.25)
project(project.basic_game_engine VERSION 1.0.0 LANGUAGES CXX C)

message(STATUS "Configuring ${PROJECT_NAME}...")

# Set default build type to Release if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
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


# Set up include directories for engine and OpenGL
add_library(engine-includes INTERFACE)
target_include_directories(engine-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include
        )

add_library(opengl-includes INTERFACE)
target_include_directories(opengl-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/include/opengl
        )



add_library(imgui-engine)
target_include_directories(imgui-engine PUBLIC "${CMAKE_CURRENT_LIST_DIR}/src/imgui")

target_sources(imgui-engine PRIVATE
        "${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_engine.cxx"
        "${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_backends/imgui_impl_opengl3.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/src/imgui/imgui_backends/imgui_impl_sdl.cpp"
        )
target_compile_definitions(imgui-engine PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(imgui-engine PRIVATE opengl-glad)
target_link_libraries(imgui-engine PRIVATE SDL3::SDL3-shared)
target_link_libraries(imgui-engine PUBLIC engine-includes)
target_link_libraries(imgui-engine PUBLIC opengl-includes)
target_link_libraries(imgui-engine PUBLIC imgui)




# Add OpenGL glad library
add_library(opengl-glad)
target_sources(opengl-glad
        PRIVATE src/glad.c
        )
target_link_libraries(opengl-glad PUBLIC opengl-includes)




add_library(sdl-engine)
target_sources(sdl-engine PRIVATE
        src/sdl/misc/dll_reloader.cxx
        src/sdl/opengl_functions.cxx
        src/sdl/sdl_engine.cxx
        src/sdl/sdl_shader.cxx)
target_link_libraries(sdl-engine PUBLIC SDL3::SDL3-shared opengl-glad engine-includes)
target_include_directories(sdl-engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>/src/sdl
        )





# Enable/disable building tests
option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    function(add_test_executable target_name source_file)
        add_executable(${target_name} ${source_file})
        set_target_properties(${target_name} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test")
        target_link_libraries(${target_name} PRIVATE GTest::GTest)
    endfunction()

    enable_testing()

    add_test_executable(test_sdl_static_compilation test/sdl_general.cxx)
    target_link_libraries(test_sdl_static_compilation PRIVATE SDL3::SDL3-static)

    add_test_executable(test_sdl_shared_compilation test/sdl_general.cxx)
    target_link_libraries(test_sdl_shared_compilation PRIVATE SDL3::SDL3-shared)

    add_test_executable(test_opengl_init test/opengl_general.cxx)
    target_link_libraries(test_opengl_init PRIVATE opengl-glad SDL3::SDL3-shared)

    add_test_executable(test_engine_shaders_sdl test/abstract_engine_triangle.cxx)
    target_link_libraries(test_engine_shaders_sdl PRIVATE sdl-engine)

    add_test_executable(test_opengl_engine_shaders test/sdl_opengl_shader.cxx)
    target_link_libraries(test_opengl_engine_shaders PRIVATE sdl-engine)

    add_test_executable(test_complex_light_shader test/complex_light_shader.cxx)
    target_link_libraries(test_complex_light_shader PRIVATE sdl-engine)

    add_test_executable(test_sdl_engine test/sdl_engine.cxx)
    target_link_libraries(test_sdl_engine PRIVATE sdl-engine)

    add_test_executable(test_imgui test/imgui_general.cxx)
    target_link_libraries(test_imgui PRIVATE imgui-engine SDL3::SDL3-shared opengl-glad)

    set(NON_CPP_EXTENSIONS glsl)

    foreach(EXTENSION ${NON_CPP_EXTENSIONS})
        file(GLOB_RECURSE NON_CPP_FILES "test/*.${EXTENSION}")
        foreach(NON_CPP_FILE ${NON_CPP_FILES})
            file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR}/test ${NON_CPP_FILE})
            configure_file(${NON_CPP_FILE} "${CMAKE_BINARY_DIR}/test/${REL_PATH}" COPYONLY)
        endforeach()
    endforeach()
endif ()

# Print build information
message(STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}")
message(STATUS "Linker flags: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

include(CMakePrintHelpers)
cmake_print_variables(CMAKE_CXX_FLAGS CMAKE_EXE_LINKER_FLAGS CMAKE_BINARY_DIR)

cmake_print_properties(
        TARGETS MY_LIBRARIES_NAMES
        PROPERTIES POSITION_INDEPENDENT_CODE
)

# Check for required files and definitions
if (NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/cmake/configure_compiler.cmake")
    message(SEND_ERROR "configure_compiler.cmake not found")
endif ()

if (NOT DEFINED MY_LIBRARIES_NAMES)
    message(FATAL_ERROR "MY_LIBRARIES_NAMES not defined")
endif ()
