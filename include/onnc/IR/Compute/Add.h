//===- Add.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_ADD_H
#define ONNC_IR_COMPUTE_OPERATOR_ADD_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Add : public ComputeOperator
{
public:
  enum IOConst {
    kA = 0,
    kB = 1,
    kC = 0
  };

public:
  Add();

  Add(const IntAttr& pAxis,
      const IntAttr& pBroadcast);

  Add(const Add& pCopy);

  ~Add() { }

  const IntAttr& getAxis() const { return m_Axis; }

  void setAxis(const IntAttr& pAxis) { m_Axis = pAxis; }

  const IntAttr& getBroadcast() const { return m_Broadcast; }

  void setBroadcast(const IntAttr& pB) { m_Broadcast = pB; }

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

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

private:
  IntAttr m_Axis;
  IntAttr m_Broadcast;
};

} // namespace of onnc

#endif
