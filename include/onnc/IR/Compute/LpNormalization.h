//===- LpNormalization.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_LPNORMALIZATION_H
#define ONNC_IR_COMPUTE_OPERATOR_LPNORMALIZATION_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class LpNormalization : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kOutput = 0
  };

  static char ID;

public:
  LpNormalization();

  // clang-format off
  LpNormalization(const IntAttr& pAxis,
                  const IntAttr& pP);

  // clang-format on

  // shallow copy constructor.
  LpNormalization(const LpNormalization &pCopy);

  ~LpNormalization() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getP() const { return m_P; }


  // Attributes setters
  void setAxis(const IntAttr& pAxis) { m_Axis = pAxis; }

  void setP(const IntAttr& pP) { m_P = pP; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getInput() { return getInput(kInput); }


  // Outputs getters
  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }


  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntAttr m_Axis;
  IntAttr m_P;
  // clang-format on
};

} // namespace of onnc

#endif
