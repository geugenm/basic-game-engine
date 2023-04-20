cmake_minimum_required(VERSION 3.25)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages.cmake)

include(${CPM_PACKAGES_DIR}/sdl.cmake)
include(${CPM_PACKAGES_DIR}/imgui.cmake)

include(${CPM_PACKAGES_DIR}/glog.cmake)
include(${CPM_PACKAGES_DIR}/ccache.cmake)
include(${CPM_PACKAGES_DIR}/boost.cmake)

add_library(engine-includes INTERFACE)
target_include_directories(engine-includes INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
        )

add_executable(main main.cxx)

add_subdirectory(sdl)
add_subdirectory(apps)

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../include/
        )

target_compile_features(main PRIVATE cxx_std_23)

target_link_libraries(main PRIVATE
        sdl-engine
        example-game
        )
