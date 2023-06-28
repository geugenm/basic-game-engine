cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp" "src/*.cxx")

add_library(glad_loader SHARED)

target_sources(glad_loader PRIVATE ${SOURCES})
target_include_directories(glad_loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
