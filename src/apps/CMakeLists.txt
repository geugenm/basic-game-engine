cmake_minimum_required(VERSION 3.25)

add_library(example-game SHARED game.cxx)
target_link_libraries(example-game PRIVATE engine-includes imgui glog)
set_target_properties(example-game PROPERTIES
        ENABLE_EXPORTS 1
        )
target_compile_features(example-game PRIVATE cxx_std_23)