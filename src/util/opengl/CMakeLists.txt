cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(opengl_utils SHARED)

target_sources(opengl_utils PUBLIC ${SOURCES})
target_include_directories(opengl_utils PUBLIC .)
target_link_libraries(opengl_utils PRIVATE engine_includes)
