//===- ArgMin.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_ARGMIN_H
#define ONNC_IR_COMPUTE_OPERATOR_ARGMIN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ArgMin : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kReduced = 0
  };

  static char ID;

public:
  ArgMin();

  // clang-format off
  ArgMin(const IntAttr& pAxis,
         const IntAttr& pKeepdims);

  // clang-format on

  // shallow copy constructor.
  ArgMin(const ArgMin &pCopy);

  ~ArgMin() { }

  // clang-format off
  // Attributes getters
  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getKeepdims() const { return m_Keepdims; }


  // Attributes setters
  void setAxis(const IntAttr& pAxis) { m_Axis = pAxis; }

  void setKeepdims(const IntAttr& pKeepdims) { m_Keepdims = pKeepdims; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getData() { return getInput(kData); }


  // Outputs getters
  Tensor* getReduced() { return getOutput(kReduced); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setReduced(Tensor& pTensor) { m_Outputs[kReduced] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntAttr m_Axis;
  IntAttr m_Keepdims;
  // clang-format on
};

} // namespace of onnc

#endif
