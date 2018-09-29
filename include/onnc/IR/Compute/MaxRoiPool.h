//===- MaxRoiPool.h -------------------------------------------------------===//
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

  static char ID;

public:
  MaxRoiPool(const IntsAttr& pPooledShape);

  // clang-format off
  MaxRoiPool(const IntsAttr& pPooledShape,
             const FloatAttr& pSpatialScale);

  // clang-format on

  // shallow copy constructor.
  MaxRoiPool(const MaxRoiPool &pCopy);

  virtual ~MaxRoiPool() { }

  // clang-format off
  // Attributes getters
  const IntsAttr& getPooledShape() const { return m_PooledShape; }

  const FloatAttr& getSpatialScale() const { return m_SpatialScale; }


  // Attributes setters
  void setPooledShape(const IntsAttr& pPooledShape) { m_PooledShape = pPooledShape; }

  void setSpatialScale(const FloatAttr& pSpatialScale) { m_SpatialScale = pSpatialScale; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  const Tensor* getX() const { return getInput(kX); }

  const Tensor* getRois() const { return getInput(kRois); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getRois() { return getInput(kRois); }


  // Outputs getters
  const Tensor* getY() const { return getOutput(kY); }

  Tensor* getY() { return getOutput(kY); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setRois(Tensor& pTensor) { m_Inputs[kRois] = &pTensor; }


  // Outputs setters
  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  // clang-format on

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  IntsAttr m_PooledShape;
  FloatAttr m_SpatialScale;
  // clang-format on
};

} // namespace of onnc

#endif
