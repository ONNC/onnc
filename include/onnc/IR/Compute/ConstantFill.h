//===- ConstantFill.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_CONSTANTFILL_H
#define ONNC_IR_COMPUTE_OPERATOR_CONSTANTFILL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ConstantFill : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

  static char ID;

public:
  ConstantFill();

  ConstantFill(const IntAttr& pDtype,
               const IntsAttr& pExtraShape,
               const IntAttr& pInputAsShape,
               const IntsAttr& pShape,
               const FloatAttr& pValue);

  ~ConstantFill() { }

  const IntAttr& getDtype() const { return m_Dtype; }

  const IntsAttr& getExtraShape() const { return m_ExtraShape; }

  const IntAttr& getInputAsShape() const { return m_InputAsShape; }

  const IntsAttr& getShape() const { return m_Shape; }

  const FloatAttr& getValue() const { return m_Value; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getInput() { return getInput(kInput); }

  Tensor* getOutput() { return getOutput(kOutput); }

  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_Dtype;
  IntsAttr m_ExtraShape;
  IntAttr m_InputAsShape;
  IntsAttr m_Shape;
  FloatAttr m_Value;
};

} // namespace of onnc

#endif
