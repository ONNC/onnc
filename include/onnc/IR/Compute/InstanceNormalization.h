//===- InstanceNormalization.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_INSTANCENORMALIZATION_H
#define ONNC_IR_COMPUTE_OPERATOR_INSTANCENORMALIZATION_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class InstanceNormalization : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kScale = 1,
    kB = 2,
    kOutput = 0
  };

public:
  InstanceNormalization();

  InstanceNormalization(const FloatAttr& pEpsilon);

  ~InstanceNormalization() { }

  const FloatAttr& getEpsilon() const { return m_Epsilon; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getInput() { return getInput(kInput); }

  Tensor* getScale() { return getInput(kScale); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getOutput() { return getOutput(kOutput); }

  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }

  void setScale(Tensor& pTensor) { m_Inputs[kScale] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  FloatAttr m_Epsilon;
};

} // namespace of onnc

#endif
