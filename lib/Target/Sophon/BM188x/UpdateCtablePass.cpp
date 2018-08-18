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
#include "BM188xBackend.h"
#include "Compute/Conv.h"
#include "Compute/Gemm.h"
#include "Compute/LRN.h"
#include "Compute/MaxPool.h"
#include <onnc/Config/ONNX.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/InputOperator.h>
#include <onnc/IR/Compute/OutputOperator.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Softmax.h>
#include <onnc/Target/Sophon/BM188x/common_calibration2.pb.h>
#include <onnc/Transforms/GraphBuildingPass.h>
#include <onnx/common/ir.h>

using namespace onnc;

namespace {

class UpdateCtablePass : public GraphBuildingPass
{
public:
  static char ID;

public:
  UpdateCtablePass(BM1880Backend *pBackend)
      : GraphBuildingPass(ID), m_pBackend(pBackend)
  {
  }

  StringRef getPassName() const override { return "UpdateCtable"; }

  Pass::ReturnType runOnGraphs(xGraph &pTG, ComputeGraph &pCG) override;

private:
  BM1880Backend *m_pBackend; // NOLINT
};

} // namespace

Pass::ReturnType UpdateCtablePass::runOnGraphs(xGraph &pTG,
                                               ComputeGraph &pCG)
{
  auto nEnd = pCG.end();
  for (auto nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    onnc::ComputeOperator *node = nodeIt;
    if (isa<OutputOperator>(node) || isa<Initializer>(node) ||
        isa<InputOperator>(node))
      continue;

    const std::string ctableName = node->getOutput(0)->getName();
    const auto *layerCtable = m_pBackend->getLayerCtable(ctableName);
    if (auto *conv = dyn_cast<BM188X::Conv>(node)) {
      conv->setRShiftWidth(layerCtable->right_shift_width());
      conv->setScaleRShiftWidth(
          layerCtable->convolution_param().scale_right_shift_width());
      continue;
    } else if (auto *maxPool = dyn_cast<BM188X::MaxPool>(node)) {
      maxPool->setRShiftWidth(layerCtable->right_shift_width());
      maxPool->setThresholdXQuantized(
          *layerCtable->threshold_x_quantized().data());
      continue;
    } else if (auto *gemm = dyn_cast<BM188X::Gemm>(node)) {
      gemm->setRShiftWidth(layerCtable->right_shift_width());
      continue;
    } else if (isa<onnc::Reshape>(node) || isa<onnc::Softmax>(node)) {
      continue;
    } else if (auto *lrn = dyn_cast<BM188X::LRN>(node)) {
      std::string outputName = lrn->getOutput(0)->getName();
      for (int i = 0; i < layerCtable->blob_param_size(); ++i) {
        if (layerCtable->blob_param(i).name() == outputName) {
          lrn->setLrnRightShiftWidth(
              layerCtable->blob_param(i).right_shift_width());
        } else if (layerCtable->blob_param(i).name() == "sum_sq") {
          lrn->setSumRightShiftWidth(
              layerCtable->blob_param(i).right_shift_width());
        }
      }
      lrn->getThresholdXQuantized()[0] = layerCtable->threshold_x_quantized(0);
      lrn->getThresholdXQuantized()[1] = layerCtable->threshold_x_quantized(1);
      continue;
    }
    errs() << "FIXME: missed update " << ctableName << " operator\n";
  }
  return Pass::kModuleChanged;
}

char UpdateCtablePass::ID = 0;

ModulePass *onnc::createUpdateCtablePass(BM1880Backend *pBackend)
{
  return new UpdateCtablePass(pBackend);
}
