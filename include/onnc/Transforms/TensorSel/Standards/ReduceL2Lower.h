//===- ReduceL2Lower.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_REDUCEL2LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_REDUCEL2LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {

class ReduceL2Lower : public Lower
{
public:
  ReduceL2Lower();

  ~ReduceL2Lower();

  int isMe(const ::onnx::Node& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, ::onnx::Node& pN) const override;
};

} // namespace of onnc

#endif
