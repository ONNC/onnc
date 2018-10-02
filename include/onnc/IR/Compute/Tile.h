//===- Tile.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_TILE_H
#define ONNC_IR_COMPUTE_OPERATOR_TILE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Tile : public ComputeOperator
{
public:
  enum IOConst {
    kInput = 0,
    kRepeats = 1,
    kOutput = 0
  };

  static char ID;

public:
  Tile();

  // clang-format off
  
  // clang-format on

  // shallow copy constructor.
  Tile(const Tile &pCopy);

  virtual ~Tile() { }

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
  const Tensor* getInput() const { return getInput(kInput); }

  const Tensor* getRepeats() const { return getInput(kRepeats); }

  Tensor* getInput() { return getInput(kInput); }

  Tensor* getRepeats() { return getInput(kRepeats); }


  // Outputs getters
  const Tensor* getOutput() const { return getOutput(kOutput); }

  Tensor* getOutput() { return getOutput(kOutput); }


  // Inputs setters
  void setInput(Tensor& pTensor) { m_Inputs[kInput] = &pTensor; }

  void setRepeats(Tensor& pTensor) { m_Inputs[kRepeats] = &pTensor; }


  // Outputs setters
  void setOutput(Tensor& pTensor) { m_Outputs[kOutput] = &pTensor; }

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
