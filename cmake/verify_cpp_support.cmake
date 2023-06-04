# Check for the highest supported C++ standard and output the compiler specs
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++20" COMPILER_SUPPORTS_CXX20)
if (COMPILER_SUPPORTS_CXX20)
    message(STATUS "C++20 is supported by the compiler.")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
else ()
    include(CompilerFlag)
    set(COMPILER_SPECS "${COMPILER_SPECS}")
    message(STATUS "Compiler specs: ${COMPILER_SPECS}")

    message(FATAL_ERROR "C++20 is not supported.")
endif ()


include(CheckIncludeFile)

check_include_file("stdio.h" HAVE_STDIO_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)

# Add status message for include check result
if (HAVE_STDIO_H AND HAVE_STDLIB_H)
    message(STATUS "Required headers found.")
else ()
    message(FATAL_ERROR "Basic required headers(stdio.h and stdlib.h) are not found.")
endif ()
