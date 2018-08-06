//===- RandomUniformLikeLower.h -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_RANDOMUNIFORMLIKELOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_RANDOMUNIFORMLIKELOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {

class RandomUniformLikeLower : public Lower
{
public:
  RandomUniformLikeLower();

  ~RandomUniformLikeLower();

  int isMe(const xNode& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, xNode& pN) const override;
};

} // namespace of onnc

#endif
