cmake_minimum_required(VERSION 3.25)

add_library(engine-includes INTERFACE)
target_include_directories(engine-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        )

add_library(sdl-engine SHARED renderer.cxx dll_reloader.cxx)
target_link_libraries(sdl-engine PRIVATE SDL3::SDL3-shared engine-includes)