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

    add_test_executable(test_sdl_static_compilation test_sdl_general.cxx)
    target_link_libraries(test_sdl_static_compilation PRIVATE SDL3::SDL3-static)

    add_test_executable(test_sdl_shared_compilation test_sdl_general.cxx)
    target_link_libraries(test_sdl_shared_compilation PRIVATE SDL3::SDL3-shared)

    add_test_executable(test_opengl_texture test_opengl_texture.cxx)
    target_link_libraries(test_opengl_texture PRIVATE engine_sdk)

    add_test_executable(test_imgui_interaction test_imgui_engine_interaction.cxx)
    target_link_libraries(test_imgui_interaction PRIVATE engine_sdk)

    add_test_executable(test_sdl_sound test_sdl_sound.cxx)
    target_link_libraries(test_sdl_sound PRIVATE engine_sdk)

    add_test_executable(test_entt test_entt.cxx)
    target_link_libraries(test_entt PRIVATE engine_sdk EnTT::EnTT)

    set(EXCLUDED_EXTENSIONS cpp cxx c hpp hxx h txt)

    file(GLOB_RECURSE ALL_FILES *)

    foreach(FILE ${ALL_FILES})
        get_filename_component(EXTENSION ${FILE} EXT)
        list(FIND EXCLUDED_EXTENSIONS ${EXTENSION} IS_EXCLUDED)
        if(IS_EXCLUDED EQUAL -1)
            file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR} ${FILE})
            configure_file(${FILE} "${CMAKE_BINARY_DIR}/${REL_PATH}" COPYONLY)
        endif()
    endforeach()

endif ()