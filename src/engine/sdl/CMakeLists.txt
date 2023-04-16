add_library(lib.SDLModel SHARED sdl_model.cxx)

target_link_libraries(lib.SDLModel PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLModel PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )




add_library(lib.SDLView SHARED sdl_view.cxx)

target_link_libraries(lib.SDLView PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLView PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )




add_library(lib.SDLPresenter SHARED sdl_presenter.cxx)

target_link_libraries(lib.SDLPresenter PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLPresenter PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )
