//===- UpdateGraphOutputSize.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/ONNXWrapper/ONNXWrapper.h>
#include <onnc/Option/CommandLine.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Options
//===----------------------------------------------------------------------===//
static unsigned int getBatchSize()
{
  static cl::opt<unsigned int> batch_size(
      "batch-size", cl::kShort, cl::kOptional, cl::kValueRequired, cl::init(0),
      cl::desc("specific input batch size"));
  return batch_size;
}

//===----------------------------------------------------------------------===//
// UpdateGraphOutputSize
//===----------------------------------------------------------------------===//
UpdateGraphOutputSize::UpdateGraphOutputSize()
  : ModulePass(ID) {
  m_BatchSize = getBatchSize();
}

void UpdateGraphOutputSize::updateInputBatchSize(xGraph *pGraph)
{
  // update input batch size
  std::unordered_set<std::string> initializer_names(
      pGraph->initializer_names().begin(), pGraph->initializer_names().end());
  for (int i = 0; i < pGraph->inputs().size(); ++i) {
    xValue *v = pGraph->inputs()[i];
    // ignore weight
    if (0 != initializer_names.count(v->uniqueName()))
      continue;
    // update valueInfo
    auto sizes = v->sizes();
    sizes[0] = m_BatchSize;
    v->setSizes(sizes);
  }
}

void UpdateGraphOutputSize::updateOutputValueInfo(xGraph *pGraph)
{
  auto outputs = pGraph->outputs();
  std::unordered_set<xValue *> output_set(outputs.begin(), outputs.end());
  // reset output valueInfo
  for (xNode *n : pGraph->nodes()) {
    for (xValue *out : n->outputs()) {
      // do not reset graph's outputs valueInfo
      if (output_set.count(out))
        continue;
      // reset dimension and elemType
      std::vector<xDimension> sizes;
      out->setSizes(sizes);
      out->setElemType((xTensorProtoDataType)onnc::Value::kUndefined);
    }
  }

  // update graph's outputs valueInfo
  for (xValue *out : outputs) {
    auto sizes = out->sizes();
    sizes[0] = m_BatchSize;
    out->setSizes(sizes);
  }
}

Pass::ReturnType UpdateGraphOutputSize::runOnModule(Module &pModule)
{
  xGraph *graph = pModule.getRootTensorGraph();

  // update input batch size and reset old output valueInfo
  if (m_BatchSize > 0) {
    updateInputBatchSize(graph);
    updateOutputValueInfo(graph);
  }

  onnxInferShape(pModule);

  return Pass::kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char UpdateGraphOutputSize::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(UpdateGraphOutputSize, "UpdateGraphOutputSize")
  void InitializeUpdateGraphOutputSizePassOptions();
} // namespace onnc

void onnc::InitializeUpdateGraphOutputSizePassOptions()
{
  // initialisze all options of UpdateGraphOutputSize pass.
  getBatchSize();
}

UpdateGraphOutputSize *onnc::CreateUpdateGraphOutputSizePass()
{
  return new UpdateGraphOutputSize();
}
