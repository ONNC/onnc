bin_PROGRAMS = llc llcTest


llcTest_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ @ONNX_INCLUDES@ @SKYPAT_INCLUDES@ @LIBONNC_INCLUDES@
llcTest_LDFLAGS = @LIBONNC_LDFLAGS@
llcTest_LDADD = @LIBONNC_LIBS@ @ONNX_LIBS@ @BMKERNEL_LIBS@ @BMAPI_LIBS@ @SKYPAT_LIBS@

llc_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ @ONNX_INCLUDES@ @LIBONNC_INCLUDES@
llc_LDFLAGS = @LIBONNC_LDFLAGS@
llc_LDADD = @LIBONNC_LIBS@ @ONNX_LIBS@ @BMKERNEL_LIBS@ @BMAPI_LIBS@


nodist_llcTest_SOURCES = llcTest.cpp TGBackend.cpp TGConv.cpp TGGemm.cpp TGRelu.cpp TGLRN.cpp TGMaxPool.cpp TGSoftmax.cpp TGISelLowering.cpp removeUnusedNodePass.cpp updateOutputInfoPass.cpp TargetLowering.cpp

nodist_llc_SOURCES = llc.cpp TGBackend.cpp TGConv.cpp TGGemm.cpp TGRelu.cpp TGLRN.cpp TGMaxPool.cpp TGSoftmax.cpp TGISelLowering.cpp removeUnusedNodePass.cpp updateOutputInfoPass.cpp TargetLowering.cpp
