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
#ifndef ONNC_TARGET_BM168X_ISEL_LOWERING_H
#define ONNC_TARGET_BM168X_ISEL_LOWERING_H
#include "BM168xBackend.h"
#include "TargetLowering.h"
#include <onnc/Config/ONNX.h>
#include <memory>

namespace onnc {

class BM168xBackend;

class BM168xTargetLowering : public TargetLowering
{
public:
  BM168xTargetLowering(TGBackend *pBackend) : TargetLowering(pBackend) {}

  ComputeOperator2 *LowerOperation(
      const xNode &pNode,
      std::vector<std::unique_ptr<ComputeOperator2> > &pInstList) override;

private:
  ComputeOperator2 *LowerHelper(const xNode &pNode);
};

} // namespace onnc

#endif
