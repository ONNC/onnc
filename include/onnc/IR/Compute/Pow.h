//===- Pow.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_POW_H
#define ONNC_IR_COMPUTE_OPERATOR_POW_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Pow : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 1,
    kZ = 0
  };

  static char ID;

public:
  Pow();

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Pow(const Pow &pCopy);

  virtual ~Pow() { }

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
  const Tensor* getX() const { return getInput(kX); }

  const Tensor* getY() const { return getInput(kY); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getY() { return getInput(kY); }


  // Outputs getters
  const Tensor* getZ() const { return getOutput(kZ); }

  Tensor* getZ() { return getOutput(kZ); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setY(Tensor& pTensor) { m_Inputs[kY] = &pTensor; }


  // Outputs setters
  void setZ(Tensor& pTensor) { m_Outputs[kZ] = &pTensor; }

  // clang-format on

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  
  // clang-format on
};

} // namespace of onnc

#endif
