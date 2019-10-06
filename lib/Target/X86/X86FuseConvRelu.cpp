//===- X86FuseConvRelu.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/PassSupport.h>
#include <onnc/IR/Compute/Conv.h>
#include <onnc/IR/Compute/Relu.h>
#include "Compute/X86ConvRelu.h"
#include "X86FuseConvRelu.h"

using namespace onnc;

//===----------------------------------------------------------------------===//
// X86FuseConvRelu
//===----------------------------------------------------------------------===//
Pass::ReturnType X86FuseConvRelu::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator* node = nodeIt;

    if (!isFusible(*node))
      continue;

    // Create X86ConvRelu to fuse Conv and Relu.
    Conv& conv = *(Conv *)node;
    Relu& relu = *(Relu *)conv.getOutput(0)->getUses()[0].getUser();

    mergeConvRelu(pCG, conv, relu);

    pCG.erase(conv);
    pCG.erase(relu);

    ret |= Pass::kModuleChanged;
  }

  pCG.topologicalSort();

  return ret;
}

bool X86FuseConvRelu::isFusible(ComputeOperator& pNode)
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

X86ConvRelu* X86FuseConvRelu::mergeConvRelu(ComputeGraph& pCG,
                                            Conv& pConv, Relu& pRelu)
{
  Value* outv = pRelu.getOutput(0);
  Value* out_conv = pConv.getOutput(0);
  pConv.replaceOutput(0, *outv);
  pCG.erase(*out_conv);
  // FIXME: need move newOp to correct position.
  X86ConvRelu* newOp = pCG.addOperator<X86ConvRelu>(pConv, pRelu);
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

namespace onnc
{
  INITIALIZE_PASS(X86FuseConvRelu, "X86FuseConvRelu")
}
