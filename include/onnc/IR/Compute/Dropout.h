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

public:
  Dropout();

  Dropout(const IntAttr& pIsTest,
          const FloatAttr& pRatio);

  ~Dropout() { }

  const IntAttr& getIsTest() const { return m_IsTest; }

  const FloatAttr& getRatio() const { return m_Ratio; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getData() { return getInput(kData); }

  Tensor* getOutput() { return getOutput(kOutput); }

  Tensor* getMask() { return getOutput(kMask); }

  void setData(Tensor& pTensor) { m_Inputs[kData] = &pTensor; }

  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

  void setMask(Tensor& pTensor) { m_Outputs[kMask] = &pTensor; }

  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  IntAttr m_IsTest;
  FloatAttr m_Ratio;
};

} // namespace of onnc

#endif
