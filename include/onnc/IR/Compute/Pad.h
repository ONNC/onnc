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

public:
  Pad();

  Pad(const StringAttr& pMode,
      const IntsAttr& pPads,
      const FloatAttr& pValue);

  ~Pad() { }

  const StringAttr& getMode() const { return m_Mode; }

  const IntsAttr& getPads() const { return m_Pads; }

  const FloatAttr& getValue() const { return m_Value; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getOutput() { return getOutput(kOutput); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  StringAttr m_Mode;
  IntsAttr m_Pads;
  FloatAttr m_Value;
};

} // namespace of onnc

#endif
