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

  static char ID;

public:
  Squeeze();

  // clang-format off
  Squeeze(const IntsAttr& pAxes);

  // clang-format on

  // shallow copy constructor.
  Squeeze(const Squeeze &pCopy);

  ~Squeeze() { }

  // clang-format off
  // Attributes getters
  const IntsAttr& getAxes() const { return m_Axes; }


  // Attributes setters
  void setAxes(const IntsAttr& pAxes) { m_Axes = pAxes; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getData() { return getInput(kData); }


  // Outputs getters
  Tensor* getSqueezed() { return getOutput(kSqueezed); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setSqueezed(Tensor& pTensor) { m_Outputs[kSqueezed] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  IntsAttr m_Axes;
  // clang-format on
};

} // namespace of onnc

#endif
