//===- Squeeze.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_SQUEEZE_H
#define ONNC_IR_COMPUTE_OPERATOR_SQUEEZE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Squeeze : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kSqueezed = 0
  };

public:
  Squeeze();

  Squeeze(const IntsAttr& pAxes);

  ~Squeeze() { }

  const IntsAttr& getAxes() const { return m_Axes; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getSqueezed() { return getOutput(kSqueezed); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setSqueezed(Tensor& pTensor) { m_Outputs[kSqueezed] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

private:
  IntsAttr m_Axes;
};

} // namespace of onnc

#endif
