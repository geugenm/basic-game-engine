include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
    NAME JsonReader
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG master
)
