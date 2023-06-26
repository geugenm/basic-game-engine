include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME EnTT
        GITHUB_REPOSITORY skypjack/entt
        GIT_TAG v3.12.2
)

if (EnTT_ADDED)
    message(STATUS "EnTT added to CPM")
endif ()
