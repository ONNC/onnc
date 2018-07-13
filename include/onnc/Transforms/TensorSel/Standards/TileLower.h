//===- TileLower.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_TILELOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_TILELOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {

class TileLower : public Lower
{
public:
  TileLower();

  ~TileLower();

  int isMe(const ::onnx::Node& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, ::onnx::Node& pN) const override;
};

} // namespace of onnc

#endif
