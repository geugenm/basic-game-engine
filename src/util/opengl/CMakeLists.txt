cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "wrapper/*.c" "wrapper/*.cpp" "wrapper/*.cxx")

add_library(opengl_utils SHARED ${SOURCES})

target_include_directories(opengl_utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/wrapper)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    target_link_libraries(opengl_utils PUBLIC
                          android
                          EGL
                          GLESv3
                          )
else ()
    add_subdirectory(glad-loader)
    
    target_link_libraries(opengl_utils PUBLIC
                          glad_loader
                          )
endif ()
