//===- Pad.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_PAD_H
#define ONNC_IR_COMPUTE_OPERATOR_PAD_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Pad : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kOutput = 0
  };

  static char ID;

public:
  Pad(const IntsAttr& pPads);

  // clang-format off
  Pad(const StringAttr& pMode,
      const IntsAttr& pPads,
      const FloatAttr& pValue);

  // clang-format on

  // shallow copy constructor.
  Pad(const Pad &pCopy);

  ~Pad() { }

  // clang-format off
  // Attributes getters
  const StringAttr& getMode() const { return m_Mode; }

  const IntsAttr& getPads() const { return m_Pads; }

  const FloatAttr& getValue() const { return m_Value; }


  // Attributes setters
  void setMode(const StringAttr& pMode) { m_Mode = pMode; }

  void setPads(const IntsAttr& pPads) { m_Pads = pPads; }

  void setValue(const FloatAttr& pValue) { m_Value = pValue; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getData() { return getInput(kData); }


  // Outputs getters
  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  StringAttr m_Mode;
  IntsAttr m_Pads;
  FloatAttr m_Value;
  // clang-format on
};

} // namespace of onnc

#endif
