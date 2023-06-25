cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(sdk_ui SHARED)

target_sources(sdk_ui PUBLIC ${SOURCES})

target_include_directories(sdk_ui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sdk_ui PUBLIC
                      sdk_components
                      )
