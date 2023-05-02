# Add custom targets for formatting, linting, and other checks
include(${CMAKE_CURRENT_LIST_DIR}/code-quality-tools/enable_clang_format.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/code-quality-tools/enable_clang_tidy.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/code-quality-tools/enable_linter.cmake)
