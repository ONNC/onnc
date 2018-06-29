//===- TGTargetMemInfo.cpp --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BM188xTargetMemInfo.h"
#include <onnc/Support/IOStream.h>
#include <onnc/Target/TargetTransformInfo.h>

using namespace onnc;

namespace {
const uint64_t MB = 1024 * 1024;
const uint64_t KB = 1024;
// TODO(arcbbb): Remove this once we have BM188xTTI
// const uint64_t EU_NUM = 32;
} // namespace

using TP_DataTy = onnx::TensorProto_DataType;

uint64_t BM188xTargetMemInfo::getElemSize(TP_DataTy pTy) const
{
  switch (pTy) {
  case onnx::TensorProto_DataType_FLOAT:
    // After Quantization, assume FLOAT should be transformed to INT8
  case onnx::TensorProto_DataType_BOOL:
  case onnx::TensorProto_DataType_INT8:
  case onnx::TensorProto_DataType_UINT8:
    return 1;
    break;

  case onnx::TensorProto_DataType_UINT16:
  case onnx::TensorProto_DataType_INT16:
    return 2;
    break;

  case onnx::TensorProto_DataType_COMPLEX64:
  case onnx::TensorProto_DataType_FLOAT16:
  case onnx::TensorProto_DataType_INT32:
  case onnx::TensorProto_DataType_UINT32:
  case onnx::TensorProto_DataType_INT64:
  case onnx::TensorProto_DataType_UINT64:
  case onnx::TensorProto_DataType_DOUBLE:
  case onnx::TensorProto_DataType_COMPLEX128:
  case onnx::TensorProto_DataType_STRING:
  case onnx::TensorProto_DataType_UNDEFINED:
    break;
  }
  errs() << "Unsupport element size: " << TensorProto_DataType_Name(pTy)
         << "\n";
  assert(0 && "Unsupport element size.");
}

uint64_t BM188xTargetMemInfo::getGlobalMemSize() const { return 1024 * MB; }

uint64_t BM188xTargetMemInfo::getLocalMemSize() const { return 64 * KB; }

MemSize BM188xTargetMemInfo::getValueMemorySize(onnx::Value *pValue)
{
  uint64_t sum = getElemSize(pValue->elemType());
  uint64_t eu_num = m_pTGBackend->getTTI()->getProcessingUnitCount();

  for (const onnx::Dimension &dim : pValue->sizes())
    sum *= dim.dim;

  return { eu_num, sum };
}
