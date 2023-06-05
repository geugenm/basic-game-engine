if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../../CPM.cmake/cmake/CPM.cmake")
    include("${CMAKE_CURRENT_LIST_DIR}/../../CPM.cmake/cmake/CPM.cmake")
else ()
    message(FATAL_ERROR "CPM.cmake not found! Make sure you've initialized git submodules or download CPM.cmake repo and put it in the 'modules' directory of your project.")
endif ()

set(CPM_PACKAGES_DIR "${CMAKE_CURRENT_LIST_DIR}/packages")

set(CPM_SOURCE_CACHE "${CMAKE_CURRENT_LIST_DIR}/../../../cache")

set(CPM_USE_LOCAL_PACKAGES OFF)
