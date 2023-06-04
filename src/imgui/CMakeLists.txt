cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx" "*.hxx" "*.hpp" "*.h")

file(GLOB_RECURSE IMGUI_BACKEND_SOURCES CONFIGURE_DEPENDS
        "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
        "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
        )

add_library(imgui_sdk STATIC)

target_sources(imgui_sdk PRIVATE ${SOURCES} ${IMGUI_BACKEND_SOURCES})
target_include_directories(imgui_sdk PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(imgui_sdk PRIVATE sdl3_sdk)
target_link_libraries(imgui_sdk PUBLIC
        imgui
        )
