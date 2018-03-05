bin_PROGRAMS = llc

llc_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ @ONNX_INCLUDES@
llc_LDFLAGS = @ONNX_LIBS@ @BMKERNEL_LIBS@ @BMAPI_LIBS@

nodist_llc_SOURCES = llc.cpp TGBackend.cpp TGOperator.cpp Operator.cc oper.cc
