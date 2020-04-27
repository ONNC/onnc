add_libonnc_src(
  Interpreter.cpp
)

add_library(${ONNC_RUNTIME_LIB_NAME}
  onnc-runtime.c
)

OPTION(USE_MKLDNN "Use mkldnn" ON)
if(USE_MKLDNN)
  find_package(MKLDNN REQUIRED)
  message(STATUS "[${PACKAGE}] Using MKLDNN include at ${MKLDNN_INCLUDE_DIR}")
  include_directories(${MKLDNN_INCLUDE_DIR})
  link_directories(${MKLDNN_LIBRARY_DIR})
  add_subdirectory(operatorMKLDNN)
  target_link_libraries(onnc-rt dnnl)
else()
  add_subdirectory(operator)
endif(USE_MKLDNN)

install(TARGETS ${ONNC_RUNTIME_LIB_NAME}
    ARCHIVE DESTINATION lib/Runtime
)
