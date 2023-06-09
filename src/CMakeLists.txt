cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_subdirectory(opengl)
add_subdirectory(sdl)
add_subdirectory(imgui)
add_subdirectory(system)

add_library(engine_sdk STATIC)

target_include_directories(engine_sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine_sdk PUBLIC
        sdl3_sdk
        opengl_sdk
        imgui_sdk
        engine_includes
)


