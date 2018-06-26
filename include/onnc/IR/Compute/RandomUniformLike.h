//===- RandomUniformLike.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_RANDOMUNIFORMLIKE_H
#define ONNC_IR_COMPUTE_OPERATOR_RANDOMUNIFORMLIKE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class RandomUniformLike : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

public:
  RandomUniformLike();

  RandomUniformLike(const IntAttr& pDtype,
                    const FloatAttr& pHigh,
                    const FloatAttr& pLow,
                    const FloatAttr& pSeed);

  ~RandomUniformLike() { }

  const IntAttr& getDtype() const { return m_Dtype; }

  const FloatAttr& getHigh() const { return m_High; }

  const FloatAttr& getLow() const { return m_Low; }

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
  FloatAttr m_High;
  FloatAttr m_Low;
  FloatAttr m_Seed;
};

} // namespace of onnc

#endif
