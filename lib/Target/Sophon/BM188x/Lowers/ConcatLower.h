//===- ConcatLower.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_CONCAT_LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_BM188X_CONCAT_LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {
namespace BM188X {

class ConcatLower : public Lower
{
public:
  ConcatLower();

  ~ConcatLower();

  int isMe(const ::onnx::Node& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, ::onnx::Node& pN) const override;
};

} // namespace BM188X
} // namespace onnc

#endif
