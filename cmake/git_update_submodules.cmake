function(update_submodules)
    set(GIT_REPO_PATH "${CMAKE_SOURCE_DIR}/")

    # Check if Git is installed
    find_package(Git REQUIRED)

    # Check if the Git repository exists
    if (NOT EXISTS ${GIT_REPO_PATH}/.git)
        message(FATAL_ERROR "Git repository not found at ${GIT_REPO_PATH}!")
    endif ()

    # Update submodules
    execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${GIT_REPO_PATH}
            RESULT_VARIABLE submodule_result
            OUTPUT_VARIABLE submodule_output
    )

    # Check if the submodule update was successful
    if (NOT submodule_result EQUAL 0)
        message(FATAL_ERROR "Failed to update submodules in Git repository at ${GIT_REPO_PATH}!")
    else ()
        message(STATUS "Submodules updated successfully.")
    endif ()

    # Check if the submodule directories exist
    set(submodules modules/CPM.cmake)
    foreach (submodule ${submodules})
        if (NOT EXISTS ${GIT_REPO_PATH}/${submodule})
            message(FATAL_ERROR "Submodule ${submodule} not found in Git repository at ${GIT_REPO_PATH}!")
        endif ()
    endforeach ()

    # Output submodule update output
    message(STATUS "Git submodule update output: ${submodule_output}")
endfunction()

update_submodules()
