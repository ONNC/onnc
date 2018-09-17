//===- Interpreter.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Interpreter.h"
#include <onnc/Support/IOStream.h>

#define restrict __restrict__
extern "C" {
#include <onnc/Runtime/onnc-runtime.h>
}
#undef restrict

using namespace onnc;


static inline
size_t sizeof_tensor_type(const ::onnx::TensorProto_DataType& dataType){
  switch(dataType){
    case ::onnx::TensorProto_DataType_BOOL:
      return sizeof(bool);
    case ::onnx::TensorProto_DataType_INT8:
    case ::onnx::TensorProto_DataType_UINT8:
      return 1;
    case ::onnx::TensorProto_DataType_UINT16:
    case ::onnx::TensorProto_DataType_INT16:
    case ::onnx::TensorProto_DataType_FLOAT16:
      return 2;
    case ::onnx::TensorProto_DataType_UINT32:
    case ::onnx::TensorProto_DataType_INT32:
      return 4;
    case ::onnx::TensorProto_DataType_FLOAT:
      return sizeof(float);
    case ::onnx::TensorProto_DataType_DOUBLE:
      return sizeof(double);
    case ::onnx::TensorProto_DataType_INT64:
    case ::onnx::TensorProto_DataType_UINT64:
    case ::onnx::TensorProto_DataType_COMPLEX64:
      return 8;
    case ::onnx::TensorProto_DataType_COMPLEX128:
      return 16;
    case ::onnx::TensorProto_DataType_STRING:
      return sizeof(char *);
    default:
      return 0;
  }
}

// XXX: Use runtime to allocate memory
static inline void *allocate_output_memory(::onnx::Value *v) {
  size_t size = 1;
  for (auto i: v->sizes()) {
    size *= i.dim;
  }
  return new char[sizeof_tensor_type(v->elemType()) * size];
}

static inline void print_value(::onnx::Value *v, const void *ptr) {
  size_t size = 1;
  for (auto i: v->sizes()) {
    size *= i.dim;
  }
  if (size > 100) size = 100;
  errs() << '[';
  for (size_t i = 0; i < size; ++i) {
    errs() << reinterpret_cast<const float *>(ptr)[i] << ", ";
  }
  errs() << ']' << std::endl;
}

//===----------------------------------------------------------------------===//
// Interpreter
//===----------------------------------------------------------------------===//
void Interpreter::visitNode(::onnx::Node *pNode) {
  errs() << "Not implemented node: " << pNode->kind().toString() << std::endl;
}

${interpreter_visitors}
