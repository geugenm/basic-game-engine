cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx" "*.hxx" "*.hpp" "*.h")

add_library(sdl3_sdk STATIC)

target_sources(sdl3_sdk PUBLIC ${SOURCES})
target_include_directories(sdl3_sdk PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(sdl3_sdk PUBLIC
        engine_includes
        opengl_sdk
        SDL3::SDL3-shared
        EasyLogging::EasyLogging
        )
