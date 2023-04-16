cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)

include(${CMAKE_CURRENT_LIST_DIR}/../cmake/packages.cmake)
include(${CPM_PACKAGES_DIR}/sdl.cmake)

add_executable(main main.cxx engine/sdl/sdl_model.cxx engine/sdl/sdl_view.cxx engine/sdl/sdl_presenter.cxx)

target_link_libraries(main PRIVATE
        SDL3::SDL3
        )

target_include_directories(main PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../include/
        )

target_compile_features(main PUBLIC cxx_std_23)
