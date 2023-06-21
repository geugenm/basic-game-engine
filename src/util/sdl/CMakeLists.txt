cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(sdl3_utils SHARED)

target_sources(sdl3_utils PUBLIC ${SOURCES})
target_include_directories(sdl3_utils PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(sdl3_utils PUBLIC
                      engine_includes
                      opengl_utils
                      SDL3::SDL3-shared
                      EasyLogging::EasyLogging
                      )

