include(${CMAKE_CURRENT_LIST_DIR}/../CPM.cmake.cmake)

CPMAddPackage(
        NAME easyloggingpp
        GITHUB_REPOSITORY abumq/easyloggingpp
        GIT_TAG 3bbb9a563858915b9624485356fcc7e0fdb4d68f
)

if(easyloggingpp_ADDED)
    add_library(easyloggingpp STATIC)
    target_sources(easyloggingpp PRIVATE ${easyloggingpp_SOURCE_DIR}/src/easylogging++.cc)
    target_include_directories(easyloggingpp INTERFACE ${easyloggingpp_SOURCE_DIR}/src)
endif()

if(TARGET easyloggingpp)
    add_library(EasyLogging::EasyLogging ALIAS easyloggingpp)
endif()
