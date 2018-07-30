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

  static char ID;

public:
  GivenTensorFill();

  // clang-format off
  GivenTensorFill(const IntsAttr& pExtraShape,
                  const IntAttr& pInputAsShape,
                  const IntsAttr& pShape,
                  const FloatsAttr& pValues);

  // clang-format on

  // shallow copy constructor.
  GivenTensorFill(const GivenTensorFill &pCopy);

  ~GivenTensorFill() { }

  // clang-format off
  // Attributes getters
  const IntsAttr& getExtraShape() const { return m_ExtraShape; }

  const IntAttr& getInputAsShape() const { return m_InputAsShape; }

  const IntsAttr& getShape() const { return m_Shape; }

  const FloatsAttr& getValues() const { return m_Values; }


  // Attributes setters
  void setExtraShape(const IntsAttr& pExtraShape) { m_ExtraShape = pExtraShape; }

  void setInputAsShape(const IntAttr& pInputAsShape) { m_InputAsShape = pInputAsShape; }

  void setShape(const IntsAttr& pShape) { m_Shape = pShape; }

  void setValues(const FloatsAttr& pValues) { m_Values = pValues; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getShape() { return getInput(kShape); }


  // Outputs getters
  Tensor* getX() { return getOutput(kX); }


  // Inputs setters
  void setShape(Tensor& pTensor) { m_Inputs[kShape] = &pTensor; }


  // Outputs setters
  void setX(Tensor& pTensor) { m_Outputs[kX] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntsAttr m_ExtraShape;
  IntAttr m_InputAsShape;
  IntsAttr m_Shape;
  FloatsAttr m_Values;
  // clang-format on
};

} // namespace of onnc

#endif
