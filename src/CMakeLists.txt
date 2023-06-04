cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_subdirectory(opengl)
add_subdirectory(sdl)
add_subdirectory(imgui)

add_library(engine_sdk INTERFACE)
target_link_libraries(engine_sdk INTERFACE
        sdl3_sdk
        opengl_sdk
        imgui_sdk
)


