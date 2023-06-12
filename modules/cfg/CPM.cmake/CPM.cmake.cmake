if (EXISTS "${CMAKE_SOURCE_DIR}/modules/CPM.cmake/cmake/CPM.cmake")
    include("${CMAKE_SOURCE_DIR}/modules/CPM.cmake/cmake/CPM.cmake")
else ()
    message(FATAL_ERROR "CPM.cmake not found! Make sure you've initialized git submodules or download CPM.cmake repo and put it in the 'modules' directory of your project.")
endif ()

set(CPM_PACKAGES_DIR "${CMAKE_SOURCE_DIR}/modules/cfg/CPM.cmake/packages")

set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/cache")

set(CPM_USE_LOCAL_PACKAGES ON)
