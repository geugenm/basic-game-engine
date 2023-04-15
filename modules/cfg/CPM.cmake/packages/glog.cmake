include(../CPM.cmake.cmake)

CPMAddPackage(
        NAME glog
        GIT_REPOSITORY https://github.com/google/glog.git
        GIT_TAG main
)
