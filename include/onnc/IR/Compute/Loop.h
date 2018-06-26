//===- Loop.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_LOOP_H
#define ONNC_IR_COMPUTE_OPERATOR_LOOP_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Loop : public ComputeOperator
{
public:
  enum IOConst {
    kM = 0,
    kCond = 1,
    kVInitial = 2,
    kVFinalAndScanOutputs = 0
  };

public:
  Loop();

  Loop(const GraphAttr& pBody);

  ~Loop() { }

  const GraphAttr& getBody() const { return m_Body; }

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  Tensor* getM() { return getInput(kM); }

  Tensor* getCond() { return getInput(kCond); }

  Tensor* getVInitial(size_t pIdx) { return getInput(kVInitial + pIdx); }

  Tensor* getVFinalAndScanOutputs(size_t pIdx) { return getOutput(kVFinalAndScanOutputs + pIdx); }

  void setM(Tensor& pTensor) { m_Inputs[kM] = &pTensor; }

  void setCond(Tensor& pTensor) { m_Inputs[kCond] = &pTensor; }

  void setVInitial(size_t pIdx, Tensor& pTensor) { m_Inputs[kVInitial + pIdx] = &pTensor; }

  void setVFinalAndScanOutputs(size_t pIdx, Tensor& pTensor) { m_Outputs[kVFinalAndScanOutputs + pIdx] = &pTensor; }

  void print(std::ostream& pOS) const override;

  void dump() const { print(errs()); }

  void accept(ComputeVisitor& pVisitor) { pVisitor.visit(*this); }

private:
  GraphAttr m_Body;
};

} // namespace of onnc

#endif
