if ENABLE_UNITTEST
if HAVE_BMKERNEL
SUBDIRS = bitmain
endif
TEST_SOURCES = DigraphTest.cpp
endif

if ENABLE_REGRESSION
MORE_TEST_SOURCES =
endif

ONNC_INCLUDES = -I${abs_top_srcdir}/tools/unittests \
	@LIBONNC_INCLUDES@ @SKYPAT_INCLUDES@

ANDROID_CPPFLAGS=-fno-rtti -fno-exceptions -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

ONNC_CPPFLAGS = -O0 -g3 \
	-DUNITTEST=1 \
	-DTOPDIR=\"${abs_top_srcdir}\" \
	-DBUILDDIR=\"${abs_top_builddir}\"

if ENABLE_WERROR
ONNC_CPPFLAGS += -Werror
endif

AM_CPPFLAGS = ${ONNC_INCLUDES} ${ONNC_CPPFLAGS} ${ANDROID_CPPFLAGS}

bin_PROGRAMS = unittests

unittests_LDFLAGS = @LIBONNC_LDFLAGS@

unittests_LDADD = -lonnc @SKYPAT_LIBS@

nodist_unittests_SOURCES = main.cpp ${TEST_SOURCES} ${MORE_TEST_SOURCES}

if HAVE_PTHREADS
unittests_LDADD += -lpthread
endif
