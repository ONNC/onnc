//===- Gemm.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_GEMM_H
#define ONNC_IR_COMPUTE_OPERATOR_GEMM_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Gemm : public ComputeOperator
{
public:
  enum IOConst {
    kA = 0,
    kB = 1,
    kC = 2,
    kY = 0
  };

  static char ID;

public:
  Gemm();

  // clang-format off
  Gemm(const FloatAttr& pAlpha,
       const FloatAttr& pBeta,
       const IntAttr& pTransA,
       const IntAttr& pTransB);

  // clang-format on

  // shallow copy constructor.
  Gemm(const Gemm &pCopy);

  ~Gemm() { }

  // clang-format off
  // Attributes getters
  const FloatAttr& getAlpha() const { return m_Alpha; }

  const FloatAttr& getBeta() const { return m_Beta; }

  const IntAttr& getTransA() const { return m_TransA; }

  const IntAttr& getTransB() const { return m_TransB; }


  // Attributes setters
  void setAlpha(const FloatAttr& pAlpha) { m_Alpha = pAlpha; }

  void setBeta(const FloatAttr& pBeta) { m_Beta = pBeta; }

  void setTransA(const IntAttr& pTransA) { m_TransA = pTransA; }

  void setTransB(const IntAttr& pTransB) { m_TransB = pTransB; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getA() { return getInput(kA); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getC() { return getInput(kC); }


  // Outputs getters
  Tensor* getY() { return getOutput(kY); }


  // Inputs setters
  void setA(Tensor& pTensor) { m_Inputs[kA] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setC(Tensor& pTensor) { m_Inputs[kC] = &pTensor; }


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
  IntAttr m_TransA;
  IntAttr m_TransB;
  // clang-format on
};

} // namespace of onnc

#endif
