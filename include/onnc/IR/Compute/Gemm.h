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

public:
  Gemm();

  Gemm(const FloatAttr& pAlpha,
       const FloatAttr& pBeta,
       const IntAttr& pBroadcast,
       const IntAttr& pTransA,
       const IntAttr& pTransB);

  ~Gemm() { }

  const FloatAttr& getAlpha() const { return m_Alpha; }

  const FloatAttr& getBeta() const { return m_Beta; }

  const IntAttr& getBroadcast() const { return m_Broadcast; }

  const IntAttr& getTransA() const { return m_TransA; }

  const IntAttr& getTransB() const { return m_TransB; }

  void setAlpha(const FloatAttr& pAlpha) { m_Alpha = pAlpha; }

  void setBeta(const FloatAttr& pBeta) { m_Beta = pBeta; }

  void setBroadcast(const IntAttr& pBroadcast) { m_Broadcast = pBroadcast; }

  void setTransA(const IntAttr& pTransA) { m_TransA = pTransA; }

  void setTransB(const IntAttr& pTransB) { m_TransB = pTransB; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getA() { return getInput(kA); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getC() { return getInput(kC); }

  Tensor* getY() { return getOutput(kY); }

  void setA(Tensor& pTensor) { m_Inputs[kA] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setC(Tensor& pTensor) { m_Inputs[kC] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

private:
  FloatAttr m_Alpha;
  FloatAttr m_Beta;
  IntAttr m_Broadcast;
  IntAttr m_TransA;
  IntAttr m_TransB;
};

} // namespace of onnc

#endif
