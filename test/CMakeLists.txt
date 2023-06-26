cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

# Enable/disable building tests
option(BUILD_TESTS "Build tests" ON)
if (BUILD_TESTS)
    function(add_test_executable target_name source_file)
        add_executable(${target_name} ${source_file})
        set_target_properties(${target_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                              "${CMAKE_BINARY_DIR}/test")
        target_link_libraries(${target_name} PRIVATE GTest::GTest)
    endfunction()

    enable_testing()

    add_test_executable(test_entt test_entt.cxx)
    target_link_libraries(test_entt PUBLIC engine_sdk EnTT::EnTT Tracy::TracyClient)
    target_compile_options(test_entt PRIVATE -fPIE)
endif ()