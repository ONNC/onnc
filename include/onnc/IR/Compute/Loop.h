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

  static char ID;

public:
  Loop(const GraphAttr& pBody);

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Loop(const Loop &pCopy);

  ~Loop() { }

  // clang-format off
  // Attributes getters
  const GraphAttr& getBody() const { return m_Body; }


  // Attributes setters
  void setBody(const GraphAttr& pBody) { m_Body = pBody; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getM() { return getInput(kM); }

  Tensor* getCond() { return getInput(kCond); }

  Tensor* getVInitial(size_t pIdx) { return getInput(kVInitial + pIdx); }


  // Outputs getters
  Tensor* getVFinalAndScanOutputs(size_t pIdx) { return getOutput(kVFinalAndScanOutputs + pIdx); }


  // Inputs setters
  void setM(Tensor& pTensor) { m_Inputs[kM] = &pTensor; }

  void setCond(Tensor& pTensor) { m_Inputs[kCond] = &pTensor; }

  void setVInitial(size_t pIdx, Tensor& pTensor) { m_Inputs[kVInitial + pIdx] = &pTensor; }


  // Outputs setters
  void setVFinalAndScanOutputs(size_t pIdx, Tensor& pTensor) { m_Outputs[kVFinalAndScanOutputs + pIdx] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  GraphAttr m_Body;
  // clang-format on
};

} // namespace of onnc

#endif
