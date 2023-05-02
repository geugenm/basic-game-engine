find_program(CMAKE_FORMAT "cmake-format")
if (CMAKE_FORMAT)
  add_custom_target(cmake-format
          COMMAND \${CMAKE_FORMAT} -i \${PROJECT_SOURCE_DIR}/CMakeLists.txt \${PROJECT_SOURCE_DIR}/cmake/code-quality-tools/*.cmake
          COMMENT "Formatting CMake files..."
          )
else ()
  message(WARNING "cmake-format not found. The cmake-format target will not be available.")
endif ()
