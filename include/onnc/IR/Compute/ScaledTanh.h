//===- ScaledTanh.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_SCALEDTANH_H
#define ONNC_IR_COMPUTE_OPERATOR_SCALEDTANH_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ScaledTanh : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

  static char ID;

public:
  ScaledTanh();

  // clang-format off
  ScaledTanh(const FloatAttr& pAlpha,
             const FloatAttr& pBeta);

  // clang-format on

  // shallow copy constructor.
  ScaledTanh(const ScaledTanh &pCopy);

  ~ScaledTanh() { }

  // clang-format off
  // Attributes getters
  const FloatAttr& getAlpha() const { return m_Alpha; }

  const FloatAttr& getBeta() const { return m_Beta; }


  // Attributes setters
  void setAlpha(const FloatAttr& pAlpha) { m_Alpha = pAlpha; }

  void setBeta(const FloatAttr& pBeta) { m_Beta = pBeta; }

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
  FloatAttr m_Alpha;
  FloatAttr m_Beta;
  // clang-format on
};

} // namespace of onnc

#endif
