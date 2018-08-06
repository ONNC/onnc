//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#ifndef ONNC_TG188x_TARGET_TRANSFORMINFO_H
#define ONNC_TG188x_TARGET_TRANSFORMINFO_H
#include <onnc/Target/TargetTransformInfo.h>

namespace onnc {

class TGBackend;
class BM188xTargetTransformInfo : public TargetTransformInfo
{
public:
  enum TargetCostKind : unsigned {
    BUILTIN_COST_KIND_END = TargetTransformInfo::BUILTIN_COST_KIND_END,
    kCycleCount, ///< Get graph (or compute) IR cycle count.
    kOptOpCount, /// Optimal Operation number
    kOpCount,    /// Operation number
  };

  BM188xTargetTransformInfo(TGBackend *pTGBackend) : m_pTGBackend(pTGBackend){};
  uint64_t getOperatorCost(const xNode *pNode,
                           unsigned pKind) const override;

  int getWarpSize() const override;

  int getProcessingUnitCount() const override;
  int getBusBitWidth() const;

private:
  TGBackend *m_pTGBackend; // NOLINT
};

} // namespace onnc

#endif
