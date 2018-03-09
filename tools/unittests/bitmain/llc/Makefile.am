bin_PROGRAMS = llc llcTest


llcTest_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ @ONNX_INCLUDES@ @LIBONNC_LDFLAGS@ @SKYPAT_INCLUDES@
llcTest_LDFLAGS = @ONNX_LIBS@ @BMKERNEL_LIBS@ @BMAPI_LIBS@ @SKYPAT_LIBS@

llc_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ @ONNX_INCLUDES@
llc_LDFLAGS = @ONNX_LIBS@ @BMKERNEL_LIBS@ @BMAPI_LIBS@


nodist_llcTest_SOURCES = llcTest.cpp TGBackend.cpp TGOperator.cpp Operator.cc oper.cc TGConv.cpp TGGemm.cpp TGRelu.cpp TGLRN.cpp TGMaxPool.cpp TGSoftmax.cpp
nodist_llc_SOURCES = llc.cpp TGBackend.cpp TGOperator.cpp Operator.cc oper.cc TGConv.cpp TGGemm.cpp TGRelu.cpp TGLRN.cpp TGMaxPool.cpp TGSoftmax.cpp
