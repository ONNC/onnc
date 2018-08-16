include(CheckIncludeFile)
function(check_headers)
    foreach(file_n ${ARGN})
        string(REPLACE "." "_" var_n ${file_n})
        string(REPLACE "/" "_" var_n ${var_n})
        string(TOUPPER ${var_n} var_n)
        check_include_file(${file_n} HAVE_${var_n})
    endforeach()
endfunction()

include(CheckTypeSize)
function(check_types)
    foreach(type_n ${ARGN})
        string(REPLACE " " "_" var_n ${type_n})
        string(TOUPPER ${var_n} var_n)
        check_type_size(${type_n} ${var_n})
    endforeach()
endfunction()

function(try_compile_code)
    cmake_parse_arguments(OPT "" "RESULT" "HEAD;CODE" ${ARGN})
    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/try_compile.cc.in
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/try_compile.cc
        @ONLY)
    try_compile(${OPT_RESULT}
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile
        ${CMAKE_CURRENT_BINARY_DIR}/try_compile/try_compile.cc)
endfunction()