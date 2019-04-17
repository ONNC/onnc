include_directories(
    ${ONNC_ROOT_PATH}/export/runtime/include
)

add_libonnc_src(
    Interpreter.cpp
    onni-runtime.c
    Operators.c
)
