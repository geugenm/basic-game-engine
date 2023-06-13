cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "*.c" "*.cpp" "*.cxx")

file(GLOB_RECURSE IMGUI_BACKEND_SOURCES CONFIGURE_DEPENDS
     "${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp"
     "${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp"
     )

add_library(imgui_utils SHARED)

target_sources(imgui_utils PRIVATE ${SOURCES} ${IMGUI_BACKEND_SOURCES})
target_include_directories(imgui_utils PUBLIC .)

target_link_libraries(imgui_utils PRIVATE sdl3_utils)
target_link_libraries(imgui_utils PUBLIC
                      imgui
                      )
