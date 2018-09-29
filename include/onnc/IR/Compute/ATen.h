//===- ATen.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_ATEN_H
#define ONNC_IR_COMPUTE_OPERATOR_ATEN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class ATen : public ComputeOperator
{
public:
  enum IOConst {
    kInInput = 0,
    kOutOutput = 0
  };

  static char ID;

public:
  ATen();

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  ATen(const ATen &pCopy);

  virtual ~ATen() { }

  // clang-format off
  // Attributes getters
  

  // Attributes setters
  
  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  const Tensor* getInInput(size_t pIdx) const { return getInput(kInInput + pIdx); }

  Tensor* getInInput(size_t pIdx) { return getInput(kInInput + pIdx); }


  // Outputs getters
  const Tensor* getOutOutput(size_t pIdx) const { return getOutput(kOutOutput + pIdx); }

  Tensor* getOutOutput(size_t pIdx) { return getOutput(kOutOutput + pIdx); }


  // Inputs setters
  void setInInput(size_t pIdx, Tensor& pTensor) { m_Inputs[kInInput + pIdx] = &pTensor; }


  // Outputs setters
  void setOutOutput(size_t pIdx, Tensor& pTensor) { m_Outputs[kOutOutput + pIdx] = &pTensor; }

  // clang-format on

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  
  // clang-format on
};

} // namespace of onnc

#endif
