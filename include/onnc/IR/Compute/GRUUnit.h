//===- GRUUnit.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_GRUUNIT_H
#define ONNC_IR_COMPUTE_OPERATOR_GRUUNIT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class GRUUnit : public ComputeOperator
{
public:
  enum IOConst {
    kHiddenPrev = 0,
    kGates = 1,
    kSeqLengths = 2,
    kT = 3,
    kHidden = 0
  };

public:
  GRUUnit();

  GRUUnit(const IntAttr& pDropStates);

  ~GRUUnit() { }

  const IntAttr& getDropStates() const { return m_DropStates; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getHiddenPrev() { return getInput(kHiddenPrev); }

  Tensor* getGates() { return getInput(kGates); }

  Tensor* getSeqLengths() { return getInput(kSeqLengths); }

  Tensor* getT() { return getInput(kT); }

  Tensor* getHidden() { return getOutput(kHidden); }

  void setHiddenPrev(Tensor& pTensor) { m_Inputs[kHiddenPrev] = &pTensor; }

  void setGates(Tensor& pTensor) { m_Inputs[kGates] = &pTensor; }

  void setSeqLengths(Tensor& pTensor) { m_Inputs[kSeqLengths] = &pTensor; }

  void setT(Tensor& pTensor) { m_Inputs[kT] = &pTensor; }

  void setHidden(Tensor& pTensor) { m_Outputs[kHidden] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }
private:
  IntAttr m_DropStates;
};

} // namespace of onnc

#endif
