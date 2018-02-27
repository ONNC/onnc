bin_PROGRAMS = tl_test bmapi_test

tl_test_CPPFLAGS = @BMKERNEL_INCLUDES@
tl_test_LDFLAGS = @BMKERNEL_LIBS@

bmapi_test_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@
bmapi_test_LDFLAGS = @BMKERNEL_LIBS@ @BMAPI_LIBS@

nodist_tl_test_SOURCES = tl_test.cpp bmctx.cpp
nodist_bmapi_test_SOURCES = bmapi_test.cpp bmctx.cpp
