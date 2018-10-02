//===- Unsqueeze.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_UNSQUEEZE_H
#define ONNC_IR_COMPUTE_OPERATOR_UNSQUEEZE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Unsqueeze : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kExpanded = 0
  };

  static char ID;

public:
  Unsqueeze(const IntsAttr& pAxes);

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Unsqueeze(const Unsqueeze &pCopy);

  virtual ~Unsqueeze() { }

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
  const Tensor* getData() const { return getInput(kData); }

  Tensor* getData() { return getInput(kData); }


  // Outputs getters
  const Tensor* getExpanded() const { return getOutput(kExpanded); }

  Tensor* getExpanded() { return getOutput(kExpanded); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setExpanded(Tensor& pTensor) { m_Outputs[kExpanded] = &pTensor; }

  // clang-format on

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  IntsAttr m_Axes;
  // clang-format on
};

} // namespace of onnc

#endif
