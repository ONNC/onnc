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

  // clang-format off
  BatchNormalization(const FloatAttr& pEpsilon,
                     const FloatAttr& pMomentum,
                     const IntAttr& pSpatial);

  // clang-format on

  // shallow copy constructor.
  BatchNormalization(const BatchNormalization &pCopy);

  ~BatchNormalization() { }

  // clang-format off
  // Attributes getters
  const FloatAttr& getEpsilon() const { return m_Epsilon; }

  const FloatAttr& getMomentum() const { return m_Momentum; }

  const IntAttr& getSpatial() const { return m_Spatial; }


  // Attributes setters
  void setEpsilon(const FloatAttr& pEpsilon) { m_Epsilon = pEpsilon; }

  void setMomentum(const FloatAttr& pMomentum) { m_Momentum = pMomentum; }

  void setSpatial(const IntAttr& pSpatial) { m_Spatial = pSpatial; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getX() { return getInput(kX); }

  Tensor* getScale() { return getInput(kScale); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getInMean() { return getInput(kInMean); }

  Tensor* getInVar() { return getInput(kInVar); }


  // Outputs getters
  Tensor* getY() { return getOutput(kY); }

  Tensor* getOutMean() { return getOutput(kOutMean); }

  Tensor* getOutVar() { return getOutput(kOutVar); }

  Tensor* getSavedMean() { return getOutput(kSavedMean); }

  Tensor* getSavedVar() { return getOutput(kSavedVar); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setScale(Tensor& pTensor) { m_Inputs[kScale] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setInMean(Tensor& pTensor) { m_Inputs[kInMean] = &pTensor; }

  void setInVar(Tensor& pTensor) { m_Inputs[kInVar] = &pTensor; }


  // Outputs setters
  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void setOutMean(Tensor& pTensor) { m_Outputs[kOutMean] = &pTensor; }

  void setOutVar(Tensor& pTensor) { m_Outputs[kOutVar] = &pTensor; }

  void setSavedMean(Tensor& pTensor) { m_Outputs[kSavedMean] = &pTensor; }

  void setSavedVar(Tensor& pTensor) { m_Outputs[kSavedVar] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  FloatAttr m_Epsilon;
  FloatAttr m_Momentum;
  IntAttr m_Spatial;
  // clang-format on
};

} // namespace of onnc

#endif
