include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME entt
        GITHUB_REPOSITORY skypjack/entt
        GIT_TAG v3.12.2
        DOWNLOAD_ONLY True
)

if (entt_ADDED)
    add_library(entt INTERFACE)

    file(GLOB_RECURSE entt_headers_h "${entt_SOURCE_DIR}/src/*.h" "${entt_SOURCE_DIR}/src/*.hpp" "${entt_SOURCE_DIR}/src/*.hxx")

    target_include_directories(entt INTERFACE "${entt_SOURCE_DIR}/src")

    add_library(EnTT::EnTT ALIAS entt)

    message(STATUS "EnTT + alias added to CPM")
endif ()
