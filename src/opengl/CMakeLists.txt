cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx" "*.hxx" "*.hpp" "*.h")

add_library(opengl_glad_sdl3_sdk STATIC)

target_sources(opengl_glad_sdl3_sdk PUBLIC ${SOURCES})
target_include_directories(opengl_glad_sdl3_sdk PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(opengl_glad_sdl3_sdk PRIVATE
        engine_includes
        )
