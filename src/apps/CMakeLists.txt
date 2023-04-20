cmake_minimum_required(VERSION 3.25)

add_library(app-includes INTERFACE)
target_include_directories(app-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>
        )

add_library(example-game SHARED game.cxx)
target_link_libraries(example-game PRIVATE app-includes Boost::asio imgui glog)
set_target_properties(example-game PROPERTIES
        ENABLE_EXPORTS 1
        )
target_compile_features(example-game PRIVATE cxx_std_23)