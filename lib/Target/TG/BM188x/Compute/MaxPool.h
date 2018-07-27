//===- MaxPool.h ----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGMAXPOOL_H
#define ONNC_TARGET_BM1880_TGMAXPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>

namespace onnc {
namespace BM188X {

class MaxPool : public ComputeOperator
{
public:
  MaxPool();

  void setKernelShape(const IntsAttr &pKS) { m_KernelShape = pKS; }

  const IntsAttr &getKernelShape() const { return m_KernelShape; }

  void setPads(const IntsAttr &pPads) { m_Pads = pPads; }

  const IntsAttr &getPads() const { return m_Pads; }

  void setStrides(const IntsAttr &pStrides) { m_Strides = pStrides; }

  const IntsAttr &getStrides() const { return m_Strides; }

  void setRShiftWidth(int pRShiftWidth) { m_RShiftWidth = pRShiftWidth; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  void setThresholdXQuantized(int pTXQ) { m_ThresholdXQuantized = pTXQ; }

  int getThresholdXQuantized() const { return m_ThresholdXQuantized; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

private:
  IntsAttr m_KernelShape;
  IntsAttr m_Pads;
  IntsAttr m_Strides;
  int m_RShiftWidth;
  int m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
