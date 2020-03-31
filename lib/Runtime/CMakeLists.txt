add_libonnc_src(
  Interpreter.cpp
)

add_library(${ONNC_RUNTIME_LIB_NAME}
  onnc-runtime.c
)

OPTION(USE_MKLDNN "Use mkldnn" ON)
if(USE_MKLDNN)
  add_subdirectory(operatorMKLDNN)
else()
  add_subdirectory(operator)
endif(USE_MKLDNN)

install(TARGETS ${ONNC_RUNTIME_LIB_NAME}
    ARCHIVE DESTINATION lib/Runtime
)
