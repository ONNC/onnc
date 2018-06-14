//===- Div.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_DIV_H
#define ONNC_IR_COMPUTE_OPERATOR_DIV_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Div : public ComputeOperator
{
public:
  enum IOConst {
    kA = 0,
    kB = 1,
    kC = 0
  };

public:
  Div();

  Div(const IntAttr& pAxis,
      const IntAttr& pBroadcast);

  ~Div() { }

  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getBroadcast() const { return m_Broadcast; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getA() { return getInput(kA); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getC() { return getOutput(kC); }

  void setA(Tensor& pTensor) { m_Inputs[kA] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setC(Tensor& pTensor) { m_Outputs[kC] = &pTensor; }

  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  IntAttr m_Axis;
  IntAttr m_Broadcast;
};

} // namespace of onnc

#endif
