//===- ReduceLogSum.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_REDUCELOGSUM_H
#define ONNC_IR_COMPUTE_OPERATOR_REDUCELOGSUM_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ReduceLogSum : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kReduced = 0
  };

  static char ID;

public:
  ReduceLogSum();

  ReduceLogSum(const IntsAttr& pAxes,
               const IntAttr& pKeepdims);

  ~ReduceLogSum() { }

  const IntsAttr& getAxes() const { return m_Axes; }

  const IntAttr& getKeepdims() const { return m_Keepdims; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getReduced() { return getOutput(kReduced); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setReduced(Tensor& pTensor) { m_Outputs[kReduced] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  IntsAttr m_Axes;
  IntAttr m_Keepdims;
};

} // namespace of onnc

#endif
