cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_library(engine_includes INTERFACE)
target_include_directories(engine_includes INTERFACE
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>)
