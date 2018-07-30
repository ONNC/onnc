//===- LpPool.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_OPERATOR_LPPOOL_H
#define ONNC_IR_COMPUTE_OPERATOR_LPPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeVisitor.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/Support/IOStream.h>

namespace onnc {

class LpPool : public ComputeOperator
{
public:
  enum IOConst {
    kX = 0,
    kY = 0
  };

  static char ID;

public:
  LpPool(const IntsAttr& pKernelShape);

  // clang-format off
  LpPool(const StringAttr& pAutoPad,
         const IntsAttr& pKernelShape,
         const IntAttr& pP,
         const IntsAttr& pPads,
         const IntsAttr& pStrides);

  // clang-format on

  // shallow copy constructor.
  LpPool(const LpPool &pCopy);

  ~LpPool() { }

  // clang-format off
  // Attributes getters
  const StringAttr& getAutoPad() const { return m_AutoPad; }

  const IntsAttr& getKernelShape() const { return m_KernelShape; }

  const IntAttr& getP() const { return m_P; }

  const IntsAttr& getPads() const { return m_Pads; }

  const IntsAttr& getStrides() const { return m_Strides; }


  // Attributes setters
  void setAutoPad(const StringAttr& pAutoPad) { m_AutoPad = pAutoPad; }

  void setKernelShape(const IntsAttr& pKernelShape) { m_KernelShape = pKernelShape; }

  void setP(const IntAttr& pP) { m_P = pP; }

  void setPads(const IntsAttr& pPads) { m_Pads = pPads; }

  void setStrides(const IntsAttr& pStrides) { m_Strides = pStrides; }

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
  StringAttr m_AutoPad;
  IntsAttr m_KernelShape;
  IntAttr m_P;
  IntsAttr m_Pads;
  IntsAttr m_Strides;
  // clang-format on
};

} // namespace of onnc

#endif
