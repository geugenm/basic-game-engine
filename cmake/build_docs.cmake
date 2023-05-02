find_package(Doxygen)
if (DOXYGEN_FOUND)
  set(DOXYGEN_FILE "${PROJECT_SOURCE_DIR}/.doxygen")

  # Create custom target for generating documentation
  add_custom_target(doc
          COMMAND ${DOXYGEN_EXECUTABLE} "${DOXYGEN_FILE}"
          WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM
          )

  # Set output paths for HTML and LaTeX documentation
  set(DOCS_HTML_PATH "docs/html")
  set(DOCS_LATEX_PATH "docs/latex")
  set(INDEX_FILE "index.html")
  set(LATEX_MAKE_FILE "Makefile")

  # Display information about the generated documentation
  add_custom_command(TARGET doc POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E echo "Documentation generated successfully."
          COMMAND ${CMAKE_COMMAND} -E echo "You can find the web version documentation at ${PROJECT_SOURCE_DIR}/${DOCS_HTML_PATH}/${INDEX_FILE}"
          COMMAND ${CMAKE_COMMAND} -E echo "Also, you can build latex at ${PROJECT_SOURCE_DIR}/${DOCS_LATEX_PATH}/${LATEX_MAKE_FILE}"
          )
else()
  message(WARNING "Doxygen needs to be installed to generate the documentation")
endif()
