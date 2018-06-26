//===- Slice.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_SLICE_H
#define ONNC_IR_COMPUTE_OPERATOR_SLICE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Slice : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kOutput = 0
  };

public:
  Slice();

  Slice(const IntsAttr& pAxes,
        const IntsAttr& pEnds,
        const IntsAttr& pStarts);

  ~Slice() { }

  const IntsAttr& getAxes() const { return m_Axes; }

  const IntsAttr& getEnds() const { return m_Ends; }

  const IntsAttr& getStarts() const { return m_Starts; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getOutput() { return getOutput(kOutput); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }
private:
  IntsAttr m_Axes;
  IntsAttr m_Ends;
  IntsAttr m_Starts;
};

} // namespace of onnc

#endif
