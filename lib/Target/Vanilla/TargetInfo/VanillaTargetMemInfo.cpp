//===- VanillaTargetMemInfo.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "VanillaTargetMemInfo.h"
#include <onnc/IR/Compute/Tensor.h>

using namespace onnc;

MemSize VanillaTargetMemInfo::getTensorMemorySize(const Tensor& pVal)
{
  uint64_t align, size;
  switch (pVal.kind()) {
  case kUint8:
  case kInt8:
  case kBoolean:
    align = 16, size = 1;
    break;

  case kUint16:
  case kInt16:
  case kFloat16:
    align = 16, size = 2;
    break;

  case kFloat:
  case kInt32:
  case kUint32:
    align = 16, size = 4;
    break;

  case kInt64:
  case kUint64:
    align = 16, size = 8;
    break;

  default:
    assert(false && "Un-support value type.");
    return MemSize();
  }

  for (auto i : pVal.getDimensions())
    size *= i;

  return MemSize(align, size);
}
