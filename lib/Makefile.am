INCDIR=../include/onnc

ONNC_SOURCES = \
	IRReader/ONNXReader.cpp \
	Target/Target.cpp \
	Target/TargetRegistry.cpp \
	Target/Sophon/SophonBackend.cpp \
	Target/Sophon/TargetInfo/SophonTargetInfo.cpp \
	Target/TG/TGBackend.cpp \
	Target/TG/TargetInfo/TGTargetInfo.cpp \
	Target/X86/X86Backend.cpp \
	Target/X86/TargetInfo/X86TargetInfo.cpp \
	Core/PassRegistry.cpp \
	Core/PassInfo.cpp \
	Core/AnalysisUsage.cpp \
	Core/Pass.cpp \
	Core/ObjectWriter.cpp \
	Core/Application.cpp \
	ADT/PolicyNodeIterator.cpp \
	ADT/Digraph.cpp \
	ADT/Buffer.cpp \
	ADT/ConstBuffer.cpp \
	ADT/Any.cpp \
	ADT/Color.cpp \
	ADT/TypeTag.cpp \
	ADT/IList.cpp \
	ADT/IListIterator.cpp \
	ADT/Rope.cpp \
	ADT/StringList.cpp \
	ADT/StringRef.cpp \
  Support/AsyncPipe.cpp \
  Support/CArgu.cpp \
  Support/CArguRef.cpp \
  Support/CEnv.cpp \
  Support/Directory.cpp \
  Support/DirIterator.cpp \
  Support/ErrorCode.cpp \
  Support/Environ.cpp \
  Support/Expansion.cpp \
  Support/Glob.cpp \
  Support/IOStream.cpp \
  Support/IndentOStreamBuf.cpp \
  Support/ManagedStatic.cpp \
  Support/MappedFileRegion.cpp \
  Support/MemoryMap.cpp \
  Support/Path.cpp \
  Support/Pipe.cpp \
  Support/FileHandle.cpp \
  Support/FileLock.cpp \
  Support/FileInfo.cpp \
  Support/FileStatus.cpp \
  Support/FileSystem.cpp \
  Support/Timer.cpp \
  Support/Random.cpp \
  Support/Readline.cpp \
  Support/linenoise.cpp \
  Support/SelfPipe.cpp \
  Support/Signal.cpp \
  Support/Unix/AsyncPipe.inc \
  Support/Unix/Expansion.inc \
  Support/Unix/Glob.inc \
  Support/Unix/Path.inc \
  Support/Unix/Pipe.inc \
  Support/Unix/FileSystem.inc \
  Support/Unix/Random.inc \
  Support/Unix/SelfPipe.inc \
  Support/Windows/Path.inc \
  Support/Windows/FileSystem.inc \
  Support/Dragon/Path.inc \
  Support/Dragon/FileSystem.inc \
  Diagnostic/Diagnostic.cpp \
  Diagnostic/DiagnosticInfoMap.cpp \
  Diagnostic/Engine.cpp \
  Diagnostic/GeneralOptions.cpp \
  Diagnostic/MsgHandler.cpp \
  Diagnostic/MsgHandling.cpp \
  Diagnostic/Policy.cpp \
  Diagnostic/Logger.cpp \
  Diagnostic/SysLog.cpp \
  Diagnostic/StreamLog.cpp \
	Config/AboutLicense.cpp \
	Config/AboutData.cpp \
  Option/Alias.cpp \
  Option/CommandLine.cpp \
  Option/OptDefs.cpp \
  Option/Option.cpp \
  Option/OptionPool.cpp \
  Option/OptParser.cpp

ANDROID_CPPFLAGS=-fno-exceptions -Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

ONNC_INCLUDES = @LIBONNC_INCLUDES@ @ONNX_INCLUDES@

ONNC_CPPFLAGS =

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

libonnc_la_LIBADD = -lz @ONNX_LIBS@

if HAVE_PTHREADS
libonnc_la_LDFLAGS = -lpthread
endif
