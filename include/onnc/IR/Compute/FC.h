//===- FC.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_FC_H
#define ONNC_IR_COMPUTE_OPERATOR_FC_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class FC : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kW = 1,
    kB = 2,
    kY = 0
  };

public:
  FC();

  FC(const IntAttr& pAxis,
     const IntAttr& pAxisW);

  ~FC() { }

  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getAxisW() const { return m_AxisW; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getW() { return getInput(kW); }

  Tensor* getB() { return getInput(kB); }

  Tensor* getY() { return getOutput(kY); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setW(Tensor& pTensor) { m_Inputs[kW] = &pTensor; }

  void setB(Tensor& pTensor) { m_Inputs[kB] = &pTensor; }

  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  IntAttr m_Axis;
  IntAttr m_AxisW;
};

} // namespace of onnc

#endif
