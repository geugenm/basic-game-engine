include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    CPMAddPackage(
            NAME SDL3
            GITHUB_REPOSITORY libsdl-org/SDL
            GIT_TAG 87e916dd214866af9cfd67eb4618918bc8b0f1b4
            OPTIONS

            "SDL_SHARED ON"

            "SDL_CCACHE ON"

            "SDL_WAYLAND OFF"
            "SDL_WAYLAND_LIBDECOR OFF"
            "SDL_WAYLAND_LIBDECOR_SHARED OFF"
            "SDL_WAYLAND_QT_TOUCH OFF"
            "SDL_WAYLAND_SHARED OFF"

            "SDL_DISABLE_INSTALL ON"
            "SDL_DISABLE_INSTALL_CPACK ON"
            "SDL_DISABLE_INSTALL_MAN ON"
            "SDL_DISABLE_UNINSTALL ON"

            "SDL_VULKAN OFF"
            "SDL_ASSEMBLY OFF"

            "SDL_TEST OFF"
            "SDL_TESTS OFF"

            "SDL_ROCKCHIP OFF"
    )
else()
CPMAddPackage(
        NAME SDL3
        GITHUB_REPOSITORY libsdl-org/SDL
        GIT_TAG 87e916dd214866af9cfd67eb4618918bc8b0f1b4
        OPTIONS

        "SDL_SHARED ON"

        "SDL_VIRTUAL_JOYSTICK OFF"
        "SDL_HIDAPI_JOYSTICK OFF"
        "SDL_HAPTIC OFF"

        "SDL_CCACHE ON"

        "SDL_WAYLAND OFF"
        "SDL_WAYLAND_LIBDECOR OFF"
        "SDL_WAYLAND_LIBDECOR_SHARED OFF"
        "SDL_WAYLAND_QT_TOUCH OFF"
        "SDL_WAYLAND_SHARED OFF"

        "SDL_DISABLE_INSTALL ON"
        "SDL_DISABLE_INSTALL_CPACK ON"
        "SDL_DISABLE_INSTALL_MAN ON"
        "SDL_DISABLE_UNINSTALL ON"

        "SDL_VULKAN OFF"
        "SDL_ASSEMBLY OFF"

        "SDL_TEST OFF"
        "SDL_TESTS OFF"

        "SDL_ROCKCHIP OFF"
)
endif ()