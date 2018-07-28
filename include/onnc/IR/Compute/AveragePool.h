//===- AveragePool.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_AVERAGEPOOL_H
#define ONNC_IR_COMPUTE_OPERATOR_AVERAGEPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class AveragePool : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 0
  };

  static char ID;

public:
  AveragePool(const IntsAttr& pKernelShape);

  AveragePool(const StringAttr& pAutoPad,
              const IntAttr& pCountIncludePad,
              const IntsAttr& pKernelShape,
              const IntsAttr& pPads,
              const IntsAttr& pStrides);

  ~AveragePool() { }

  const StringAttr& getAutoPad() const { return m_AutoPad; }

  void setAutoPad(const StringAttr& pAutoPad) { m_AutoPad = pAutoPad; }

  const IntAttr& getCountIncludePad() const { return m_CountIncludePad; }

  void setCountIncludePad(const IntAttr& pCIP) { m_CountIncludePad = pCIP; }

  const IntsAttr& getKernelShape() const { return m_KernelShape; }

  const IntsAttr& getPads() const { return m_Pads; }

  void setPads(const IntsAttr& pPads) { m_Pads = pPads; }

  const IntsAttr& getStrides() const { return m_Strides; }

  void setStrides(const IntsAttr& pS) { m_Strides = pS; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getY() { return getOutput(kY); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  StringAttr m_AutoPad;
  IntAttr m_CountIncludePad;
  IntsAttr m_KernelShape;
  IntsAttr m_Pads;
  IntsAttr m_Strides;
};

} // namespace of onnc

#endif
