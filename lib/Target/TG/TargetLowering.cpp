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
#include "TargetLowering.h"
#include <onnx/common/ir.h>

using namespace onnc;

void TargetLowering::ISelLowering(const ::onnx::Graph *pOnnxGraph,
                                  TGBackend::Instructions& pInsns)
{
  for (const ::onnx::Node *node : pOnnxGraph->nodes()) {
    std::unique_ptr<ComputeOperator2> oper(LowerOperation(*node, pInsns));
    if (oper != nullptr)
      pInsns.push_back(std::move(oper));
  }
}
