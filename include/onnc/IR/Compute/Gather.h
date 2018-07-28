//===- Gather.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_GATHER_H
#define ONNC_IR_COMPUTE_OPERATOR_GATHER_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Gather : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kIndices = 1,
    kOutput = 0
  };

  static char ID;

public:
  Gather();

  Gather(const IntAttr& pAxis);

  ~Gather() { }

  const IntAttr& getAxis() const { return m_Axis; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getIndices() { return getInput(kIndices); }

  Tensor* getOutput() { return getOutput(kOutput); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setIndices(Tensor& pTensor) { m_Inputs[kIndices] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  IntAttr m_Axis;
};

} // namespace of onnc

#endif
