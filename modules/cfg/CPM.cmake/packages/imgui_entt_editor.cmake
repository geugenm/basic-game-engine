include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME imgui_entt_editor
        GITHUB_REPOSITORY Green-Sky/imgui_entt_entity_editor
        GIT_TAG entt_v3.11.0
)