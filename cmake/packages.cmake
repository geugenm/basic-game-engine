include(${CMAKE_CURRENT_LIST_DIR}/../modules/cfg/CPM.cmake/CPM.cmake.cmake)

file(GLOB CPM_PACKAGE_FILES "${CPM_PACKAGES_DIR}/*.cmake")

foreach (CPM_PACKAGE_FILE IN LISTS CPM_PACKAGE_FILES)
    include(${CPM_PACKAGE_FILE})
    get_filename_component(PACKAGE_NAME ${CPM_PACKAGE_FILE} NAME_WE)
    message(STATUS "Included package: ${PACKAGE_NAME}")
endforeach ()

message(STATUS "Using CPM.cmake version ${CPM_VERSION}")
message(STATUS "CPM packages directory: ${CPM_PACKAGES_DIR}")
message(STATUS "CPM source cache directory: ${CPM_SOURCE_CACHE}")
message(STATUS "Using local packages: ${CPM_USE_LOCAL_PACKAGES}")
