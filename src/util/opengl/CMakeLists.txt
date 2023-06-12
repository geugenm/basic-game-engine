cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(opengl_sdk SHARED)

target_sources(opengl_sdk PUBLIC ${SOURCES})
target_include_directories(opengl_sdk PUBLIC .)
target_link_libraries(opengl_sdk PRIVATE engine_includes)
