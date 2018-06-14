//===- GivenTensorFill.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_GIVENTENSORFILL_H
#define ONNC_IR_COMPUTE_OPERATOR_GIVENTENSORFILL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class GivenTensorFill : public ComputeOperator
{
public:
  enum IOConst {
    kShape = 0,
    kX = 0
  };

public:
  GivenTensorFill();

  GivenTensorFill(const IntsAttr& pExtraShape,
                  const IntAttr& pInputAsShape,
                  const IntsAttr& pShape,
                  const FloatsAttr& pValues);

  ~GivenTensorFill() { }

  const IntsAttr& getExtraShape() const { return m_ExtraShape; }

  const IntAttr& getInputAsShape() const { return m_InputAsShape; }

  const IntsAttr& getShape() const { return m_Shape; }

  const FloatsAttr& getValues() const { return m_Values; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getShape() { return getInput(kShape); }

  Tensor* getX() { return getOutput(kX); }

  void setShape(Tensor& pTensor) { m_Inputs[kShape] = &pTensor; }

  void setX(Tensor& pTensor) { m_Outputs[kX] = &pTensor; }

  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  IntsAttr m_ExtraShape;
  IntAttr m_InputAsShape;
  IntsAttr m_Shape;
  FloatsAttr m_Values;
};

} // namespace of onnc

#endif
