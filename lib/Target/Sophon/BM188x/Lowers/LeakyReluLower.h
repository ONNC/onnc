//===- LeakyReluLower.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_LEAKYRELU_LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_LEAKYRELU_LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {
namespace BM188X {

class LeakyReluLower : public Lower
{
public:
  LeakyReluLower();

  ~LeakyReluLower();

  int isMe(const ::onnx::Node& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, ::onnx::Node& pN) const override;
};

} // namespace of BM188X
} // namespace of onnc

#endif
