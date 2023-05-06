cmake_minimum_required(VERSION 3.25)

add_library(engine-includes INTERFACE)
target_include_directories(engine-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        )

add_library(opengl-includes INTERFACE)
target_include_directories(opengl-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/opengl
        )