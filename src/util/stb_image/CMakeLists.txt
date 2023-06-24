cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(stb_image SHARED)

target_sources(stb_image PUBLIC ${SOURCES})
target_include_directories(stb_image PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

