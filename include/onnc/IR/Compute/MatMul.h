//===- MatMul.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_MATMUL_H
#define ONNC_IR_COMPUTE_OPERATOR_MATMUL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class MatMul : public ComputeOperator
{
public:
  enum IOConst {
    kA = 0,
    kB = 1,
    kY = 0
  };

public:
  MatMul();

  
  ~MatMul() { }

  
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getA() { return getInput(kA); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getY() { return getOutput(kY); }

  void setA(Tensor& pTensor) { m_Inputs[kA] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }
};

} // namespace of onnc

#endif
