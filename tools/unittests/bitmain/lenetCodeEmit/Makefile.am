bin_PROGRAMS = lenetCodeEmit

lenetCodeEmit_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ @ONNX_INCLUDES@
lenetCodeEmit_LDFLAGS = @ONNX_LIBS@ @BMKERNEL_LIBS@ @BMAPI_LIBS@

nodist_lenetCodeEmit_SOURCES = lenetCodeEmit.cpp TGBackend.cpp TGOperator.cpp Operator.cc oper.cc
