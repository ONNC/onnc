add_libonnc_src(
  Interpreter.cpp
)

add_library(${ONNC_RUNTIME_LIB_NAME}
  onnc-runtime.c
)

add_subdirectory(operator)
