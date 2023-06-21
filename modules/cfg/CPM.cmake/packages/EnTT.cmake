include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

cpmaddpackage(
        NAME EnTT
        GITHUB_REPOSITORY skypjack/entt
        GIT_TAG v3.11.1
)

if (EnTT_ADDED)
    message(STATUS "EnTT added to CPM")
endif ()
