cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(sdk_components SHARED)

target_sources(sdk_components PUBLIC ${SOURCES})
target_include_directories(sdk_components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sdk_components PUBLIC
                      sdl3_utils
                      opengl_utils
                      imgui_utils
                      nlohmann_json
                      )
