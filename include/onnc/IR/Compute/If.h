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

  static char ID;

public:
  If(const GraphAttr& pElseBranch, const GraphAttr& pThenBranch);

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  If(const If &pCopy);

  ~If() { }

  // clang-format off
  // Attributes getters
  const GraphAttr& getElseBranch() const { return m_ElseBranch; }

  const GraphAttr& getThenBranch() const { return m_ThenBranch; }


  // Attributes setters
  void setElseBranch(const GraphAttr& pElseBranch) { m_ElseBranch = pElseBranch; }

  void setThenBranch(const GraphAttr& pThenBranch) { m_ThenBranch = pThenBranch; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getCond() { return getInput(kCond); }


  // Outputs getters
  Tensor* getOutputs(size_t pIdx) { return getOutput(kOutputs + pIdx); }


  // Inputs setters
  void setCond(Tensor& pTensor) { m_Inputs[kCond] = &pTensor; }


  // Outputs setters
  void setOutputs(size_t pIdx, Tensor& pTensor) { m_Outputs[kOutputs + pIdx] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  GraphAttr m_ElseBranch;
  GraphAttr m_ThenBranch;
  // clang-format on
};

} // namespace of onnc

#endif
