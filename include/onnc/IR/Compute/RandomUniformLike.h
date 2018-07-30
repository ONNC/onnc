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

  static char ID;

public:
  RandomUniformLike();

  // clang-format off
  RandomUniformLike(const IntAttr& pDtype,
                    const FloatAttr& pHigh,
                    const FloatAttr& pLow,
                    const FloatAttr& pSeed);

  // clang-format on

  // shallow copy constructor.
  RandomUniformLike(const RandomUniformLike &pCopy);

  ~RandomUniformLike() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getDtype() const { return m_Dtype; }

  const FloatAttr& getHigh() const { return m_High; }

  const FloatAttr& getLow() const { return m_Low; }

  const FloatAttr& getSeed() const { return m_Seed; }


  // Attributes setters
  void setDtype(const IntAttr& pDtype) { m_Dtype = pDtype; }

  void setHigh(const FloatAttr& pHigh) { m_High = pHigh; }

  void setLow(const FloatAttr& pLow) { m_Low = pLow; }

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
  FloatAttr m_High;
  FloatAttr m_Low;
  FloatAttr m_Seed;
  // clang-format on
};

} // namespace of onnc

#endif
