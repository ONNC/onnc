//===- Dropout.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_DROPOUT_H
#define ONNC_IR_COMPUTE_OPERATOR_DROPOUT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Dropout : public ComputeOperator
{
public:
  enum IOConst {
    kData = 0,
    kOutput = 0,
    kMask = 1
  };

  static char ID;

public:
  Dropout();

  // clang-format off
  Dropout(const FloatAttr& pRatio);

  // clang-format on

  // shallow copy constructor.
  Dropout(const Dropout &pCopy);

  ~Dropout() { }

  // clang-format off
  // Attributes getters
  const FloatAttr& getRatio() const { return m_Ratio; }


  // Attributes setters
  void setRatio(const FloatAttr& pRatio) { m_Ratio = pRatio; }

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

  Tensor* getMask() { return getOutput(kMask); }


  // Inputs setters
  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }


  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void setMask(Tensor& pTensor) { m_Outputs[kMask] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  FloatAttr m_Ratio;
  // clang-format on
};

} // namespace of onnc

#endif
