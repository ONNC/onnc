//===- Concat.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_CONCAT_H
#define ONNC_IR_COMPUTE_OPERATOR_CONCAT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Concat : public ComputeOperator
{
public:
  enum IOConst {
    kInputs = 0,
    kConcatResult = 0
  };

  static char ID;

public:
  Concat(const IntAttr& pAxis);

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Concat(const Concat &pCopy);

  ~Concat() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getAxis() const { return m_Axis; }


  // Attributes setters
  void setAxis(const IntAttr& pAxis) { m_Axis = pAxis; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getInputs(size_t pIdx) { return getInput(kInputs + pIdx); }


  // Outputs getters
  Tensor* getConcatResult() { return getOutput(kConcatResult); }


  // Inputs setters
  void setInputs(size_t pIdx, Tensor& pTensor) { m_Inputs[kInputs + pIdx] = &pTensor; }


  // Outputs setters
  void setConcatResult(Tensor& pTensor) { m_Outputs[kConcatResult] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntAttr m_Axis;
  // clang-format on
};

} // namespace of onnc

#endif
