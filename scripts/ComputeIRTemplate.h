//===- ${OperatorName}.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_${OPERATORNAME}_H
#define ONNC_IR_COMPUTE_OPERATOR_${OPERATORNAME}_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ${OperatorName} : public ComputeOperator
{
public:
  enum IOConst {
    ${IOConst}
  };

public:
  ${OperatorName}();

  // clang-format off
  ${ConstructorByAttributes}
  // clang-format on
  // shallow copy constructor.
  ${OperatorName}(const ${OperatorName} &pCopy);

  ~${OperatorName}() { }

  // clang-format off
  ${AttributesGetters}
  // clang-format on
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  ${InputsGetters}
  ${OutputsGetters}
  ${InputsSetters}
  ${OutputsSetters}
  // clang-format on
  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  // clang-format off
private:
  ${AttributesMemberVariables}
  // clang-format on
};

} // namespace of onnc

#endif
