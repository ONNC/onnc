//===- BMScale.h
//----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_BM1880_BMSCALE_H
#define ONNC_TARGET_BM1880_BMSCALE_H

#include <onnc/IR/Compute/Attributes.h>
#include <onnc/IR/ComputeOperator.h>

namespace onnc {
namespace BM188X {

class BMScale : public ComputeOperator
{
public:
  static char ID;

public:
  BMScale();

  const IntAttr &getRShiftWidth() const { return m_RShiftWidth; }

  void setRShiftWidth(const IntAttr &pRSW) { m_RShiftWidth = pRSW; }

  void print(std::ostream &pOS) const override;

  void accept(ComputeVisitor &pV) override;

  void accept(ComputeVisitor &pV) const override;

  static bool classof(const ComputeOperator *pOp);

private:
  IntAttr m_RShiftWidth;
};

} // namespace BM188X
} // namespace onnc

#endif
