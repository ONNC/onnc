//===- Equal.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_EQUAL_H
#define ONNC_IR_COMPUTE_OPERATOR_EQUAL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Equal : public ComputeOperator
{
public:
  enum IOConst {
    kA = 0,
    kB = 1,
    kC = 0
  };

  static char ID;

public:
  Equal();

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Equal(const Equal &pCopy);

  ~Equal() { }

  // clang-format off
  // Attributes getters
  

  // Attributes setters
  
  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getA() { return getInput(kA); }

  Tensor* getB() { return getInput(kB); }


  // Outputs getters
  Tensor* getC() { return getOutput(kC); }


  // Inputs setters
  void setA(Tensor& pTensor) { m_Inputs[kA] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }


  // Outputs setters
  void setC(Tensor& pTensor) { m_Outputs[kC] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  
  // clang-format on
};

} // namespace of onnc

#endif
