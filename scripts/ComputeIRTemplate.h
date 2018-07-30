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

  static char ID;

public:
  ${OperatorName}(${ConstructorRequireAttributes});

  // clang-format off
  ${ConstructorByAttributes}
  // clang-format on

  // shallow copy constructor.
  ${OperatorName}(const ${OperatorName} &pCopy);

  ~${OperatorName}() { }

  // clang-format off
  // Attributes getters
  ${AttributesGetters}

  // Attributes setters
  ${AttributesSetters}
  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  ${InputsGetters}

  // Outputs getters
  ${OutputsGetters}

  // Inputs setters
  ${InputsSetters}

  // Outputs setters
  ${OutputsSetters}
  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  ${AttributesMemberVariables}
  // clang-format on
};

} // namespace of onnc

#endif
