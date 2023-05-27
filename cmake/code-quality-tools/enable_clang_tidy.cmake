find_program(CLANG_TIDY "clang-tidy")
if (CLANG_TIDY)
    add_custom_target(
            tidy
            COMMAND
            ${CLANG_TIDY} ${PROJECT_SOURCE_DIR}/src/*.cpp
            ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/test/*.cpp --
            -std=c++17
            COMMAND ${CLANG_TIDY} ${PROJECT_SOURCE_DIR}/src/*.cxx
            ${PROJECT_SOURCE_DIR}/include/*.hxx ${PROJECT_SOURCE_DIR}/test/*.cxx
            COMMAND ${CLANG_TIDY} ${PROJECT_SOURCE_DIR}/src/*.c
            ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/test/*.c
            COMMENT "Checking code with clang-tidy...")
else ()
    message(
            WARNING "clang-tidy not found. The tidy target will not be available.")
endif ()
