cmake_minimum_required(VERSION 3.25)

set(SOURCES
        ui/sdl_ui.cxx
        dll_reloader.cxx
        render/vertex.ixx
        render/opengl_functions.ixx
        )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -fmodules-ts")

add_library(sdl-engine SHARED ${SOURCES})
target_link_libraries(sdl-engine PRIVATE SDL3::SDL3-shared engine opengl-glad)
