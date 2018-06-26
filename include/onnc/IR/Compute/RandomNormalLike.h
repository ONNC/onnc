//===- RandomNormalLike.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_RANDOMNORMALLIKE_H
#define ONNC_IR_COMPUTE_OPERATOR_RANDOMNORMALLIKE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class RandomNormalLike : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

public:
  RandomNormalLike();

  RandomNormalLike(const IntAttr& pDtype,
                   const FloatAttr& pMean,
                   const FloatAttr& pScale,
                   const FloatAttr& pSeed);

  ~RandomNormalLike() { }

  const IntAttr& getDtype() const { return m_Dtype; }

  const FloatAttr& getMean() const { return m_Mean; }

  const FloatAttr& getScale() const { return m_Scale; }

  const FloatAttr& getSeed() const { return m_Seed; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getInput() { return getInput(kInput); }

  Tensor* getOutput() { return getOutput(kOutput); }

  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }
private:
  IntAttr m_Dtype;
  FloatAttr m_Mean;
  FloatAttr m_Scale;
  FloatAttr m_Seed;
};

} // namespace of onnc

#endif
