find_program(CPPCHECK "cppcheck")
if (CPPCHECK)
    add_custom_target(
            cppcheck
            COMMAND
            \${CPPCHECK} --enable=warning,performance,portability --inconclusive
            --force --inline-suppr --quiet --error-exitcode=1
            \${PROJECT_SOURCE_DIR}/src \${PROJECT_SOURCE_DIR}/include
            \${PROJECT_SOURCE_DIR}/test
            COMMENT "Running Cppcheck...")
else ()
    message(
            WARNING "cppcheck not found. The cppcheck target will not be available.")
endif ()
