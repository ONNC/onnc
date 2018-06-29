
function(add_lit_target target comment)
    cmake_parse_arguments(ARG "" "" "PARAMS;DEPENDS;ARGS" ${ARGN})
    set(LIT_ARGS "${ARG_ARGS} ${LLVM_LIT_ARGS}")
    separate_arguments(LIT_ARGS)
    if (NOT CMAKE_CFG_INTDIR STREQUAL ".")
        list(APPEND LIT_ARGS --param build_mode=${CMAKE_CFG_INTDIR})
    endif ()


    set(LIT_COMMAND python ${ONNC_BINARY_DIR}/test/onnc-lit.py)
    list(APPEND LIT_COMMAND ${LIT_ARGS})
    foreach(param ${ARG_PARAMS})
        list(APPEND LIT_COMMAND --param ${param})
    endforeach()
    if (ARG_UNPARSED_ARGUMENTS)
        add_custom_target(${target}
            COMMAND ${LIT_COMMAND} ${ARG_UNPARSED_ARGUMENTS}
            COMMENT "${comment}"
            USES_TERMINAL
            )
    else()
        add_custom_target(${target}
            COMMAND ${CMAKE_COMMAND} -E echo "${target} does nothing, no tools built.")
        message(STATUS "${target} does nothing.")
    endif()
    if (ARG_DEPENDS)
        add_dependencies(${target} ${ARG_DEPENDS})
    endif()

    # Tests should be excluded from "Build Solution".
    set_target_properties(${target} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD ON)
endfunction()

function(add_lit_testsuites project directory)
    if (NOT CMAKE_CONFIGURATION_TYPES)
        cmake_parse_arguments(ARG "" "" "PARAMS;DEPENDS;ARGS" ${ARGN})

        # Search recursively for test directories by assuming anything not
        # in a directory called Inputs contains tests.
        file(GLOB_RECURSE to_process LIST_DIRECTORIES true ${directory}/*)
        foreach(lit_suite ${to_process})
            if(NOT IS_DIRECTORY ${lit_suite})
                continue()
            endif()
            string(FIND ${lit_suite} Inputs is_inputs)
            string(FIND ${lit_suite} Output is_output)
            if (NOT (is_inputs EQUAL -1 AND is_output EQUAL -1))
                continue()
            endif()

            # Create a check- target for the directory.
            string(REPLACE ${directory} "" name_slash ${lit_suite})
            if (name_slash)
                string(REPLACE "/" "-" name_slash ${name_slash})
                string(REPLACE "\\" "-" name_dashes ${name_slash})
                string(TOLOWER "${name_dashes}" name_var)
                add_lit_target("check${name_var}" "Running lit suite ${lit_suite}"
                    ${lit_suite}
                    PARAMS ${ARG_PARAMS}
                    DEPENDS ${ARG_DEPENDS}
                    ARGS ${ARG_ARGS}
                    )
            endif()
        endforeach()
    endif()
endfunction()
