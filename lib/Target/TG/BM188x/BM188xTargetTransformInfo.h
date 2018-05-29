//===- BM188xTargetTransformInfo.h ----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TG188x_TARGET_TRANSFORMINFO_H
#define ONNC_TG188x_TARGET_TRANSFORMINFO_H
#include "TGBackend.h"
#include <onnc/Target/TargetTransformInfo.h>

namespace onnc {

class BM188xComputeTTI : public TargetTransformInfo
{
public:
  BM188xComputeTTI(TGBackend *pTGBackend) : m_pTGBackend(pTGBackend){};
  int getOperatorCost(const onnx::Node *pNode,
                      enum TargetCostKind kind) const override;

  int getWarpSize() const override;

  int getProcessingUnitCount() const override;

private:
  TGBackend *m_pTGBackend;
};

class BM188xGraphTTI : public TargetTransformInfo
{
public:
  BM188xGraphTTI(TGBackend *pTGBackend) : m_pTGBackend(pTGBackend){};
  int getOperatorCost(const onnx::Node *pNode,
                      enum TargetCostKind kind) const override;

  int getWarpSize() const override;

  int getProcessingUnitCount() const override;
  int getBusBitWidth() const;

private:
  TGBackend *m_pTGBackend;
};

} // namespace onnc

#endif
