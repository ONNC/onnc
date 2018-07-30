//===- RandomUniform.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_RANDOMUNIFORM_H
#define ONNC_IR_COMPUTE_OPERATOR_RANDOMUNIFORM_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class RandomUniform : public ComputeOperator
{
public:
  enum IOConst {
    kOutput = 0
  };

  static char ID;

public:
  RandomUniform(const IntsAttr& pShape);

  // clang-format off
  RandomUniform(const IntAttr& pDtype,
                const FloatAttr& pHigh,
                const FloatAttr& pLow,
                const FloatAttr& pSeed,
                const IntsAttr& pShape);

  // clang-format on

  // shallow copy constructor.
  RandomUniform(const RandomUniform &pCopy);

  ~RandomUniform() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getDtype() const { return m_Dtype; }

  const FloatAttr& getHigh() const { return m_High; }

  const FloatAttr& getLow() const { return m_Low; }

  const FloatAttr& getSeed() const { return m_Seed; }

  const IntsAttr& getShape() const { return m_Shape; }


  // Attributes setters
  void setDtype(const IntAttr& pDtype) { m_Dtype = pDtype; }

  void setHigh(const FloatAttr& pHigh) { m_High = pHigh; }

  void setLow(const FloatAttr& pLow) { m_Low = pLow; }

  void setSeed(const FloatAttr& pSeed) { m_Seed = pSeed; }

  void setShape(const IntsAttr& pShape) { m_Shape = pShape; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  

  // Outputs getters
  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  

  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntAttr m_Dtype;
  FloatAttr m_High;
  FloatAttr m_Low;
  FloatAttr m_Seed;
  IntsAttr m_Shape;
  // clang-format on
};

} // namespace of onnc

#endif
