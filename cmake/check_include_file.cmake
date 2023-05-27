include(CheckIncludeFile)

check_include_file("stdio.h" HAVE_STDIO_H)
check_include_file("stdlib.h" HAVE_STDLIB_H)

# Add status message for include check result
if (HAVE_STDIO_H AND HAVE_STDLIB_H)
    message(STATUS "Required headers found.")
else ()
    message(FATAL_ERROR "Basic required headers(stdio.h and stdlib.h) are not found.")
endif ()
