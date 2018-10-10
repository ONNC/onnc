//===- CodeEmit.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/CodeEmit.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// CodeEmit
//===----------------------------------------------------------------------===//
Pass::ReturnType CodeEmit::runOnModule(Module& pModule)
{
  unsigned result = Pass::kModuleNoChanged;
  Module::cg_iterator cg, cgEnd = pModule.cgEnd();
  for (cg = pModule.cgBegin(); cg != cgEnd; ++cg)
    result |= runOnComputeGraph(*cg->value());
  return result;
}

Pass::ReturnType CodeEmit::runOnComputeGraph(ComputeGraph& pCG)
{
  ComputeGraph::iterator nodeIt, nEnd = pCG.end();
  for (nodeIt = pCG.begin(); nodeIt != nEnd; ++nodeIt) {
    const onnc::ComputeOperator *node = nodeIt;
    ///beforeEmit(node);
    node->accept(m_CodeEmitVisitor);
  }
  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// CodeEmit factory method
//===----------------------------------------------------------------------===//
char CodeEmit::ID = 0;

#if 0
//FIXME a pass class with input parameter cannot be initialized?
namespace onnc
{
  INITIALIZE_PASS(CodeEmit, "CodeEmit")
}
#endif

CodeEmit* onnc::CreateCodeEmitPass(ComputeVisitor& pCodeEmitVisitor)
{
  return new CodeEmit(pCodeEmitVisitor);
}
