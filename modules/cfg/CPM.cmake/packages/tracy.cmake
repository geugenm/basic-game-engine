include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME Tracy
        GITHUB_REPOSITORY wolfpld/tracy
        GIT_TAG v0.9.1
)