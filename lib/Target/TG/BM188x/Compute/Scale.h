//===- Scale.h ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_SCALE_H
#define ONNC_TARGET_BM1880_SCALE_H

#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/Compute/Scale.h>

namespace onnc {
namespace BM188X {

class Scale : public onnc::Scale
{
public:
  Scale();

  const IntAttr &getRShiftWidth() const { return m_RShiftWidth; }

  void setRShiftWidth(const IntAttr &pRSW) { m_RShiftWidth = pRSW; }

  void print(std::ostream& pOS) const override;

  void accept(ComputeVisitor& pV) override;

  void accept(ComputeVisitor& pV) const override;
  
private:
  IntAttr m_RShiftWidth;
};

} // namespace BM188X
} // namespace onnc

#endif
