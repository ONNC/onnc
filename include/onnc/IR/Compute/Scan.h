//===- Scan.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_SCAN_H
#define ONNC_IR_COMPUTE_OPERATOR_SCAN_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Scan : public ComputeOperator
{
public:
  enum IOConst {
    kSequenceLens = 0,
    kInitialStateAndScanInputs = 1,
    kFinalStateAndScanOutputs = 0
  };

  static char ID;

public:
  Scan(const GraphAttr& pBody, const IntAttr& pNumScanInputs);

  // clang-format off
  Scan(const GraphAttr& pBody,
       const IntsAttr& pDirections,
       const IntAttr& pNumScanInputs);

  // clang-format on

  // shallow copy constructor.
  Scan(const Scan &pCopy);

  virtual ~Scan() { }

  // clang-format off
  // Attributes getters
  const GraphAttr& getBody() const { return m_Body; }

  const IntsAttr& getDirections() const { return m_Directions; }

  const IntAttr& getNumScanInputs() const { return m_NumScanInputs; }


  // Attributes setters
  void setBody(const GraphAttr& pBody) { m_Body = pBody; }

  void setDirections(const IntsAttr& pDirections) { m_Directions = pDirections; }

  void setNumScanInputs(const IntAttr& pNumScanInputs) { m_NumScanInputs = pNumScanInputs; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  const Tensor* getSequenceLens() const { return getInput(kSequenceLens); }

  const Tensor* getInitialStateAndScanInputs(size_t pIdx) const { return getInput(kInitialStateAndScanInputs + pIdx); }

  Tensor* getSequenceLens() { return getInput(kSequenceLens); }

  Tensor* getInitialStateAndScanInputs(size_t pIdx) { return getInput(kInitialStateAndScanInputs + pIdx); }


  // Outputs getters
  const Tensor* getFinalStateAndScanOutputs(size_t pIdx) const { return getOutput(kFinalStateAndScanOutputs + pIdx); }

  Tensor* getFinalStateAndScanOutputs(size_t pIdx) { return getOutput(kFinalStateAndScanOutputs + pIdx); }


  // Inputs setters
  void setSequenceLens(Tensor& pTensor) { m_Inputs[kSequenceLens] = &pTensor; }

  void setInitialStateAndScanInputs(size_t pIdx, Tensor& pTensor) { m_Inputs[kInitialStateAndScanInputs + pIdx] = &pTensor; }


  // Outputs setters
  void setFinalStateAndScanOutputs(size_t pIdx, Tensor& pTensor) { m_Outputs[kFinalStateAndScanOutputs + pIdx] = &pTensor; }

  // clang-format on

  void printAttributes(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

protected:
  // clang-format off
  GraphAttr m_Body;
  IntsAttr m_Directions;
  IntAttr m_NumScanInputs;
  // clang-format on
};

} // namespace of onnc

#endif
