#!/usr/bin/env bash

compile(){
  local PYTHON_PATH=`python -c "import onnx, os; print(os.path.dirname(onnx.__path__[0]))"`
  g++ -std=c++14 reader.cc Operator.cc oper.cc OperatorVisitor.cc "${PYTHON_PATH}/onnx/onnx.pb.cc" "-I${PYTHON_PATH}" -DONNX_NAMESPACE=onnx -o reader -lprotobuf
}

compile
