//===- FuseInplaceValue.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/CodeGen/FuseInplaceValue.h>
#include <onnc/Core/PassAnalysisSupport.h>
#include <onnc/Core/PassSupport.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// FuseInplaceValue
//===----------------------------------------------------------------------===//
Pass::ReturnType FuseInplaceValue::runOnComputeGraph(ComputeGraph& pCG)
{
  Pass::ReturnType ret = Pass::kModuleNoChanged;
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    ComputeOperator* node = nodeIt;
    if (!m_IsFusibleFn(*node))
      continue;

    // TODO: Get fusible input/output pair (backend should provide this)
    //       Currently assume operator's 1st input and 1st output is a fusible
    //       pair.
    const unsigned outputId = 0;
    Value *input = node->getInput(0),
          *output = node->getOutput(outputId);

    // If input has more than one user, we can't fuse this pair, since we need
    // keep input value for other users.
    if (input->getUses().size() > 1)
      continue;

    Define* origDef = input->getDefine();
    unsigned origDefNo = input->getDefineNo();
    input->clearDefine();
    node->replaceOutput(outputId, *input);
    input->clearDefine();
    input->setDefine(origDef, origDefNo);

    pCG.erase(*output);

    ret |= Pass::kModuleChanged;
  }
  return ret;
}

namespace onnc
{
  INITIALIZE_PASS(FuseInplaceValue, "FuseInplaceValue")
}
