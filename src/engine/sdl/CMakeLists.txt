add_library(lib.SDLModel SHARED model.cxx)

target_link_libraries(lib.SDLModel PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLModel PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )




add_library(lib.SDLView SHARED view.cxx)

target_link_libraries(lib.SDLView PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLView PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )




add_library(lib.SDLPresenter SHARED presenter.cxx)

target_link_libraries(lib.SDLPresenter PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLPresenter PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )



add_library(lib.SDLReload SHARED reload.cxx)

target_link_libraries(lib.SDLReload PRIVATE
        SDL3::SDL3
        )

target_include_directories(lib.SDLReload PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../../../include/
        )