message(STATUS "Copying 'resources' to build folder...")

set(EXCLUDED_EXTENSIONS cpp cxx c hpp hxx h txt)

file(GLOB_RECURSE ALL_FILES assets/*)

foreach (FILE ${ALL_FILES})
    get_filename_component(EXTENSION ${FILE} EXT)
    list(FIND EXCLUDED_EXTENSIONS ${EXTENSION} IS_EXCLUDED)
    if (IS_EXCLUDED EQUAL -1)
        file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR} ${FILE})
        configure_file(${FILE} "${CMAKE_BINARY_DIR}/${REL_PATH}" COPYONLY)
    endif ()
endforeach ()