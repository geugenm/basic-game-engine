include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME SDL3
        GITHUB_REPOSITORY "libsdl-org/SDL"
        GIT_TAG main
        OPTIONS
        "SDL_ATOMIC OFF"
        "SDL_STATIC ON"
        "SDL_SHARED ON"
        "SDL_VIRTUAL_JOYSTICK ON"
        "SDL_HIDAPI_JOYSTICK ON"
        "SDL_HAPTIC ON"
        "SDL_JOYSTICK ON"
        "SDL_CCACHE ON"
        "SDL_WAYLAND OFF"
        "SDL_WAYLAND_LIBDECOR OFF"
        "SDL_WAYLAND_LIBDECOR_SHARED OFF"
        "SDL_WAYLAND_QT_TOUCH OFF"
        "SDL_WAYLAND_SHARED OFF"
        "SDL_X11 ON"
        "SDL_X11_SHARED OFF"
        "SDL_X11_XCURSOR OFF"
        "SDL_X11_XDBE OFF"
        "SDL_X11_XFIXES OFF"
        "SDL_X11_XINPUT OFF"
        "SDL_X11_XRANDR OFF"
        "SDL_X11_XSCRNSAVER OFF"
        "SDL_X11_XSHAPE OFF"
        "X11_LIB ON"
)
