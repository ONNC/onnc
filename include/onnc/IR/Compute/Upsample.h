//===- Upsample.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_UPSAMPLE_H
#define ONNC_IR_COMPUTE_OPERATOR_UPSAMPLE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Upsample : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 0
  };

public:
  Upsample();

  Upsample(const FloatAttr& pHeightScale,
           const StringAttr& pMode,
           const FloatAttr& pWidthScale);

  ~Upsample() { }

  const FloatAttr& getHeightScale() const { return m_HeightScale; }

  const StringAttr& getMode() const { return m_Mode; }

  const FloatAttr& getWidthScale() const { return m_WidthScale; }

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
  FloatAttr m_HeightScale;
  StringAttr m_Mode;
  FloatAttr m_WidthScale;
};

} // namespace of onnc

#endif
