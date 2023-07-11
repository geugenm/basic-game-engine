cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(player_system SHARED)

target_sources(player_system PUBLIC)

target_sources(player_system PUBLIC ${SOURCES})

target_include_directories(player_system PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(player_system PUBLIC
                      sdk_components
                      EnTT::EnTT
                      )
