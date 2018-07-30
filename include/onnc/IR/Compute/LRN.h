//===- LRN.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_LRN_H
#define ONNC_IR_COMPUTE_OPERATOR_LRN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class LRN : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 0
  };

  static char ID;

public:
  LRN(const IntAttr& pSize);

  // clang-format off
  LRN(const FloatAttr& pAlpha,
      const FloatAttr& pBeta,
      const FloatAttr& pBias,
      const IntAttr& pSize);

  // clang-format on

  // shallow copy constructor.
  LRN(const LRN &pCopy);

  ~LRN() { }

  // clang-format off
  // Attributes getters
  const FloatAttr& getAlpha() const { return m_Alpha; }

  const FloatAttr& getBeta() const { return m_Beta; }

  const FloatAttr& getBias() const { return m_Bias; }

  const IntAttr& getSize() const { return m_Size; }


  // Attributes setters
  void setAlpha(const FloatAttr& pAlpha) { m_Alpha = pAlpha; }

  void setBeta(const FloatAttr& pBeta) { m_Beta = pBeta; }

  void setBias(const FloatAttr& pBias) { m_Bias = pBias; }

  void setSize(const IntAttr& pSize) { m_Size = pSize; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getX() { return getInput(kX); }


  // Outputs getters
  Tensor* getY() { return getOutput(kY); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }


  // Outputs setters
  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  FloatAttr m_Alpha;
  FloatAttr m_Beta;
  FloatAttr m_Bias;
  IntAttr m_Size;
  // clang-format on
};

} // namespace of onnc

#endif
