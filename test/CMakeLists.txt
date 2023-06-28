cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    add_library(test_entt SHARED test_entt.cxx)
    target_link_libraries(test_entt PUBLIC engine_sdk)
else ()

    add_executable(test_entt test_entt.cxx)
    target_link_libraries(test_entt PUBLIC engine_sdk)

endif ()