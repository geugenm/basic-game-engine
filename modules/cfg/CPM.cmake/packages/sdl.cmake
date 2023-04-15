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
)
