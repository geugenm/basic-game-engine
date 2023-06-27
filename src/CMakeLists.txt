cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

add_subdirectory(util)
add_subdirectory(components)
add_subdirectory(system)

add_library(engine_sdk SHARED)

target_include_directories(engine_sdk PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(engine_sdk PUBLIC
                      sdk_systems
                      )

set_target_properties(engine_sdk PROPERTIES
                      UNITY_BUILD "ON"
                      UNITY_BUILD_BATCH_SIZE 8
                      UNITY_BUILD_MODE BATCH
                      UNITY_BUILD_UNIQUE_ID "SDK_UNITY_ID"
                      )



