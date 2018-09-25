//===- PadLower.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_PAD_LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_PAD_LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {
namespace BM188X {

class PadLower : public Lower
{
public:
  PadLower();

  ~PadLower();

  int isMe(const xNode& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, xNode& pN) const override;
};

} // namespace of BM188X
} // namespace of onnc

#endif
