ONNC_INCLUDES = -I${abs_top_srcdir}/tools/onnx2tg \
	@LIBONNC_INCLUDES@ @SKYPAT_INCLUDES@ @ONNX_INCLUDES@

ANDROID_CPPFLAGS=-fno-exceptions -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

ONNC_CPPFLAGS = -O0 -g3 \
	-DUNITTEST=1 \
	-DTOPDIR=\"${abs_top_srcdir}\" \
	-DBUILDDIR=\"${abs_top_builddir}\" \
	-DONNX_NAMESPACE=onnx

if ENABLE_WERROR
ONNC_CPPFLAGS += -Werror
endif

AM_CPPFLAGS = ${ONNC_INCLUDES} ${ONNC_CPPFLAGS} ${ANDROID_CPPFLAGS}

bin_PROGRAMS = onnx2tg

onnx2tg_LDFLAGS = @LIBONNC_LDFLAGS@

onnx2tg_LDADD = @LIBONNC_LIBS@ @SKYPAT_LIBS@

nodist_onnx2tg_SOURCES = main.cpp \
	ONNX2TGApp.cpp \
	Config.cpp

if HAVE_PTHREADS
onnx2tg_LDADD += -lpthread
endif
