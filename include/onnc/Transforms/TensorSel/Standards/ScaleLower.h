//===- ScaleLower.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_SCALELOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_SCALELOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {

class ScaleLower : public Lower
{
public:
  ScaleLower();

  ~ScaleLower();

  int isMe(const xNode& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, xNode& pN) const override;
};

} // namespace of onnc

#endif
