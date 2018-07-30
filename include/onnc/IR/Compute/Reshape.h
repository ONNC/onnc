//===- Reshape.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_RESHAPE_H
#define ONNC_IR_COMPUTE_OPERATOR_RESHAPE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Reshape : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kShape = 1,
    kReshaped = 0
  };

  static char ID;

public:
  Reshape();

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Reshape(const Reshape &pCopy);

  ~Reshape() { }

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
  Tensor* getData() { return getInput(kData); }

  Tensor* getShape() { return getInput(kShape); }


  // Outputs getters
  Tensor* getReshaped() { return getOutput(kReshaped); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setShape(Tensor& pTensor) { m_Inputs[kShape] = &pTensor; }


  // Outputs setters
  void setReshaped(Tensor& pTensor) { m_Outputs[kReshaped] = &pTensor; }

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
