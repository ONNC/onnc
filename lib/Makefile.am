INCDIR=../include/onnc

ONNC_SOURCES = \
	IRReader/ONNXReader.cpp \
	IR/ComputeRegOperand.cpp \
	IR/ComputeMemOperand.cpp \
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
	IR/Compute/Scale.cpp \
	IR/Compute/Tensor.cpp \
	IR/Compute/Use.cpp \
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
	IR/Compute/GlobalAveragePool.cpp \
	IR/Compute/LRN.cpp \
	IR/Compute/LeakyRelu.cpp \
	IR/Compute/MaxPool.cpp \
	IR/Compute/PRelu.cpp \
	IR/Compute/Upsample.cpp \
	IR/Compute/Relu.cpp \
	IR/Compute/Reshape.cpp \
	IR/Compute/Softmax.cpp \
	IR/Compute/Sum.cpp \
	IR/Compute/Transpose.cpp \
	IR/Compute/Xor.cpp \
	IR/Compute/Initializer.cpp \
	IR/Compute/InputOperator.cpp \
	IR/Compute/OutputOperator.cpp \
	IR/Tensor/InitializerProxy.cpp \
	Transforms/DeadNodeElimination.cpp \
	Transforms/RemoveTrainingNodes.cpp \
	Transforms/BookONNXGraphs.cpp \
	Transforms/GraphBuildingPass.cpp \
	Transforms/BuildInitializers.cpp \
	Transforms/BuildInputOperators.cpp \
	Transforms/BuildOutputOperators.cpp \
	Transforms/TensorSel.cpp \
	Transforms/TensorSel/Lower.cpp \
	Transforms/TensorSel/LowerRegistry.cpp \
	Transforms/TensorSel/DefaultAttributes.cpp \
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

ONNC_LIBS = @ONNX_LIBS@

ONNC_CPPFLAGS=-Waddress -Wchar-subscripts -Wcomment -Wformat -Wparentheses -Wreorder -Wreturn-type -Wsequence-point -Wstrict-aliasing -Wstrict-overflow=1 -Wswitch -Wtrigraphs -Wuninitialized -Wunknown-pragmas -Wunused-function -Wunused-label -Wunused-value -Wunused-variable -Wvolatile-register-var -Wno-return-stack-address

if ENABLE_DEBUG
ONNC_CPPFLAGS += -O0 -g3
endif

if ENABLE_OPTIMIZE
ONNC_CPPFLAGS += -O2 -DNDEBUG
endif

AM_CPPFLAGS = ${ONNC_INCLUDES} ${ONNC_CPPFLAGS}

AM_YFLAGS = -d
AM_LFLAGS = -olex.yy.c

#####################################
# Target-depednet files and settings
ONNC_TARGET_SOURCES = Target/Target.cpp \
	Target/TargetOptions.cpp \
	Target/TargetBackend.cpp \
	Target/TargetRegistry.cpp \
	Target/NPUTargetBackend.cpp

if ENABLE_SOPHON_TARGET
include $(srcdir)/Target/Sophon/Makefile.am
endif

if ENABLE_X86_TARGET
include $(srcdir)/Target/X86/Makefile.am
endif

#####################################
# Build shared libraries
lib_LIBRARIES = libonnc.a

libonnc_a_SOURCES = ${ONNC_SOURCES} ${ONNC_TARGET_SOURCES}

libonnc_a_LIBADD = ${ONNC_LIBS}

if HAVE_PTHREADS
libonnc_a_LDFLAGS = -lpthread
endif
