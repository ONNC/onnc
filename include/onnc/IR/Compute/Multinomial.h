//===- Multinomial.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_MULTINOMIAL_H
#define ONNC_IR_COMPUTE_OPERATOR_MULTINOMIAL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Multinomial : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

  static char ID;

public:
  Multinomial();

  // clang-format off
  Multinomial(const IntAttr& pDtype,
              const IntAttr& pSampleSize,
              const FloatAttr& pSeed);

  // clang-format on

  // shallow copy constructor.
  Multinomial(const Multinomial &pCopy);

  ~Multinomial() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getDtype() const { return m_Dtype; }

  const IntAttr& getSampleSize() const { return m_SampleSize; }

  const FloatAttr& getSeed() const { return m_Seed; }


  // Attributes setters
  void setDtype(const IntAttr& pDtype) { m_Dtype = pDtype; }

  void setSampleSize(const IntAttr& pSampleSize) { m_SampleSize = pSampleSize; }

  void setSeed(const FloatAttr& pSeed) { m_Seed = pSeed; }

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

private:
  // clang-format off
  IntAttr m_Dtype;
  IntAttr m_SampleSize;
  FloatAttr m_Seed;
  // clang-format on
};

} // namespace of onnc

#endif
