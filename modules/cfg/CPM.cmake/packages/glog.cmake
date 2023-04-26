include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME glog
        GITHUB_REPOSITORY google/glog
        GIT_TAG v0.6.0
)

if (GTest_ADDED)
  add_library(GLog::GLog ALIAS glog)
endif ()
