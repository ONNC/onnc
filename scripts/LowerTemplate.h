//===- ${OperatorName}Lower.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_${OPERATORNAME}LOWER_H
#define ONNC_TRANSFORMS_TENSOR_SELECTION_STANDARD_${OPERATORNAME}LOWER_H
#include <onnc/Transforms/TensorSel/Lower.h>

namespace onnc {

class ${OperatorName}Lower : public Lower
{
public:
  ${OperatorName}Lower();

  ~${OperatorName}Lower();

  int isMe(const ::onnx::Node& pNode) const override;

  ComputeOperator* activate(ComputeGraph& pC, ::onnx::Node& pN) const override;
};

} // namespace of onnc

#endif
