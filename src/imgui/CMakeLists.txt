cmake_minimum_required(VERSION 3.25)

add_library(imgui-engine SHARED ui/imgui_ui.cxx)
target_compile_features(imgui-engine PRIVATE cxx_std_23)
target_link_libraries(imgui-engine PRIVATE imgui
        engine-includes
        glfw
        )
