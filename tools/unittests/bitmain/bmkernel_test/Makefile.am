
bin_PROGRAMS = tl_test bmapi_test

BMTAP_INSTALL = ${abs_top_srcdir}/../external/install
BMNET_INSTALL = ${abs_top_srcdir}/../external/out_bmnet/src/targets/.libs

tl_test_CPPFLAGS = -I$(BMTAP_INSTALL)/include
tl_test_LDFLAGS = -L$(BMTAP_INSTALL)/lib -lbmtap -lpthread

bmapi_test_CPPFLAGS = -I$(BMTAP_INSTALL)/include -I$(abs_top_srcdir)/../external/bmnet/src/targets/plat-bm168x/bmkernel_api_origin
bmapi_test_LDFLAGS = -L$(BMTAP_INSTALL)/lib -lbmtap -lpthread -L$(BMNET_INSTALL) -ltargets

nodist_tl_test_SOURCES = tl_test.cpp bmctx.cpp
nodist_bmapi_test_SOURCES = bmapi_test.cpp bmctx.cpp
