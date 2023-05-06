cmake_minimum_required(VERSION 3.25)

add_library(opengl-glad)

target_sources(opengl-glad
        PRIVATE glad.c
        )

target_link_libraries(opengl-glad PUBLIC opengl-includes)

add_subdirectory(sdl)
