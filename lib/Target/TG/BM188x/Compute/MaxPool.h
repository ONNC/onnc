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
#include <onnc/IR/Compute/MaxPool.h>

namespace onnc {
namespace BM188X {

class MaxPool : public onnc::MaxPool
{
public:
  static char ID;

public:
  MaxPool(const IntsAttr& pKS);

  void setRShiftWidth(int pRShiftWidth) { m_RShiftWidth = pRShiftWidth; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  void setThresholdXQuantized(int pTXQ) { m_ThresholdXQuantized = pTXQ; }

  int getThresholdXQuantized() const { return m_ThresholdXQuantized; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator* pOp);

private:
  int m_RShiftWidth;
  int m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
