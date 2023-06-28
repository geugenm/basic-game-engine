cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_subdirectory(glad-loader)
add_library(opengl_utils SHARED)

target_sources(opengl_utils PRIVATE ${SOURCES})
target_include_directories(opengl_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    target_link_libraries(opengl_utils PUBLIC
                          android
                          EGL
                          GLESv3
                          )
else ()
    target_link_libraries(opengl_utils PUBLIC
                          glad_loader
                          )
endif ()
