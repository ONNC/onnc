//===- Max.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_MAX_H
#define ONNC_IR_COMPUTE_OPERATOR_MAX_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Max : public ComputeOperator
{
public:
  enum IOConst {
    kData0 = 0,
    kMax = 0
  };

public:
  Max();

  
  ~Max() { }

  
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData0(size_t pIdx) { return getInput(kData0 + pIdx); }

  Tensor* getMax() { return getOutput(kMax); }

  void setData0(size_t pIdx, Tensor& pTensor) { m_Inputs[kData0 + pIdx] = &pTensor; }

  void setMax(Tensor& pTensor) { m_Outputs[kMax] = &pTensor; }

  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  
};

} // namespace of onnc

#endif
