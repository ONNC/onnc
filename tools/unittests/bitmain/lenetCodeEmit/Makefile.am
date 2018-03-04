bin_PROGRAMS = lenetCodeEmit

PYTHON_PATH=$(shell python -c "import onnx, os; print(os.path.dirname(onnx.__path__[0]))")
lenetCodeEmit_CPPFLAGS = @BMKERNEL_INCLUDES@ @BMAPI_INCLUDES@ "-I${PYTHON_PATH}" -DONNX_NAMESPACE=onnx
lenetCodeEmit_LDFLAGS = @BMKERNEL_LIBS@ @BMAPI_LIBS@ -lprotobuf

nodist_lenetCodeEmit_SOURCES = onnx.pb.cc lenetCodeEmit.cpp TGBackend.cpp TGOperator.cpp Operator.cc oper.cc
