//===- NvDlaFuseGemmReluPass.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaFuseGemmReluPass.h"

#include "Compute/NvDlaGemmRelu.h"

#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Relu.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// NvDlaFuseGemmReluPass
//===----------------------------------------------------------------------===//
Pass::ReturnType NvDlaFuseGemmReluPass::runOnModule(Module& pModule)
{
  Pass::ReturnType    ret = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    ret |= runOnComputeGraph(*cg->value());

  return ret;
}

Pass::ReturnType NvDlaFuseGemmReluPass::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType       ret = Pass::kModuleNoChanged;
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator* node = nodeIt;

    if (!isFusible(*node))
      continue;

    // Create GemmRelu to fuse Gemm and Relu.
    Gemm& gemm = *(Gemm*)node;
    Relu& relu = *(Relu*)gemm.getOutput(0)->getUses()[0].getUser();

    mergeGemmRelu(pCG, gemm, relu);

    pCG.erase(gemm);
    pCG.erase(relu);

    ret |= Pass::kModuleChanged;
  }

  pCG.topologicalSort();

  return ret;
}

bool NvDlaFuseGemmReluPass::isFusible(ComputeOperator& pNode)
{
  if (!isa<Gemm>(&pNode))
    return false;

  Value* outv = pNode.getOutput(0);

  // if Gemm's result has more than one users, we can't fuse it.
  if (outv->getUses().size() > 1)
    return false;

  ComputeOperator* userNode = outv->getUses()[0].getUser();
  if (!isa<Relu>(userNode))
    return false;
  return true;
}

NvDlaGemmRelu* NvDlaFuseGemmReluPass::mergeGemmRelu(ComputeGraph& pCG, Gemm& pGemm, Relu& pRelu)
{
  Value* outv     = pRelu.getOutput(0);
  Value* out_gemm = pGemm.getOutput(0);
  pGemm.replaceOutput(0, *outv);
  pCG.erase(*out_gemm);
  // FIXME: need move newOp to correct position.
  NvDlaGemmRelu* newOp  = pCG.addOperator<NvDlaGemmRelu>(pGemm, pRelu);
  Value*         emptyV = new Value;

  for (unsigned i = 0; i < pGemm.getNumOfInputs(); ++i) {
    newOp->addInput(*pGemm.getInput(i));

    // FIXME: need implement ComputeOperator::removeAllInputs.
    pGemm.replaceInput(i, *emptyV);
  }
  pRelu.replaceInput(0, *emptyV);

  outv->clearDefine();
  newOp->addOutput(*outv);

  return newOp;
}

namespace onnc {
INITIALIZE_PASS(NvDlaFuseGemmReluPass, "NvDlaFuseGemmReluPass")
}
