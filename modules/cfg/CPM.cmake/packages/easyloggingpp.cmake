include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME easyloggingpp
        GITHUB_REPOSITORY abumq/easyloggingpp
        GIT_TAG 8ef3b2d2e982f7c5715672ea0f618f36fc10d2a4
)

if(easyloggingpp_ADDED)
    message(STATUS "easyloggingpp added to CPM")
    add_library(easyloggingpp STATIC)
    target_sources(easyloggingpp PRIVATE ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc)
    target_include_directories(easyloggingpp PUBLIC ${easyloggingpp_SOURCE_DIR}/src)
endif()

if(TARGET easyloggingpp)
    add_library(EasyLogging::EasyLogging ALIAS easyloggingpp)
endif()
