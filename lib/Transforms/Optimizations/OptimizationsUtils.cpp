//===- OptimizationsUtils.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include <onnc/IR/Compute/Tensor.h>

namespace onnc {
namespace internal {

Tensor::Size getTotalSizeOfDimensions(const Tensor::Dimensions& dims)
{
  Tensor::Size totalSize = 1;
  for (Tensor::Size idx = 0; idx < dims.size(); ++idx) {
    totalSize *= dims[idx];
  }
  return totalSize;
}

} // namespace of internal
} // namespace of onnc
