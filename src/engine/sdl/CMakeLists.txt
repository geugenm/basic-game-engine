add_library(int.Include INTERFACE)
target_include_directories(int.Include INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

add_library(lib.SDLModel SHARED model.cxx)
target_link_libraries(lib.SDLModel PRIVATE SDL3::SDL3-shared int.Include)
set_target_properties(
        lib.SDLModel PROPERTIES
        ENABLE_EXPORTS 1
)

add_library(lib.SDLView SHARED view.cxx)
target_link_libraries(lib.SDLView PRIVATE SDL3::SDL3-shared int.Include)
set_target_properties(
        lib.SDLView PROPERTIES
        ENABLE_EXPORTS 1
)

add_library(lib.SDLPresenter SHARED presenter.cxx)
target_link_libraries(lib.SDLPresenter PRIVATE SDL3::SDL3-shared int.Include)
set_target_properties(
        lib.SDLPresenter PROPERTIES
        ENABLE_EXPORTS 1
)