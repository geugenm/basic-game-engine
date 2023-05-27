find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
    add_custom_target(
            format
            COMMAND ${CLANG_FORMAT} -i ${PROJECT_SOURCE_DIR}/src/*.cpp
            ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/test/*.cpp
            COMMAND ${CLANG_FORMAT} -i ${PROJECT_SOURCE_DIR}/src/*.cxx
            ${PROJECT_SOURCE_DIR}/include/*.hxx ${PROJECT_SOURCE_DIR}/test/*.cxx
            COMMAND ${CLANG_FORMAT} -i ${PROJECT_SOURCE_DIR}/src/*.c
            ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/test/*.c
            COMMENT "Formatting code...")
else ()
    message(
            WARNING "clang-format not found. The format target will not be available.")
endif ()
