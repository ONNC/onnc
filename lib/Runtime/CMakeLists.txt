file(GLOB_RECURSE OPERATOR_C_FILES operator/*.c)

include_directories(
    ${ONNC_ROOT_PATH}/export/runtime/include
)

add_libonnc_src(
    ${OPERATOR_C_FILES}
    Interpreter.cpp
    onni-runtime.c
)
