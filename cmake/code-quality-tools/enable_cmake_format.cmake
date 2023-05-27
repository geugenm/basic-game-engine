find_program(CMAKE_FORMAT NAMES cmake-format)
if (CMAKE_FORMAT)
    message(STATUS "Found cmake-format: ${CMAKE_FORMAT}")
else ()
    message(
            WARNING
            "cmake-format not found. The cmake-format target will not be available.")
endif ()

include(CMakeFindDependencyMacro)
if (CMAKE_C_COMPILER)
    add_custom_target(
            cmake-format
            COMMAND ${CMAKE_FORMAT} -i ${PROJECT_SOURCE_DIR}/CMakeLists.txt
            ${PROJECT_SOURCE_DIR}/cmake/code-quality-tools/*.cmake
            COMMENT "Formatting CMake files...")
else ()
    message(
            WARNING
            "C compiler not found. The cmake-format target will not be available.")
endif ()
