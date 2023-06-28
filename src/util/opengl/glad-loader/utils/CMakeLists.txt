cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(glad_utils SHARED ${SOURCES})

target_include_directories(glad_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(glad_utils PUBLIC glad_es_debug_loader)
