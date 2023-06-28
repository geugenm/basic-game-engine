cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    message(WARNING "Android already has OpenGL loader (prepare for possible crashes).
                     Try using `GLESv3/gl3.h` default android libraries instead:
                     0) Check your device OpenGL support: https://developer.android.com/develop/ui/views/graphics/opengl/about-opengl
                     1) Make sure you've added OpenGL support to manifest: https://developer.android.com/develop/ui/views/graphics/opengl/about-opengl#manifest.
                     2) After that, make sure you're linking with `android` `EGL` and `GLESv3` libraries.")
endif ()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.c" "src/*.cpp" "src/*.cxx")

add_library(glad_es_debug_loader SHARED ${SOURCES})

target_include_directories(glad_es_debug_loader PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

message(STATUS "Using debug C/C++ OpenGL ES 3.2 glad loader")
