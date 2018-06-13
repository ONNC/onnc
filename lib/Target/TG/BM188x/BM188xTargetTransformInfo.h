//===- BM188xTargetTransformInfo.h ----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TG188x_TARGET_TRANSFORMINFO_H
#define ONNC_TG188x_TARGET_TRANSFORMINFO_H
#include <onnc/Target/TargetTransformInfo.h>

namespace onnc {

class TGBackend;
class BM188xTargetTransformInfo : public TargetTransformInfo
{
public:
  BM188xTargetTransformInfo(TGBackend *pTGBackend) : m_pTGBackend(pTGBackend){};
  int getOperatorCost(const onnx::Node *pNode,
                      enum TargetCostKind pKind) const override;

  int getWarpSize() const override;

  int getProcessingUnitCount() const override;
  int getBusBitWidth() const;

private:
  TGBackend *m_pTGBackend; // NOLINT
};

} // namespace onnc

#endif
