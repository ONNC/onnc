INCDIR=../include/onnc

ONNC_SOURCES = \
	IRReader/ONNXReader.cpp \
	IR/ComputeOperand.cpp \
	IR/ComputeOperator.cpp \
	IR/ComputeGraph.cpp \
	IR/Module.cpp \
	IR/Dump.cpp \
	IR/ONNXUtils.cpp \
	IR/IRBuilder.cpp \
	IR/Compute/Value.cpp \
	IR/Compute/Scalar.cpp \
	IR/Compute/Tensor.cpp \
	IR/Compute/Conv.cpp \
	IR/Graph/Initializer.cpp \
	Target/Target.cpp \
	Target/TargetBackend.cpp \
	Target/TargetRegistry.cpp \
	Target/NPUTargetBackend.cpp \
	Transforms/removeUnusedNodes.cpp \
	Core/PassRegistry.cpp \
	Core/PassManager.cpp \
	Core/PassInfo.cpp \
	Core/AnalysisUsage.cpp \
	Core/AnalysisResolver.cpp \
	Core/Pass.cpp \
	Core/ObjectWriter.cpp \
	Core/Application.cpp \
	Core/InitializePasses.cpp \
  Analysis/LivenessAnalysis.cpp \
  Analysis/MemoryAllocation.cpp \
  Analysis/SplitNode.cpp \
  Analysis/UpdateGraphOutputSize.cpp \
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
	Support/Debug.cpp \
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
	Support/Host.cpp \
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
	JSON/Array.cpp \
  JSON/Object.cpp \
  JSON/Value.cpp \
  JSON/JsonParser.ypp \
  JSON/JsonScanner.lpp \
  JSON/String.cpp \
  JSON/Notation.cpp \
	IR/Quadruple.cpp \
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
  ONNXWrapper/ONNXWrapper.cpp \
  Option/Alias.cpp \
  Option/CommandLine.cpp \
  Option/OptDefs.cpp \
  Option/Option.cpp \
  Option/OptionPool.cpp \
  Option/OptParser.cpp

ONNC_INCLUDES = @LIBONNC_INCLUDES@ @ONNX_INCLUDES@

ONNC_LIBS = -lz @ONNX_LIBS@

if ENABLE_SOPHON_TARGET
ONNC_SOPHON_TARGET_SOURCES = \
	Target/Sophon/SophonBackend.cpp \
	Target/Sophon/TargetInfo/SophonTargetInfo.cpp
endif

if ENABLE_TG_TARGET
ONNC_TG_TARGET_SOURCES = \
	Target/TG/TGBackend.cpp \
	Target/TG/TargetInfo/TGTargetInfo.cpp \
	Target/TG/TGCodeEmitPass.cpp \
	Target/TG/TGMemAllocInfoPass.cpp \
	Target/TG/TGCodeEmitter.cpp \
	Target/TG/TGConv.cpp \
	Target/TG/TGGemm.cpp \
	Target/TG/TGISelLowering.cpp \
	Target/TG/TGLRN.cpp  \
	Target/TG/TGMaxPool.cpp \
	Target/TG/TGRelu.cpp \
	Target/TG/TGSoftmax.cpp \
	Target/TG/TargetLowering.cpp \
	Target/TG/TargetLoweringPass.cpp \
	Target/TG/RemoveUnusedNodePass.cpp \
	Target/TG/UpdateOutputInfoPass.cpp

ONNC_INCLUDES += @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@
ONNC_LIBS += ${BMKERNEL_LIBS} ${BMAPI_LIBS}

endif

if ENABLE_X86_TARGET
ONNC_X86_TARGET_SOURCES = \
	Target/X86/X86Backend.cpp \
	Target/X86/TargetInfo/X86TargetInfo.cpp
endif

ONNC_TARGET_SOURCES = \
  ${ONNC_SOPHON_TARGET_SOURCES} \
  ${ONNC_TG_TARGET_SOURCES} \
  ${ONNC_X86_TARGET_SOURCES}

ANDROID_CPPFLAGS=-Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

ONNC_CPPFLAGS = -DONNX_NAMESPACE=onnx

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

libonnc_la_SOURCES = ${ONNC_SOURCES} ${ONNC_TARGET_SOURCES}

libonnc_la_LIBADD = ${ONNC_LIBS}

if HAVE_PTHREADS
libonnc_la_LDFLAGS = -lpthread
endif
