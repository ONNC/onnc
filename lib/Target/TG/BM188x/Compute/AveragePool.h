//===- AveragePool.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_TGAVERAGEPOOL_H
#define ONNC_TARGET_BM1880_TGAVERAGEPOOL_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/AveragePool.h>

namespace onnc {
namespace BM188X {

/** \class AveragePool
 *  \brief BM188X-dependent AveragePool operator
 */
class AveragePool : public onnc::AveragePool
{
public:
  AveragePool(const IntsAttr& pKernelShape);

  void setEnableRelu(const IntAttr& pEnableRelu) { m_EnableRelu = pEnableRelu; }

  const IntAttr& getEnableRelu() const { return m_EnableRelu; }

  int getRShiftWidth() const { return m_RShiftWidth; }

  void setRShiftWidth(int pValue) { m_RShiftWidth = pValue; }

  int getThresholdXQuantized() const { return m_ThresholdXQuantized; }

  void setThresholdXQuantized(int pValue) { m_ThresholdXQuantized = pValue; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pVisitor) override { pVisitor.visit(*this); }

  void accept(ComputeVisitor& pVisitor) const override { pVisitor.visit(*this); }

private:
  IntAttr m_EnableRelu;
  int m_RShiftWidth;
  int m_ThresholdXQuantized;
};

} // namespace BM188X
} // namespace onnc

#endif
