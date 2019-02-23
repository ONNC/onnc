//===- NvDlaLayerFusionPass.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Gemm.h>
#include <onnc/IR/Compute/Relu.h>
#include <onnc/IR/Compute/MaxPool.h>
#include "Compute/NvDlaConvRelu.h"
#include "Compute/NvDlaGemmRelu.h"
#include "Compute/NvDlaConvReluMaxPool.h"
#include "NvDlaLayerFusionPass.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// FuseConvReluPass
//===----------------------------------------------------------------------===//
Pass::ReturnType NvDlaLayerFusionPass::runOnModule(Module& pModule)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    ret |= runOnComputeGraph(*cg->value());

  return ret;
}

Pass::ReturnType NvDlaLayerFusionPass::runOnComputeGraph(ComputeGraph& pCG)
{
  {
    errs() << "=============== runOnComputeGraph before ===========\n";
    ComputeGraph::iterator nodeIt, nEnd = pCG.end();
    for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
      ComputeOperator* node = nodeIt;
      node->print(errs());
      errs() << "\n";
    }
    errs() << "=============== runOnComputeGraph before ===========\n";
  }

  
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator* node = nodeIt;

    if (isFusibleForConvReluMaxPool(*node)) {
      // Create ConvReluMaxPool to fuse Conv, Relu, and MaxPool.
      Conv& conv = *(Conv *)node;
      Relu& relu = *(Relu *)conv.getOutput(0)->getUses()[0].getUser();
      MaxPool& maxpool = *(MaxPool *)relu.getOutput(0)->getUses()[0].getUser();
      
      mergeConvReluMaxPool(pCG, conv, relu, maxpool);

      pCG.erase(conv);
      pCG.erase(relu);
      pCG.erase(maxpool);

      ret |= Pass::kModuleChanged;
    }
    else if (isFusibleForConvRelu(*node)) {
      // Create ConvRelu to fuse Conv and Relu.
      Conv& conv = *(Conv *)node;
      Relu& relu = *(Relu *)conv.getOutput(0)->getUses()[0].getUser();

      mergeConvRelu(pCG, conv, relu);

      pCG.erase(conv);
      pCG.erase(relu);

      ret |= Pass::kModuleChanged;
    }
    else if (isFusibleForGemmRelu(*node)) {
      // Create GemmRelu to fuse Gemm and Relu.
      Gemm& gemm = *(Gemm *)node;
      Relu& relu = *(Relu *)gemm.getOutput(0)->getUses()[0].getUser();

      mergeGemmRelu(pCG, gemm, relu);

      pCG.erase(gemm);
      pCG.erase(relu);

      ret |= Pass::kModuleChanged;
    }    
  }

  pCG.topologicalSort();


  {
    errs() << "=============== runOnComputeGraph after ===========\n";
    ComputeGraph::iterator nodeIt, nEnd = pCG.end();
    for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
      ComputeOperator* node = nodeIt;
      node->print(errs());
      errs() << "\n";
    }
    errs() << "=============== runOnComputeGraph after ===========\n";
  }

  return ret;
}

bool NvDlaLayerFusionPass::isFusibleForConvReluMaxPool(ComputeOperator& pNode)
{
  if (!isa<Conv>(&pNode))
    return false;

  Value* outv = pNode.getOutput(0);

  // if Conv's result has more than one users, we can't fuse it.
  if (outv->getUses().size() > 1) {
    return false;
  }

  ComputeOperator* secondNode = outv->getUses()[0].getUser();
  if (!isa<Relu>(secondNode)) {
    return false;
  }
  
  outv = secondNode->getOutput(0);

  if (outv->getUses().size() > 1) {
    return false;
  }

  ComputeOperator* thirdNode = outv->getUses()[0].getUser();
  if (!isa<MaxPool>(thirdNode)) {
    return false;
  }
  
  return true;
}

bool NvDlaLayerFusionPass::isFusibleForConvRelu(ComputeOperator& pNode)
{
  if (!isa<Conv>(&pNode))
    return false;

  Value* outv = pNode.getOutput(0);

  // if Conv's result has more than one users, we can't fuse it.
  if (outv->getUses().size() > 1)
    return false;

  ComputeOperator* userNode = outv->getUses()[0].getUser();
  if (!isa<Relu>(userNode))
    return false;
  return true;
}


bool NvDlaLayerFusionPass::isFusibleForGemmRelu(ComputeOperator& pNode)
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


NvDlaConvReluMaxPool* NvDlaLayerFusionPass::mergeConvReluMaxPool(ComputeGraph& pCG,
                                                          Conv& pConv, Relu& pRelu, MaxPool& pMaxPool)
{
  Value* outv = pMaxPool.getOutput(0);
  Value* out_conv = pConv.getOutput(0);
  Value* out_relu = pRelu.getOutput(0);
  pConv.replaceOutput(0, *outv);
  pRelu.replaceOutput(0, *outv);
  pCG.erase(*out_conv);
  pCG.erase(*out_relu);
  // FIXME: need move newOp to correct position.
  NvDlaConvReluMaxPool* newOp = pCG.addOperator<NvDlaConvReluMaxPool>(pConv, pRelu, pMaxPool);
  Value* emptyV = new Value;

  for (unsigned i = 0; i < pConv.getNumOfInputs(); ++i) {
    newOp->addInput(*pConv.getInput(i));

    // FIXME: need implement ComputeOperator::removeAllInputs.
    pConv.replaceInput(i, *emptyV);
  }
  pRelu.replaceInput(0, *emptyV);
  pMaxPool.replaceInput(0, *emptyV);

  outv->clearDefine();
  newOp->addOutput(*outv);

  return newOp;
}

NvDlaConvRelu* NvDlaLayerFusionPass::mergeConvRelu(ComputeGraph& pCG,
                                                    Conv& pConv, Relu& pRelu)
{
  Value* outv = pRelu.getOutput(0);
  Value* out_conv = pConv.getOutput(0);
  pConv.replaceOutput(0, *outv);
  pCG.erase(*out_conv);
  // FIXME: need move newOp to correct position.
  NvDlaConvRelu* newOp = pCG.addOperator<NvDlaConvRelu>(pConv, pRelu);
  Value* emptyV = new Value;

  for (unsigned i = 0; i < pConv.getNumOfInputs(); ++i) {
    newOp->addInput(*pConv.getInput(i));

    // FIXME: need implement ComputeOperator::removeAllInputs.
    pConv.replaceInput(i, *emptyV);
  }
  pRelu.replaceInput(0, *emptyV);

  outv->clearDefine();
  newOp->addOutput(*outv);

  return newOp;
}


NvDlaGemmRelu* NvDlaLayerFusionPass::mergeGemmRelu(ComputeGraph& pCG,
                                                   Gemm& pGemm, Relu& pRelu)
{
  Value* outv = pRelu.getOutput(0);
  Value* out_gemm = pGemm.getOutput(0);
  pGemm.replaceOutput(0, *outv);
  pCG.erase(*out_gemm);
  // FIXME: need move newOp to correct position.
  NvDlaGemmRelu* newOp = pCG.addOperator<NvDlaGemmRelu>(pGemm, pRelu);
  Value* emptyV = new Value;

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

namespace onnc
{
  INITIALIZE_PASS(NvDlaLayerFusionPass, "NvDlaLayerFusionPass")
}
