//===- Clip.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_CLIP_H
#define ONNC_IR_COMPUTE_OPERATOR_CLIP_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Clip : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

  static char ID;

public:
  Clip();

  // clang-format off
  Clip(const FloatAttr& pMax,
       const FloatAttr& pMin);

  // clang-format on

  // shallow copy constructor.
  Clip(const Clip &pCopy);

  virtual ~Clip() { }

  // clang-format off
  // Attributes getters
  const FloatAttr& getMax() const { return m_Max; }

  const FloatAttr& getMin() const { return m_Min; }


  // Attributes setters
  void setMax(const FloatAttr& pMax) { m_Max = pMax; }

  void setMin(const FloatAttr& pMin) { m_Min = pMin; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getInput() { return getInput(kInput); }


  // Outputs getters
  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }


  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  FloatAttr m_Max;
  FloatAttr m_Min;
  // clang-format on
};

} // namespace of onnc

#endif
