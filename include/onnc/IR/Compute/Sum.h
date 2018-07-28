//===- Sum.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_SUM_H
#define ONNC_IR_COMPUTE_OPERATOR_SUM_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Sum : public ComputeOperator
{
public:
  enum IOConst {
    kData0 = 0,
    kSum = 0
  };

  static char ID;

public:
  Sum();
  
  ~Sum() { }
  
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData0(size_t pIdx) { return getInput(kData0 + pIdx); }

  Tensor* getSum() { return getOutput(kSum); }

  void setData0(size_t pIdx, Tensor& pTensor) { m_Inputs[kData0 + pIdx] = &pTensor; }

  void setSum(Tensor& pTensor) { m_Outputs[kSum] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);
};

} // namespace of onnc

#endif
