//===- If.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_IF_H
#define ONNC_IR_COMPUTE_OPERATOR_IF_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class If : public ComputeOperator
{
public:
  enum IOConst {
    kCond = 0,
    kOutputs = 0
  };

public:
  If();

  If(const GraphAttr& pElseBranch,
     const GraphAttr& pThenBranch);

  ~If() { }

  const GraphAttr& getElseBranch() const { return m_ElseBranch; }

  const GraphAttr& getThenBranch() const { return m_ThenBranch; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getCond() { return getInput(kCond); }

  Tensor* getOutputs(size_t pIdx) { return getOutput(kOutputs + pIdx); }

  void setCond(Tensor& pTensor) { m_Inputs[kCond] = &pTensor; }

  void setOutputs(size_t pIdx, Tensor& pTensor) { m_Outputs[kOutputs + pIdx] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

private:
  GraphAttr m_ElseBranch;
  GraphAttr m_ThenBranch;
};

} // namespace of onnc

#endif
