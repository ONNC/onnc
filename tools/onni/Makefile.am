ONNI_INCLUDES = -I${abs_top_srcdir}/tools/onni \
	@LIBONNC_INCLUDES@ @SKYPAT_INCLUDES@

ANDROID_CPPFLAGS=-Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

ONNI_CPPFLAGS = -O0 -g3 \
	-DUNITTEST=1 \
	-DTOPDIR=\"${abs_top_srcdir}\" \
	-DBUILDDIR=\"${abs_top_builddir}\" \
	-DONNX_NAMESPACE=onnx

if ENABLE_WERROR
ONNI_CPPFLAGS += -Werror
endif

AM_CPPFLAGS = ${ONNI_INCLUDES} ${ONNI_CPPFLAGS} ${ANDROID_CPPFLAGS}

bin_PROGRAMS = onni

onni_LDFLAGS = @LIBONNC_LDFLAGS@

onni_LDADD = @LIBONNC_LIBS@ @SKYPAT_LIBS@

nodist_onni_SOURCES = main.cpp \
	CountOperatorsPass.cpp \
	ONNIApp.cpp \
	ONNIConfig.cpp \
	Interpreter.cpp \
	InterpreterPass.cpp

if HAVE_PTHREADS
onni_LDADD += -lpthread
endif
