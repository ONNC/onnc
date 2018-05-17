//===- MemoryUsageAnalysis.cpp --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/MemoryUsageAnalysis.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// MemoryUsageAnalysis
//===----------------------------------------------------------------------===//
MemoryUsageAnalysis::MemoryUsageAnalysis()
  : ModulePass(ID), m_MemUsageMap() {
}

Pass::ReturnType MemoryUsageAnalysis::runOnModule(Module& pModule)
{
  clear();
  calculateMemoryUsages(*pModule.getGraph());
  return kModuleNoChanged;
}

void MemoryUsageAnalysis::print(std::ostream& pOS) const
{
  // TODO
}

static size_t GetTensorMemSize(const TensorSize &pTS, TP_DataTy pTy)
{
  outs() << "- size = " << pTS.size() << " (";
  for (const onnx::Dimension &dim : pTS)
    outs() << dim.param << ":" << dim.dim << " ";
  outs() << ")" << "\n";
  return 0;
}

void MemoryUsageAnalysis::calculateMemoryUsages(onnx::Graph &pGraph)
{
  for (onnx::Node *n : pGraph.nodes()) {
    for (onnx::Value *v : n->inputs()) {
      outs() << v->uniqueName() << "\n";
      GetTensorMemSize(v->sizes(), v->elemType());
    }

    for (onnx::Value *v : n->outputs()) {
      outs() << v->uniqueName() << "\n";
      GetTensorMemSize(v->sizes(), v->elemType());
    }
  } // for each node
}

void MemoryUsageAnalysis::clear()
{
  m_MemUsageMap.clear();  
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char MemoryUsageAnalysis::ID = 0;

MemoryUsageAnalysis *onnc::CreateMemoryUsageAnalysisPass()
{
  return new MemoryUsageAnalysis();
}
