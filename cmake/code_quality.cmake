set(CMAKE_CODE_QUALITY_SCRIPTS ${CMAKE_SOURCE_DIR}/cmake/code-quality-tools)

# Add custom targets for formatting, linting, and other checks
include(${CMAKE_CODE_QUALITY_SCRIPTS}/enable_clang_format.cmake)
include(${CMAKE_CODE_QUALITY_SCRIPTS}/enable_clang_tidy.cmake)
include(${CMAKE_CODE_QUALITY_SCRIPTS}/enable_linter.cmake)

# Additional code-quality-tools
include(${CMAKE_CODE_QUALITY_SCRIPTS}/enable_cppcheck.cmake)
include(${CMAKE_CODE_QUALITY_SCRIPTS}/enable_cmake_format.cmake)
include(${CMAKE_CODE_QUALITY_SCRIPTS}/enable_pre_commit_hooks.cmake)
