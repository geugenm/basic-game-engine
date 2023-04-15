include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG main
)
