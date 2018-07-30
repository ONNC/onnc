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

  static char ID;

public:
  RandomNormalLike();

  // clang-format off
  RandomNormalLike(const IntAttr& pDtype,
                   const FloatAttr& pMean,
                   const FloatAttr& pScale,
                   const FloatAttr& pSeed);

  // clang-format on

  // shallow copy constructor.
  RandomNormalLike(const RandomNormalLike &pCopy);

  ~RandomNormalLike() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getDtype() const { return m_Dtype; }

  const FloatAttr& getMean() const { return m_Mean; }

  const FloatAttr& getScale() const { return m_Scale; }

  const FloatAttr& getSeed() const { return m_Seed; }


  // Attributes setters
  void setDtype(const IntAttr& pDtype) { m_Dtype = pDtype; }

  void setMean(const FloatAttr& pMean) { m_Mean = pMean; }

  void setScale(const FloatAttr& pScale) { m_Scale = pScale; }

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
  FloatAttr m_Mean;
  FloatAttr m_Scale;
  FloatAttr m_Seed;
  // clang-format on
};

} // namespace of onnc

#endif
