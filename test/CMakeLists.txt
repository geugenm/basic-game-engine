cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_library(test_engine_android SHARED test_engine_android.cxx)
    target_link_libraries(test_engine_android PUBLIC engine_sdk)
else ()
    add_executable(test_entt test_entt.cxx)
    target_link_libraries(test_entt PUBLIC engine_sdk GTest::GTest)
endif ()

