//===- RNN.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_RNN_H
#define ONNC_IR_COMPUTE_OPERATOR_RNN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class RNN : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kW = 1,
    kR = 2,
    kB = 3,
    kSequenceLens = 4,
    kInitialH = 5,
    kY = 0,
    kYH = 1
  };

  static char ID;

public:
  RNN();

  // clang-format off
  RNN(const FloatsAttr& pActivationAlpha,
      const FloatsAttr& pActivationBeta,
      const StringsAttr& pActivations,
      const FloatAttr& pClip,
      const StringAttr& pDirection,
      const IntAttr& pHiddenSize);

  // clang-format on

  // shallow copy constructor.
  RNN(const RNN &pCopy);

  ~RNN() { }

  // clang-format off
  // Attributes getters
  const FloatsAttr& getActivationAlpha() const { return m_ActivationAlpha; }

  const FloatsAttr& getActivationBeta() const { return m_ActivationBeta; }

  const StringsAttr& getActivations() const { return m_Activations; }

  const FloatAttr& getClip() const { return m_Clip; }

  const StringAttr& getDirection() const { return m_Direction; }

  const IntAttr& getHiddenSize() const { return m_HiddenSize; }


  // Attributes setters
  void setActivationAlpha(const FloatsAttr& pActivationAlpha) { m_ActivationAlpha = pActivationAlpha; }

  void setActivationBeta(const FloatsAttr& pActivationBeta) { m_ActivationBeta = pActivationBeta; }

  void setActivations(const StringsAttr& pActivations) { m_Activations = pActivations; }

  void setClip(const FloatAttr& pClip) { m_Clip = pClip; }

  void setDirection(const StringAttr& pDirection) { m_Direction = pDirection; }

  void setHiddenSize(const IntAttr& pHiddenSize) { m_HiddenSize = pHiddenSize; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getX() { return getInput(kX); }

  Tensor* getW() { return getInput(kW); }

  Tensor* getR() { return getInput(kR); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getSequenceLens() { return getInput(kSequenceLens); }

  Tensor* getInitialH() { return getInput(kInitialH); }


  // Outputs getters
  Tensor* getY() { return getOutput(kY); }

  Tensor* getYH() { return getOutput(kYH); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setW(Tensor& pTensor) { m_Inputs[kW] = &pTensor; }

  void setR(Tensor& pTensor) { m_Inputs[kR] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setSequenceLens(Tensor& pTensor) { m_Inputs[kSequenceLens] = &pTensor; }

  void setInitialH(Tensor& pTensor) { m_Inputs[kInitialH] = &pTensor; }


  // Outputs setters
  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void setYH(Tensor& pTensor) { m_Outputs[kYH] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  FloatsAttr m_ActivationAlpha;
  FloatsAttr m_ActivationBeta;
  StringsAttr m_Activations;
  FloatAttr m_Clip;
  StringAttr m_Direction;
  IntAttr m_HiddenSize;
  // clang-format on
};

} // namespace of onnc

#endif
