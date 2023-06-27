cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_subdirectory(player)

add_library(sdk_systems SHARED)

target_sources(sdk_systems PUBLIC ${SOURCES})

target_include_directories(sdk_systems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(sdk_systems PUBLIC
                      sdk_components
                      EnTT::EnTT
                      stb_image
                      player_system
                      )

set_target_properties(sdk_systems PROPERTIES
                      UNITY_BUILD "ON"
                      UNITY_BUILD_BATCH_SIZE 8
                      UNITY_BUILD_MODE BATCH
                      UNITY_BUILD_UNIQUE_ID "MY_UNITY_ID"
                      )
