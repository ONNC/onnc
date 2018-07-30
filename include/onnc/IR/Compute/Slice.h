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

  static char ID;

public:
  Slice(const IntsAttr& pEnds, const IntsAttr& pStarts);

  // clang-format off
  Slice(const IntsAttr& pAxes,
        const IntsAttr& pEnds,
        const IntsAttr& pStarts);

  // clang-format on

  // shallow copy constructor.
  Slice(const Slice &pCopy);

  ~Slice() { }

  // clang-format off
  // Attributes getters
  const IntsAttr& getAxes() const { return m_Axes; }

  const IntsAttr& getEnds() const { return m_Ends; }

  const IntsAttr& getStarts() const { return m_Starts; }


  // Attributes setters
  void setAxes(const IntsAttr& pAxes) { m_Axes = pAxes; }

  void setEnds(const IntsAttr& pEnds) { m_Ends = pEnds; }

  void setStarts(const IntsAttr& pStarts) { m_Starts = pStarts; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getData() { return getInput(kData); }


  // Outputs getters
  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntsAttr m_Axes;
  IntsAttr m_Ends;
  IntsAttr m_Starts;
  // clang-format on
};

} // namespace of onnc

#endif
