//===- MaxRoiPool.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_MAXROIPOOL_H
#define ONNC_IR_COMPUTE_OPERATOR_MAXROIPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class MaxRoiPool : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kRois = 1,
    kY = 0
  };

public:
  MaxRoiPool();

  MaxRoiPool(const IntsAttr& pPooledShape,
             const FloatAttr& pSpatialScale);

  ~MaxRoiPool() { }

  const IntsAttr& getPooledShape() const { return m_PooledShape; }

  const FloatAttr& getSpatialScale() const { return m_SpatialScale; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getRois() { return getInput(kRois); }

  Tensor* getY() { return getOutput(kY); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setRois(Tensor& pTensor) { m_Inputs[kRois] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }
private:
  IntsAttr m_PooledShape;
  FloatAttr m_SpatialScale;
};

} // namespace of onnc

#endif
