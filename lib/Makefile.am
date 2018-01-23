INCDIR=../include/onnc

ONNC_SOURCES = \
	ADT/Buffer.cpp \
	ADT/ConstBuffer.cpp \
	ADT/Any.cpp \
	ADT/Color.cpp \
	ADT/Digraph.cpp \
	ADT/TypeTag.cpp \
	ADT/IList.cpp \
	ADT/IListIterator.cpp \
	ADT/Rope.cpp \
	ADT/StringList.cpp \
	ADT/StringRef.cpp

ANDROID_CPPFLAGS=-fno-rtti -fno-exceptions -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

ONNC_INCLUDES = @LIBONNC_INCLUDES@

ONNC_CPPFLAGS = -std=c++11

if ENABLE_DEBUG
ONNC_CPPFLAGS += -O0 -g3
endif

if ENABLE_OPTIMIZE
ONNC_CPPFLAGS += -O2 -DNDEBUG
endif

AM_CPPFLAGS = ${ONNC_INCLUDES} ${ONNC_CPPFLAGS} ${ANDROID_CPPFLAGS}

AM_YFLAGS = -d
AM_LFLAGS = -olex.yy.c

#####################################
# Build shared libraries
lib_LTLIBRARIES = libonnc.la

libonnc_la_SOURCES = ${ONNC_SOURCES}

libonnc_la_LIBADD = -lz

if HAVE_PTHREADS
libonnc_la_LDFLAGS = -lpthread
endif
