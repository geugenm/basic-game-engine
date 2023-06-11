cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx" "*.hxx" "*.hpp" "*.h")

add_library(sdk_systems SHARED)

target_sources(sdk_systems PUBLIC ${SOURCES})
target_include_directories(sdk_systems PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(sdk_systems PUBLIC
        engine_includes
        opengl_sdk
        imgui_sdk
        sdl3_sdk
        SDL3::SDL3
        glm::glm
        EasyLogging::EasyLogging
        )
