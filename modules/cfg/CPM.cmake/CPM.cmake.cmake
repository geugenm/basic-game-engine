# Import the CPM.cmake module
include(../../CPM.cmake/cmake/CPM.cmake)

# Define the path to the packages directory
set(CPM_PACKAGES_DIR packages)

# Define the path to the source cache directory (Overriding the CPM variable)
set(CPM_SOURCE_CACHE ../../../cache)

# Set the option to use local packages to false
set(CPM_USE_LOCAL_PACKAGES OFF)
