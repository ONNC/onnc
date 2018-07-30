//===- Upsample.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_UPSAMPLE_H
#define ONNC_IR_COMPUTE_OPERATOR_UPSAMPLE_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class Upsample : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 0
  };

  static char ID;

public:
  Upsample(const FloatsAttr& pScales);

  // clang-format off
  Upsample(const StringAttr& pMode,
           const FloatsAttr& pScales);

  // clang-format on

  // shallow copy constructor.
  Upsample(const Upsample &pCopy);

  ~Upsample() { }

  // clang-format off
  // Attributes getters
  const StringAttr& getMode() const { return m_Mode; }

  const FloatsAttr& getScales() const { return m_Scales; }


  // Attributes setters
  void setMode(const StringAttr& pMode) { m_Mode = pMode; }

  void setScales(const FloatsAttr& pScales) { m_Scales = pScales; }

  // clang-format on

  Tensor* getInput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  const Tensor* getInput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Inputs[pIdx]); }

  Tensor* getOutput(unsigned int pIdx) override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  const Tensor* getOutput(unsigned int pIdx) const override { return static_cast<Tensor*>(m_Outputs[pIdx]); }

  // clang-format off
  // Inputs getters
  Tensor* getX() { return getInput(kX); }


  // Outputs getters
  Tensor* getY() { return getOutput(kY); }


  // Inputs setters
  void setX(Tensor& pTensor) { m_Inputs[kX] = &pTensor; }


  // Outputs setters
  void setY(Tensor& pTensor) { m_Outputs[kY] = &pTensor; }

  // clang-format on

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

  static bool classof(const ComputeOperator* pOp);

private:
  // clang-format off
  StringAttr m_Mode;
  FloatsAttr m_Scales;
  // clang-format on
};

} // namespace of onnc

#endif
