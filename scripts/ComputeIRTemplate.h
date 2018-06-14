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

  ${ConstructorByAttributes}
  ~${OperatorName}() { }

  ${AttributesGetters}
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  ${InputsGetters}
  ${OutputsGetters}
  ${InputsSetters}
  ${OutputsSetters}
  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  ${AttributesMemberVariables}
};

} // namespace of onnc

#endif
