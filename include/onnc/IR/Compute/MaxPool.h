//===- MaxPool.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_MAXPOOL_H
#define ONNC_IR_COMPUTE_OPERATOR_MAXPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class MaxPool : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 0
  };

public:
  MaxPool(const IntsAttr& pKernelShape);

  MaxPool(const StringAttr& pAutoPad,
          const IntsAttr& pKernelShape,
          const IntsAttr& pPads,
          const IntsAttr& pStrides);

  ~MaxPool() { }

  const StringAttr& getAutoPad() const { return m_AutoPad; }

  const IntsAttr& getKernelShape() const { return m_KernelShape; }

  const IntsAttr& getPads() const { return m_Pads; }

  const IntsAttr& getStrides() const { return m_Strides; }

  void setAutoPad(const StringAttr& pAutoPad) { m_AutoPad = m_AutoPad; }

  void setPads(const IntsAttr& pPads) { m_Pads = m_Pads; }

  void setStrides(const IntsAttr& pStrides) { m_Strides = m_Strides; }

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
private:
  StringAttr m_AutoPad;
  IntsAttr m_KernelShape;
  IntsAttr m_Pads;
  IntsAttr m_Strides;
};

} // namespace of onnc

#endif
