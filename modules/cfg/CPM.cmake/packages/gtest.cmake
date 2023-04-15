include(../CPM.cmake.cmake)

CPMAddPackage(
        NAME GTest
        GITHUB_REPOSITORY "google/googletest"
        GIT_TAG main
)

if(GTest_ADDED)
  add_library(GTest::GTest ALIAS gtest)
endif()
