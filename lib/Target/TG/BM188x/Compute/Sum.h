//===- Sum.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_SUM_H
#define ONNC_TARGET_BM1880_SUM_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Sum.h>

namespace onnc {
namespace BM188X {

class Sum : public onnc::Sum
{
public:
  Sum();

  const IntAttr &getRShiftWidth() const { return m_RShiftWidth; }

  const IntAttr &getDoRelu() const { return m_DoRelu; }

  const IntsAttr &getThresholdXQuantized() const {
    return m_ThresholdXQuantized;
  }

  void setRShiftWidth(const IntAttr &pRSW) { m_RShiftWidth = pRSW; }

  void setDoRelu(const IntAttr &pDoRelu) { m_DoRelu = pDoRelu; }

  void setThresholdXQuantized(const IntsAttr &pT ) {
    m_ThresholdXQuantized = pT;
  }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;

private:
  IntAttr m_RShiftWidth;
  IntAttr m_DoRelu;
  IntsAttr m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
