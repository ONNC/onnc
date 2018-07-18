//===- TopK.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_TOPK_H
#define ONNC_IR_COMPUTE_OPERATOR_TOPK_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class TopK : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kValues = 0,
    kIndices = 1
  };

public:
  TopK();

  TopK(const IntAttr& pAxis,
       const IntAttr& pK);

  ~TopK() { }

  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getK() const { return m_K; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getValues() { return getOutput(kValues); }

  Tensor* getIndices() { return getOutput(kIndices); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setValues(Tensor& pTensor) { m_Outputs[kValues] = &pTensor; }

  void setIndices(Tensor& pTensor) { m_Outputs[kIndices] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }
private:
  IntAttr m_Axis;
  IntAttr m_K;
};

} // namespace of onnc

#endif
