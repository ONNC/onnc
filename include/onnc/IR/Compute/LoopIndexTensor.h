//===- LoopIndexTensor.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_LOOPINDEXTENSOR_H
#define ONNC_IR_COMPUTE_OPERATOR_LOOPINDEXTENSOR_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class LoopIndexTensor : public ComputeOperator
{
public:
  enum IOConst {
    kT = 0,
    kLoopIdx = 1,
    kO = 0
  };

  static char ID;

public:
  LoopIndexTensor();

  // clang-format off
  LoopIndexTensor(const IntAttr& pAxis);

  // clang-format on

  // shallow copy constructor.
  LoopIndexTensor(const LoopIndexTensor &pCopy);

  ~LoopIndexTensor() { }

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
  Tensor* getT() { return getInput(kT); }

  Tensor* getLoopIdx() { return getInput(kLoopIdx); }


  // Outputs getters
  Tensor* getO() { return getOutput(kO); }


  // Inputs setters
  void setT(Tensor& pTensor) { m_Inputs[kT] = &pTensor; }

  void setLoopIdx(Tensor& pTensor) { m_Inputs[kLoopIdx] = &pTensor; }


  // Outputs setters
  void setO(Tensor& pTensor) { m_Outputs[kO] = &pTensor; }

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
