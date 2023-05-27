find_program(CPPLINT "cpplint")
if (CPPLINT)
    add_custom_target(
            lint
            COMMAND ${CPPLINT} --recursive ${PROJECT_SOURCE_DIR}/src/*.cpp
            ${PROJECT_SOURCE_DIR}/include/*.hpp ${PROJECT_SOURCE_DIR}/test/*.cpp
            COMMAND ${CPPLINT} --recursive ${PROJECT_SOURCE_DIR}/src/*.cxx
            ${PROJECT_SOURCE_DIR}/include/*.hxx ${PROJECT_SOURCE_DIR}/test/*.cxx
            COMMAND ${CPPLINT} --recursive ${PROJECT_SOURCE_DIR}/src/*.c
            ${PROJECT_SOURCE_DIR}/include/*.h ${PROJECT_SOURCE_DIR}/test/*.c
            COMMENT "Linting code...")
else ()
    message(WARNING "cpplint not found. The lint target will not be available.")
endif ()
