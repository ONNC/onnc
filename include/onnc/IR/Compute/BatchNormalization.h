//===- BatchNormalization.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_BATCHNORMALIZATION_H
#define ONNC_IR_COMPUTE_OPERATOR_BATCHNORMALIZATION_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class BatchNormalization : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kScale = 1,
    kB = 2,
    kMean = 3,
    kVar = 4,
    kY = 0,
    kMean = 1,
    kVar = 2,
    kSavedMean = 3,
    kSavedVar = 4
  };

public:
  BatchNormalization();

  BatchNormalization(const FloatAttr& pEpsilon,
                     const IntAttr& pIsTest,
                     const FloatAttr& pMomentum,
                     const IntAttr& pSpatial);

  ~BatchNormalization() { }

  const FloatAttr& getEpsilon() const { return m_Epsilon; }

  const IntAttr& getIsTest() const { return m_IsTest; }

  const FloatAttr& getMomentum() const { return m_Momentum; }

  const IntAttr& getSpatial() const { return m_Spatial; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getScale() { return getInput(kScale); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getMean() { return getInput(kMean); }

  Tensor* getVar() { return getInput(kVar); }

  Tensor* getY() { return getOutput(kY); }

  Tensor* getMean() { return getOutput(kMean); }

  Tensor* getVar() { return getOutput(kVar); }

  Tensor* getSavedMean() { return getOutput(kSavedMean); }

  Tensor* getSavedVar() { return getOutput(kSavedVar); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setScale(Tensor& pTensor) { m_Inputs[kScale] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setMean(Tensor& pTensor) { m_Inputs[kMean] = &pTensor; }

  void setVar(Tensor& pTensor) { m_Inputs[kVar] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void setMean(Tensor& pTensor) { m_Outputs[kMean] = &pTensor; }

  void setVar(Tensor& pTensor) { m_Outputs[kVar] = &pTensor; }

  void setSavedMean(Tensor& pTensor) { m_Outputs[kSavedMean] = &pTensor; }

  void setSavedVar(Tensor& pTensor) { m_Outputs[kSavedVar] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  FloatAttr m_Epsilon;
  IntAttr m_IsTest;
  FloatAttr m_Momentum;
  IntAttr m_Spatial;
};

} // namespace of onnc

#endif
