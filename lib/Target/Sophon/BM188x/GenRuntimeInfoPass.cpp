//===- GenRuntimeInfoPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GenRuntimeInfoPass.h"
#include <onnc/JSON/Object.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/Value.h>
#include <onnc/Support/IndentOStream.h>
#include <onnc/Support/OFStream.h>
#include <onnc/Config/ONNX.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::GenRuntimeInfoPass::ID = 0;

//===----------------------------------------------------------------------===//
// static functions
//===----------------------------------------------------------------------===//
std::string
BM188X::GenRuntimeInfoPass::FindOnncLayerName(const onnx::Graph& pG,
                                               const ::onnx::Value &pValue)
{
  ::onnx::const_graph_node_list_iterator node, nEnd = pG.end();
  for (node = pG.begin(); node != nEnd; ++node) {
    std::string layer_name =
        const_cast< ::onnx::Node*>(*node)->output()->uniqueName();

    if (layer_name == pValue.uniqueName()) {
      return pValue.uniqueName();
    }
  }

  const onnx::Value *input = pValue.node()->inputs()[0];
  auto inputs = pG.inputs();
  if (inputs.end() != std::find(inputs.begin(), inputs.end(), input)) {
    // can not find the onnc layer
    assert(0);
    return std::string();
  }
  return FindOnncLayerName(pG, *input);
}

void
BM188X::GenRuntimeInfoPass::GetDefaultLayerNames(LayerNames& pNames,
                                                 const ::onnx::Graph& pG)
{
  const onnx::Value* value = pG.outputs()[0];
  pNames.onnx = value->uniqueName();
  pNames.onnc = FindOnncLayerName(pG, *value);
}

//===----------------------------------------------------------------------===//
// GenRuntimeInfoPass
//===----------------------------------------------------------------------===//
BM188X::GenRuntimeInfoPass::GenRuntimeInfoPass(BM1880Backend* pBackend,
                                               const Path &pOutFile)
    : ModulePass(ID), m_pBackend(pBackend), m_OutFile(pOutFile)
{
}

Pass::ReturnType BM188X::GenRuntimeInfoPass::runOnModule(Module &pModule)
{
  json::Object document;

  LayerNames names;
  GetDefaultLayerNames(names, *pModule.getRootTensorGraph());
  GenOutputLayer(document, names, *pModule.getRootTensorGraph());
  GenFallbackPlan(document, names, *pModule.getRootTensorGraph());
  GenMemoryLayout(document, *pModule.getRootComputeGraph());
  GenRest(document, *pModule.getRootTensorGraph());

  OFStream os(m_OutFile, std::ios::out | std::ios::binary);
  IndentOStream oss(os);
  document.print(oss);
  return kModuleNoChanged;
}

void BM188X::GenRuntimeInfoPass::GenOutputLayer(json::Object& pOutput,
                                                const LayerNames& pNames,
                                                const ::onnx::Graph& pG)
{
  size_t step = 0;
  onnc::json::Object jExeSteps;

  // generate default output layer
  onnc::json::Object layer_info;
  layer_info.insert("onnx output", pNames.onnx);
  layer_info.insert("onnc output", pNames.onnc);
  layer_info.insert("threshold", getThreshold(pNames.onnc));
  jExeSteps.insert(std::to_string(step++), layer_info);

  // generate the other output layer info
  while (step < pG.outputs().size()) {
    const onnx::Value *onnx_layer = pG.outputs()[step];
    std::string onnc_layer_name = FindOnncLayerName(pG, *onnx_layer);
    std::string onnx_layer_name = onnx_layer->uniqueName();
    onnc::json::Object layer_info;
    layer_info.insert("onnx output", onnx_layer_name);
    layer_info.insert("onnc output", onnc_layer_name);
    layer_info.insert("threshold", getThreshold(onnc_layer_name));
    jExeSteps.insert(std::to_string(step++), layer_info);
  }

  pOutput.insert("output layer", jExeSteps);
}

void GenRuntimeInfoPass::GenFallbackPlan(json::Object& pOutput,
                                         const LayerNames& pNames,
                                         const ::onnx::Graph& pG)
{
  bool is_find_fallback = false;
  int step = 0;

  onnc::json::Object jExeSteps;
  for (auto n : pG.nodes()) {
    // Find the left layers info for fallback.
    // The Flatten/Reshape don't generate asm,
    // so we dont add them to the fallback plan
    // if they are at end of a model
    if (is_find_fallback && strcmp(n->kind().toString(), "Flatten") != 0 &&
        strcmp(n->kind().toString(), "Reshape") != 0) {
      onnc::json::Object jLayerInfo;
      jLayerInfo.insert("type", n->kind().toString());

      onnc::json::Object jInputs;
      for (size_t i = 0; i < n->inputs().size(); ++i) {
        onnc::json::Object jValue;
        jValue.insert("name", n->inputs()[i]->uniqueName());

        onnc::json::Array jDim;
        auto Dims = n->inputs()[i]->sizes();
        for (size_t j = 0; j < Dims.size(); j++) {
          jDim.push_back(onnc::json::Value(Dims[j].dim));
        }
        jValue.insert("dim", jDim);

        jLayerInfo.insert("input" + std::to_string(i), jValue);
        // TODO: gen Attributes
      }

      for (size_t i = 0; i < n->outputs().size(); ++i) {
        onnc::json::Object jValue;
        jValue.insert("name", n->outputs()[i]->uniqueName());

        jLayerInfo.insert("output" + std::to_string(i), jValue);
      }
      jExeSteps.insert(std::to_string(step), jLayerInfo);
      step++;
    }

    if (n->outputs()[0]->uniqueName() == pNames.onnc) {
      is_find_fallback = true;
    }
  }

  std::vector<onnx::Dimension> onncOutDim;
  for (auto n : pG.nodes()) {
    for (size_t i = 0; i < n->outputs().size(); ++i) {
      if (n->outputs()[i]->uniqueName() == pNames.onnc) {
        onncOutDim = n->outputs()[i]->sizes();
        break;
      }
    }
  }
  assert(onncOutDim.size() != 0);

  // Generate acc top_1.
  {
    onnc::json::Object jLayerAccTop1;
    jLayerAccTop1.insert("type", "Accuracy");
    onnc::json::Object jInputValue;
    jInputValue.insert("name", pNames.onnc);

    onnc::json::Array jDim;
    for (size_t j = 0; j < onncOutDim.size(); j++) {
      jDim.push_back(onnc::json::Value(onncOutDim[j].dim));
    }
    jInputValue.insert("dim", jDim);
    jLayerAccTop1.insert("input0", jInputValue);

    onnc::json::Object jOutputValue;
    jOutputValue.insert("name", "acc_top1");
    jLayerAccTop1.insert("output0", jOutputValue);

    onnc::json::Object jTopk;
    jTopk.insert("top_k", 1);

    jLayerAccTop1.insert("args", jTopk);
    jExeSteps.insert(std::to_string(step), jLayerAccTop1);
    step++;
  }

  // Generate acc top_5.
  {
    onnc::json::Object jLayerAccTop5;
    jLayerAccTop5.insert("type", "Accuracy");
    onnc::json::Object jInputValue;
    jInputValue.insert("name", pNames.onnc);

    onnc::json::Array jDim;
    for (size_t j = 0; j < onncOutDim.size(); j++) {
      jDim.push_back(onnc::json::Value(onncOutDim[j].dim));
    }
    jInputValue.insert("dim", jDim);
    jLayerAccTop5.insert("input0", jInputValue);

    onnc::json::Object jOutputValue;
    jOutputValue.insert("name", "acc_top5");
    jLayerAccTop5.insert("output0", jOutputValue);

    onnc::json::Object jTopk;
    jTopk.insert("top_k", 5);
    jLayerAccTop5.insert("args", jTopk);

    jExeSteps.insert(std::to_string(step), jLayerAccTop5);
    step++;
  }

  // Generate loss.
  {
    onnc::json::Object jLayerLoss;
    jLayerLoss.insert("type", "SoftmaxWithLoss");
    onnc::json::Object jInputValue;
    jInputValue.insert("name", pNames.onnc);

    onnc::json::Array jDim;
    for (size_t j = 0; j < onncOutDim.size(); j++) {
      jDim.push_back(onnc::json::Value(onncOutDim[j].dim));
    }
    jInputValue.insert("dim", jDim);
    jLayerLoss.insert("input0", jInputValue);

    onnc::json::Object jOutput0Value;
    jOutput0Value.insert("name", "prob");
    onnc::json::Object jOutput1Value;
    jOutput1Value.insert("name", "loss");
    jLayerLoss.insert("output0", jOutput0Value);
    jLayerLoss.insert("output1", jOutput1Value);

    jExeSteps.insert(std::to_string(step), jLayerLoss);
    step++;
  }

  pOutput.insert("cpu fallback", jExeSteps);
}

void GenRuntimeInfoPass::GenMemoryLayout(json::Object& pOutput,
                                         const ComputeGraph& pG)
{
  onnc::json::Object jMemLayout;
  ComputeGraph::const_iterator inst, iEnd = pG.end();
  for (inst = pG.begin(); inst != iEnd; ++inst) {
    onnc::json::Object jLayer;
    // inputs of inst
    unsigned int ins = inst->getNumOfInputs();
    for (unsigned int i = 0; i < ins; ++i) {
      onnc::json::Object jMem;
      const ComputeMemOperand *opnd =
          backend()->getMemOpndByValue(inst->getInput(i));
      jMem.insert("addr", onnc::json::Value(opnd->start()));
      jMem.insert("size", onnc::json::Value(opnd->length()));
      jLayer.insert(inst->getInput(i)->getName(), jMem);
    }

    // outputs of inst
    unsigned int outs = inst->getNumOfOutputs();
    for (unsigned int i = 0; i < outs; ++i) {
      onnc::json::Object jMem;
      const ComputeMemOperand *opnd =
          backend()->getMemOpndByValue(inst->getOutput(i));
      jMem.insert("addr", onnc::json::Value(opnd->start()));
      jMem.insert("size", onnc::json::Value(opnd->length()));
      jLayer.insert(inst->getOutput(i)->getName(), jMem);
    }

    jMemLayout.insert(inst->getOutput(0)->getName(), jLayer);
  }

  pOutput.insert("memory layout", jMemLayout);
}

void GenRuntimeInfoPass::GenRest(json::Object& pOutput, const ::onnx::Graph& pG)
{
  onnc::json::Object jInputThres;
  onnc::json::Object jInputDim;
  onnc::json::Object jOutputLayer;
  onnc::json::Object jBatch;
  onnc::json::Object jFallback;
  // Find the input of network.
  // The input of network should be in input list but not in initializers.
  const onnx::Value *input = nullptr;
  auto *graph= const_cast< ::onnx::Graph *>(&pG);
  for (const onnx::Value *in : pG.inputs()) {
    const auto &initNames = graph->initializer_names();
    auto found =
        std::find(initNames.begin(), initNames.end(), in->uniqueName());
    if (found == initNames.end()) {
      input = in;
      break;
    }
  }

  // Generate the threshold of data_layer for quantization.
  std::string dataLayerName = input->uniqueName();
  const tg::bm1880::LayerCalibrationParameter &dataCtable =
      *backend()->getLayerCtable(dataLayerName);
  float threshold = dataCtable.blob_param(0).threshold_y();
  jInputThres.insert("threshold", threshold);

  // Generate data_layer dimension.
  auto sizes = input->sizes();
  onnc::json::Array jDimArr;
  for (size_t i = 0; i < sizes.size(); ++i) {
    jDimArr.push_back(onnc::json::Value(sizes[i].dim));
  }
  jInputDim.insert("dim", jDimArr);

  // Generate batch size of the input.
  auto batchSize = sizes[0].dim;
  jBatch.insert("size", batchSize);

  pOutput.insert("data layer threshold", jInputThres);
  pOutput.insert("batch", jBatch);
  pOutput.insert("data layer dim", jInputDim);
}

float GenRuntimeInfoPass::getThreshold(const std::string &pName)
{
  const tg::bm1880::LayerCalibrationParameter &ctable =
      *backend()->getLayerCtable(pName);
  float threshold = 0.0;
  for (int j = 0; j < ctable.blob_param_size(); j++) {
    if (ctable.blob_param(j).name() == pName) {
      threshold = ctable.blob_param(j).threshold_y();
      break;
    }
  }
  return threshold;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
ModulePass*
BM188X::CreateGenRuntimeInfoPass(BM1880Backend* pBackend, const Path& pOutFile)
{
  return new GenRuntimeInfoPass(pBackend, pOutFile);
}
