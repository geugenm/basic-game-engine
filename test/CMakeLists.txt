cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_library(test_engine_android SHARED test_engine_sprite_editor.cxx)
    target_link_libraries(test_engine_android PUBLIC engine_sdk)
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_executable(test_engine test_engine_sprite_editor_gtest.cxx)
    target_link_libraries(test_engine PUBLIC engine_sdk GTest::GTest)
endif ()

if (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    add_executable(test_engine test_engine_sprite_editor.cxx)
    target_link_libraries(test_engine PUBLIC engine_sdk)
endif ()

