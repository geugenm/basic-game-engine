cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

add_library(imgui_sdk SHARED)

target_sources(imgui_sdk PUBLIC ${SOURCES})
target_include_directories(imgui_sdk PUBLIC .)

target_link_libraries(imgui_sdk PRIVATE sdl3_sdk)

if (NOT imgui_ADDED)
    message(FATAL_ERROR "Suka blyat, no imgui")
endif ()

target_link_libraries(imgui_sdk PUBLIC
        imgui
        )
