cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_subdirectory(util)
add_subdirectory(system)

add_library(engine_sdk STATIC)

target_include_directories(engine_sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine_sdk PUBLIC
        sdk_util
        )


