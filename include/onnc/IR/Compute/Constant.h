//===- Constant.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_CONSTANT_H
#define ONNC_IR_COMPUTE_OPERATOR_CONSTANT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Constant : public ComputeOperator
{
public:
  enum IOConst {
    kOutput = 0
  };

  static char ID;

public:
  Constant(const TensorAttr& pValue);

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Constant(const Constant &pCopy);

  virtual ~Constant() { }

  // clang-format off
  // Attributes getters
  const TensorAttr& getValue() const { return m_Value; }


  // Attributes setters
  void setValue(const TensorAttr& pValue) { m_Value = pValue; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  

  // Outputs getters
  const Tensor* getOutput() const { return getOutput(kOutput); }

  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  

  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  // clang-format on

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  TensorAttr m_Value;
  // clang-format on
};

} // namespace of onnc

#endif
