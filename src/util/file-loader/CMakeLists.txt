cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(sdl_file_loader SHARED)

target_sources(sdl_file_loader PUBLIC ${SOURCES})

target_include_directories(sdl_file_loader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sdl_file_loader PUBLIC
                      sdl3_utils
                      nlohmann_json
                      )