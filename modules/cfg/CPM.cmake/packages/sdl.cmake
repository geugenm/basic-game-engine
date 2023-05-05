include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME SDL3
        GITHUB_REPOSITORY "libsdl-org/SDL"
        GIT_TAG 1ee28323266dfba37fb4655eddddb338acc7863c
        OPTIONS
        "SDL_ATOMIC OFF"
        "SDL_STATIC ON"
        "SDL_SHARED ON"
        "SDL_VIRTUAL_JOYSTICK OFF"
        "SDL_HIDAPI_JOYSTICK OFF"
        "SDL_HAPTIC OFF"
        "SDL_JOYSTICK OFF"
        "SDL_CCACHE OFF"
        "SDL_WAYLAND OFF"  # Since you're using KDE, you might want to enable Wayland support
        "SDL_WAYLAND_LIBDECOR OFF"
        "SDL_WAYLAND_LIBDECOR_SHARED OFF"
        "SDL_WAYLAND_QT_TOUCH OFF"
        "SDL_WAYLAND_SHARED OFF"
        "SDL_X11 ON"
        "SDL_X11_SHARED ON"
        "SDL_X11_XCURSOR OFF"
        "SDL_X11_XDBE OFF"
        "SDL_X11_XFIXES OFF"
        "SDL_X11_XINPUT OFF"
        "SDL_X11_XRANDR OFF"
        "SDL_X11_XSCRNSAVER OFF"
        "SDL_X11_XSHAPE OFF"
        "X11_LIB ON"
)

