cmake_minimum_required(VERSION 3.25)

set(SOURCES
        misc/dll_reloader.cxx
        render/opengl_functions.cxx
        )

add_library(sdl-engine SHARED ${SOURCES})

target_link_libraries(sdl-engine PRIVATE SDL3::SDL3 opengl-glad engine-includes)

target_include_directories(sdl-engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ui>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/render>
        )
