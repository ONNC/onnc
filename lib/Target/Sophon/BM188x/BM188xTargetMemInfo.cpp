//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
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

using TP_DataTy = xTensorProtoDataType;

uint64_t BM188xTargetMemInfo::getElemSize(TP_DataTy pTy) const
{
  switch (pTy) {
  case xValueType::kFloat:
    // After Quantization, assume FLOAT should be transformed to INT8
  case xValueType::kBoolean:
  case xValueType::kInt8:
  case xValueType::kUint8:
    return 1;
    break;

  case xValueType::kUint16:
  case xValueType::kInt16:
    return 2;
    break;

  case xValueType::kComplex64:
  case xValueType::kFloat16:
  case xValueType::kInt32:
  case xValueType::kUint32:
  case xValueType::kInt64:
  case xValueType::kUint64:
  case xValueType::kDouble:
  case xValueType::kComplex128:
  case xValueType::kString:
  case xValueType::kUndefined:
    break;
  }
  errs() << "Unsupport element size: " << TensorProto_DataType_Name(pTy)
         << "\n";
  assert(0 && "Unsupport element size.");
}

uint64_t BM188xTargetMemInfo::getGlobalMemSize() const { return 1024 * MB; }

uint64_t BM188xTargetMemInfo::getLocalMemSize() const { return 64 * KB; }

MemSize BM188xTargetMemInfo::getValueMemorySize(xValue *pValue)
{
  uint64_t sum = getElemSize(pValue->elemType());
  uint64_t eu_num = m_pTGBackend->getTTI()->getProcessingUnitCount();

  for (const xDimension &dim : pValue->sizes())
    sum *= dim.dim;

  return { eu_num, sum };
}
