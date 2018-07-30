//===- Split.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_SPLIT_H
#define ONNC_IR_COMPUTE_OPERATOR_SPLIT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Split : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutputs = 0
  };

  static char ID;

public:
  Split();

  // clang-format off
  Split(const IntAttr& pAxis,
        const IntsAttr& pSplit);

  // clang-format on

  // shallow copy constructor.
  Split(const Split &pCopy);

  ~Split() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getAxis() const { return m_Axis; }

  const IntsAttr& getSplit() const { return m_Split; }


  // Attributes setters
  void setAxis(const IntAttr& pAxis) { m_Axis = pAxis; }

  void setSplit(const IntsAttr& pSplit) { m_Split = pSplit; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getInput() { return getInput(kInput); }


  // Outputs getters
  Tensor* getOutputs(size_t pIdx) { return getOutput(kOutputs + pIdx); }


  // Inputs setters
  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }


  // Outputs setters
  void setOutputs(size_t pIdx, Tensor& pTensor) { m_Outputs[kOutputs + pIdx] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntAttr m_Axis;
  IntsAttr m_Split;
  // clang-format on
};

} // namespace of onnc

#endif
