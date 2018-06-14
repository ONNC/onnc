//===- Pow.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_POW_H
#define ONNC_IR_COMPUTE_OPERATOR_POW_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Pow : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 1,
    kZ = 0
  };

public:
  Pow();

  Pow(const IntAttr& pAxis,
      const IntAttr& pBroadcast);

  ~Pow() { }

  const IntAttr& getAxis() const { return m_Axis; }

  const IntAttr& getBroadcast() const { return m_Broadcast; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getX() { return getInput(kX); }

  Tensor* getY() { return getInput(kY); }

  Tensor* getZ() { return getOutput(kZ); }

  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }

  void setY(Tensor& pTensor) { m_Inputs[kY] = &pTensor; }

  void setZ(Tensor& pTensor) { m_Outputs[kZ] = &pTensor; }

  void print(std::ostream& pOS) const;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  IntAttr m_Axis;
  IntAttr m_Broadcast;
};

} // namespace of onnc

#endif
