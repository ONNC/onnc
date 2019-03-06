//===- AddLutTablePass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "AddLutTablePass.h"
#include "Compute/LRN.h"
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Config/ONNX.h>
#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/IR/Compute/Value.h>
#include <onnc/IR/IRBuilder.h>
#include <onnc/Target/TargetTransformInfo.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// AddLutTablePass
//===----------------------------------------------------------------------===//
Pass::ReturnType
BM188X::AddLutTablePass::runOnGraphs(xGraph& pTG, ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  const int numNPU = m_pTarget->getTTI()->getWarpSize();

  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    onnc::ComputeOperator *node = nodeIt;
    if (!isa<BM188X::LRN>(node))
      continue;

    const auto &oName = node->getOutput(0)->getName();

    auto addLutWeight = [&](const std::string& name) {
      xTensor lutTensor;
      lutTensor.sizes().push_back(256 * numNPU);
      lutTensor.elem_type() = (xTensorProtoDataType)kInt8;
      xValue *onxLutV = pTG.addInitializerAndInput(lutTensor, name);
      // FIXME: Do we need to call xNode's addInput?
      // onnxNode.addInput(onxLutV);

      Initializer* init = pCG.addOperator<onnc::Initializer>(name);
      Tensor* t = IRBuilder::CreateComputeTensor(pCG, *onxLutV, lutTensor);
      init->setTensor(*t);
      node->addInput(*t);
    };

    // add SQR and POWER LUT table
    addLutWeight(oName + "_sqrlut");
    addLutWeight(oName + "_powerlut");

    ret = Pass::kModuleChanged;
  }
  return ret;
}

ModulePass *BM188X::CreateAddLutTablePass(DLATargetBackend* pTarget)
{
  return new AddLutTablePass(pTarget);
}
