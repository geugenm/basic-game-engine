# Add custom targets for formatting, linting, and other checks
include(${CMAKE_CURRENT_LIST_DIR}/code-quality-tools/enable_clang_format.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/code-quality-tools/enable_clang_tidy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/code-quality-tools/enable_linter.cmake)

# Additional code-quality-tools
include(cmake/code-quality-tools/enable_cppcheck.cmake)
include(cmake/code-quality-tools/enable_cmake_format.cmake)
include(cmake/code-quality-tools/enable_pre_commit_hooks.cmake)
