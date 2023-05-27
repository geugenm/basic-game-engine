find_program(PRE_COMMIT "pre-commit")
if (PRE_COMMIT)
    add_custom_target(
            pre-commit
            COMMAND \${PRE_COMMIT} install
            COMMENT "Installing pre-commit hooks...")
else ()
    message(
            WARNING "pre-commit not found. The pre-commit target will not be available."
    )
endif ()
