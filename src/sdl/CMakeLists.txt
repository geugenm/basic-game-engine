cmake_minimum_required(VERSION 3.25)

add_library(sdl-engine SHARED ui/sdl_ui.cxx ui/sdl_ui.hxx dll_reloader.cxx)
target_link_libraries(sdl-engine PRIVATE SDL3::SDL3-shared engine-includes)
target_compile_features(sdl-engine PRIVATE cxx_std_23)