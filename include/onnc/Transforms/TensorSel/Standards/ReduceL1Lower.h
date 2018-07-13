//===- ReduceL1Lower.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_REDUCEL1LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_REDUCEL1LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {

class ReduceL1Lower : public Lower
{
public:
  ReduceL1Lower();

  ~ReduceL1Lower();

  int isMe(const ::onnx::Node& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, ::onnx::Node& pN) const override;
};

} // namespace of onnc

#endif
