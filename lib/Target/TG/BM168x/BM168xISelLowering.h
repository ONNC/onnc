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
#pragma once

#include "BM168xBackend.h"
#include "TargetLowering.h"
#include <memory>
#include <onnx/common/ir.h>

namespace onnc {

class BM168xBackend;

class BM168xTargetLowering : public TargetLowering
{
public:
  BM168xTargetLowering(TGBackend *pBackend) : TargetLowering(pBackend) {}

  ComputeOperator2 *LowerOperation(
      const ::onnx::Node &pNode,
      std::vector<std::unique_ptr<ComputeOperator2> > &pInstList) override;

private:
  ComputeOperator2 *LowerHelper(const ::onnx::Node &pNode);
};

} // namespace onnc
