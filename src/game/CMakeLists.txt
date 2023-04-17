cmake_minimum_required(VERSION 3.25)

add_library(AppIncludes INTERFACE)
target_include_directories(AppIncludes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        )

add_library(lib.ExampleGame SHARED game.cxx)
target_link_libraries(lib.ExampleGame PRIVATE AppIncludes)
set_target_properties(lib.ExampleGame PROPERTIES
        ENABLE_EXPORTS 1
        )