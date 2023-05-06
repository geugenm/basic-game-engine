cmake_minimum_required(VERSION 3.25)

project(sdl-engine VERSION 1.0.0 LANGUAGES CXX)

set(SOURCES
        ui/sdl_ui.cxx
        dll_reloader.cxx
        render/vertex.hxx
        render/opengl_functions.hxx
        render/shader.hxx
        )

add_library(sdl-engine SHARED ${SOURCES})

target_compile_features(sdl-engine PUBLIC cxx_std_23)

target_link_libraries(sdl-engine PRIVATE SDL3::SDL3 opengl-glad engine-includes)

target_include_directories(sdl-engine PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ui>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/render>
        )
