cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_subdirectory(opengl)
add_subdirectory(sdl)
add_subdirectory(imgui)

add_library(sdk_util SHARED)

target_include_directories(sdk_util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sdk_util PUBLIC
        sdl3_sdk
        opengl_sdk
        imgui_sdk
        engine_includes
        glm::glm
        )