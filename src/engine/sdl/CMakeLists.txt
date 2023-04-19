cmake_minimum_required(VERSION 3.25)

add_library(EngineIncludes INTERFACE)
target_include_directories(EngineIncludes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../../include>
        )

add_library(lib.SDLEngine SHARED renderer.cxx dll_reloader.cxx)
target_link_libraries(lib.SDLEngine PRIVATE SDL3::SDL3-shared EngineIncludes)