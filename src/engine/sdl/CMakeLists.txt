add_library(int.Include INTERFACE)
target_include_directories(int.Include INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/../../../include)

add_library(lib.SDLModel SHARED model.cxx)
target_link_libraries(lib.SDLModel PRIVATE SDL3::SDL3 int.Include)

add_library(lib.SDLView SHARED view.cxx)
target_link_libraries(lib.SDLView PRIVATE SDL3::SDL3 int.Include)

add_library(lib.SDLPresenter SHARED presenter.cxx)
target_link_libraries(lib.SDLPresenter PRIVATE SDL3::SDL3 int.Include)

add_library(lib.SDLReload SHARED reload.cxx)
target_link_libraries(lib.SDLReload PRIVATE SDL3::SDL3 int.Include)
