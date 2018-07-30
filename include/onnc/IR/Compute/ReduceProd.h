//===- ReduceProd.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_REDUCEPROD_H
#define ONNC_IR_COMPUTE_OPERATOR_REDUCEPROD_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ReduceProd : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kReduced = 0
  };

  static char ID;

public:
  ReduceProd();

  // clang-format off
  ReduceProd(const IntsAttr& pAxes,
             const IntAttr& pKeepdims);

  // clang-format on

  // shallow copy constructor.
  ReduceProd(const ReduceProd &pCopy);

  ~ReduceProd() { }

  // clang-format off
  // Attributes getters
  const IntsAttr& getAxes() const { return m_Axes; }

  const IntAttr& getKeepdims() const { return m_Keepdims; }


  // Attributes setters
  void setAxes(const IntsAttr& pAxes) { m_Axes = pAxes; }

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
  IntsAttr m_Axes;
  IntAttr m_Keepdims;
  // clang-format on
};

} // namespace of onnc

#endif
