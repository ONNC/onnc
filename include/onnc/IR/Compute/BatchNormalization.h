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
    kInMean = 3,
    kInVar = 4,

    kY = 0,
    kOutMean = 1,
    kOutVar = 2,
    kSavedMean = 3,
    kSavedVar = 4
  };

  static char ID;

public:
  BatchNormalization();

  BatchNormalization(const FloatAttr& pEpsilon,
                     const IntAttr& pIsTest,
                     const FloatAttr& pMomentum,
                     const IntAttr& pSpatial);

  ~BatchNormalization() { }

  const FloatAttr& getEpsilon() const { return m_Epsilon; }

  void setEpsilon(const FloatAttr& pE) { m_Epsilon = pE; }

  const IntAttr& getIsTest() const { return m_IsTest; }

  void setIsTest(const IntAttr& pB) { m_IsTest = pB; }

  const FloatAttr& getMomentum() const { return m_Momentum; }

  void setMomentum(const FloatAttr& pMomentum) { m_Momentum = pMomentum; }

  const IntAttr& getSpatial() const { return m_Spatial; }

  void setSpatial(const IntAttr& pS) { m_Spatial = pS; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getScale() { return getInput(kScale); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getInMean() { return getInput(kInMean); }

  Tensor* getInVar() { return getInput(kInVar); }

  Tensor* getY() { return getOutput(kY); }

  Tensor* getOutMean() { return getOutput(kOutMean); }

  Tensor* getOutVar() { return getOutput(kOutVar); }

  Tensor* getSavedMean() { return getOutput(kSavedMean); }

  Tensor* getSavedVar() { return getOutput(kSavedVar); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setScale(Tensor& pTensor) { m_Inputs[kScale] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setInMean(Tensor& pTensor) { m_Inputs[kInMean] = &pTensor; }

  void setInVar(Tensor& pTensor) { m_Inputs[kInVar] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void setOutMean(Tensor& pTensor) { m_Outputs[kOutMean] = &pTensor; }

  void setOutVar(Tensor& pTensor) { m_Outputs[kOutVar] = &pTensor; }

  void setSavedMean(Tensor& pTensor) { m_Outputs[kSavedMean] = &pTensor; }

  void setSavedVar(Tensor& pTensor) { m_Outputs[kSavedVar] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  FloatAttr m_Epsilon;
  IntAttr m_IsTest;
  FloatAttr m_Momentum;
  IntAttr m_Spatial;
};

} // namespace of onnc

#endif
