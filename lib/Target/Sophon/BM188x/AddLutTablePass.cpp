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
char BM188X::AddLutTablePass::ID = 0;

Pass::ReturnType
BM188X::AddLutTablePass::runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG)
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
      onnx::Tensor lutTensor;
      lutTensor.sizes().push_back(256 * numNPU);
      lutTensor.elem_type() = onnx::TensorProto_DataType_INT8;
      ::onnx::Value *onxLutV = pTG.addInitializerAndInput(lutTensor, name);
      // FIXME: Do we need to call onnx::Node's addInput?
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