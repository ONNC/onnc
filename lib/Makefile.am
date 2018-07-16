INCDIR=../include/onnc

ONNC_SOURCES = \
	IRReader/ONNXReader.cpp \
	IR/ComputeOperand.cpp \
	IR/ComputeOperator.cpp \
	IR/ComputeGraph.cpp \
	IR/Module.cpp \
	IR/Dump.cpp \
	IR/Define.cpp \
	IR/ONNXNodeVisitor.cpp \
	IR/ONNXUtils.cpp \
	IR/Quadruple.cpp \
	IR/InsertionPoint.cpp \
	IR/IRBuilder.cpp \
	IR/ONNCModulePrinter.cpp \
	IR/Compute/Value.cpp \
	IR/Compute/Scalar.cpp \
	IR/Compute/Tensor.cpp \
	IR/Compute/ATen.cpp \
	IR/Compute/Abs.cpp \
	IR/Compute/Acos.cpp \
	IR/Compute/Add.cpp \
	IR/Compute/And.cpp \
	IR/Compute/Affine.cpp \
	IR/Compute/AveragePool.cpp \
	IR/Compute/BatchNormalization.cpp \
	IR/Compute/Conv.cpp \
  IR/Compute/Concat.cpp \
	IR/Compute/Gemm.cpp \
	IR/Compute/LRN.cpp \
	IR/Compute/MaxPool.cpp \
	IR/Compute/Upsample.cpp \
	IR/Compute/Relu.cpp \
	IR/Compute/Reshape.cpp \
	IR/Compute/Softmax.cpp \
	IR/Compute/Sum.cpp \
	IR/Compute/Xor.cpp \
	IR/Compute/Initializer.cpp \
	IR/Compute/InputOperator.cpp \
	IR/Compute/OutputOperator.cpp \
	IR/Graph/InitializerProxy.cpp \
	Target/Target.cpp \
	Target/TargetBackend.cpp \
	Target/TargetRegistry.cpp \
	Target/NPUTargetBackend.cpp \
	Transforms/removeUnusedNodes.cpp \
	Transforms/BookONNXGraphs.cpp \
	Transforms/PreTensorSel.cpp \
	Transforms/GraphPairPass.cpp \
	Transforms/ComplementInitializers.cpp \
	Transforms/ComplementInputOperators.cpp \
        Transforms/ComplementOutputOperators.cpp \
	Transforms/TensorSel.cpp \
	Transforms/TensorSel/Lower.cpp \
	Transforms/TensorSel/LowerRegistry.cpp \
	Transforms/TensorSel/AddLower.cpp \
	Transforms/TensorSel/AcosLower.cpp \
	Transforms/TensorSel/AbsLower.cpp \
	Transforms/TensorSel/AndLower.cpp \
	Transforms/TensorSel/ATenLower.cpp \
	Transforms/TensorSel/AffineLower.cpp \
	Transforms/TensorSel/ConvLower.cpp \
	Transforms/TensorSel/SoftmaxLower.cpp \
	Transforms/TensorSel/AveragePoolLower.cpp \
	Transforms/TensorSel/GemmLower.cpp \
	Transforms/TensorSel/MaxPoolLower.cpp \
	Transforms/TensorSel/SumLower.cpp \
	Transforms/TensorSel/BatchNormalizationLower.cpp \
	Transforms/TensorSel/LRNLower.cpp \
	Transforms/TensorSel/ReluLower.cpp \
	Transforms/TensorSel/ConcatLower.cpp \
	Transforms/TensorSel/ReshapeLower.cpp \
	Transforms/TensorSel/UpsampleLower.cpp \
	Transforms/TensorSel/XorLower.cpp \
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
	Analysis/NodeIRScheduler.cpp \
  Analysis/SplitNode.cpp \
  Analysis/UpdateGraphOutputSize.cpp \
	ADT/PolicyNodeIterator.cpp \
	ADT/Digraph.cpp \
	ADT/Buffer.cpp \
	ADT/ConstBuffer.cpp \
	ADT/Any.cpp \
	ADT/Color.cpp \
	ADT/TypeTag.cpp \
	ADT/TopologyIterator.cpp \
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

ONNC_INCLUDES = @LIBONNC_INCLUDES@ @ONNX_INCLUDES@ -I${srcdir}/Target

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
	Target/TG/TargetLowering.cpp \
	Target/TG/TargetLoweringPass.cpp \
	Target/TG/BM188x/asm.pb.cc \
	Target/TG/BM188x/common_calibration2.pb.cc

ONNC_INCLUDES += @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ -I${srcdir}/Target/TG
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
