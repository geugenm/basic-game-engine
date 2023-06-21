cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")


add_library(sdk_systems SHARED)

target_sources(sdk_systems PUBLIC ${SOURCES})

target_include_directories(sdk_systems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sdk_systems PRIVATE EnTT::EnTT)

target_link_libraries(sdk_systems PUBLIC
                      opengl_sdk
                      imgui_sdk
                      sdl3_sdk
                      sdk_components
                      )
