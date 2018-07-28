//===- ATen.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_ATEN_H
#define ONNC_IR_COMPUTE_OPERATOR_ATEN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ATen : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

  static char ID;

public:
  ATen();

  // shallow copy constructor.
  ATen(const ATen& pCopy);
  
  ~ATen() { }
  
  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getInput(size_t pIdx) { return getInput(kInput + pIdx); }

  Tensor* getOutput(size_t pIdx) { return getOutput(kOutput + pIdx); }

  void setInput(size_t pIdx, Tensor& pTensor) { m_Inputs[kInput + pIdx] = &pTensor; }

  void setOutput(size_t pIdx, Tensor& pTensor) { m_Outputs[kOutput + pIdx] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);
};

} // namespace of onnc

#endif
