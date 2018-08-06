//===---------------------------------------------------------------------===//
//
//                             The ONNC Project
//
// Copyright(c) 2018, The ONNC Team
//
// This file is part of the ONNC Project and is distributed under
// 3-clause BSD license (https://opensource.org/licenses/BSD-3-Clause)
//
// See LICENSE.TXT for details.
//
//===---------------------------------------------------------------------===//
#define DEBUG_TYPE "bm188x_code_emitter"
#include "BM188xCodeEmitter.h"
#include "TGConv.h"
#include "TLConv.h"
#include <cstdint>
#include <fstream>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/Debug.h>
#include <onnc/Target/Sophon/BM188x/bmkernel_api.h>
#include <onnc/Target/Sophon/io.hpp>

#include <onnc/JSON/Object.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/Value.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

BM188xCodeEmitter::BM188xCodeEmitter(BM1880Backend *pBackend,
                                     BM1880Backend::Instructions& pInsns)
    : TGCodeEmitter(), m_Backend(pBackend), m_Instructions(pInsns)
{
}

float BM188xCodeEmitter::getThreshold(const std::string &pOnncLayerName)
{

  const tg::bm1880::LayerCalibrationParameter &ctable =
      *m_Backend->getLayerCtable(pOnncLayerName);
  float threshold = 0.0;
  for (int j = 0; j < ctable.blob_param_size(); j++) {
    if (ctable.blob_param(j).name() == pOnncLayerName) {
      threshold = ctable.blob_param(j).threshold_y();
      break;
    }
  }
  return threshold;
}

std::string BM188xCodeEmitter::findOnncLayerName(const xGraph *pOnnxGraph,
                                                 const xValue *pValue)
{
  for (auto inst = m_Instructions.rbegin();
       inst != m_Instructions.rend();
       ++inst) {
    if (inst->get()->getLayerName() == pValue->uniqueName()) {
      return pValue->uniqueName();
    }
  }

  const xValue *input = pValue->node()->inputs()[0];
  auto graph_inputs = pOnnxGraph->inputs();
  if (std::find(graph_inputs.begin(), graph_inputs.end(), input) !=
      graph_inputs.end()) {
    // can not find the onnc layer
    assert(0);
  }
  return findOnncLayerName(pOnnxGraph, input);
}

onnc::json::Object
BM188xCodeEmitter::genOutputLayer(const std::string &pDefaultOnncLayerName,
                                  const std::string &pDefaultOnnxLayerName,
                                  const xGraph *pOnnxGraph)
{
  size_t step = 0;
  onnc::json::Object jExeSteps;

  // generate default output layer
  onnc::json::Object layer_info;
  layer_info.insert("onnx output", pDefaultOnnxLayerName);
  layer_info.insert("onnc output", pDefaultOnncLayerName);
  layer_info.insert("threshold", getThreshold(pDefaultOnncLayerName));
  jExeSteps.insert(std::to_string(step++), layer_info);

  // generate the other output layer info
  while (step < pOnnxGraph->outputs().size()) {
    const xValue *onnx_layer = pOnnxGraph->outputs()[step];
    std::string onnc_layer_name = findOnncLayerName(pOnnxGraph, onnx_layer);
    std::string onnx_layer_name = onnx_layer->uniqueName();
    onnc::json::Object layer_info;
    layer_info.insert("onnx output", onnx_layer_name);
    layer_info.insert("onnc output", onnc_layer_name);
    layer_info.insert("threshold", getThreshold(onnc_layer_name));
    jExeSteps.insert(std::to_string(step++), layer_info);
  }
  return jExeSteps;
}

static onnc::json::Object genFallbackPlan(const std::string &pONNCLast,
                                          const xGraph *pOnnxGraph)
{
  bool is_find_fallback = false;
  int step = 0;

  onnc::json::Object jExeSteps;
  for (auto n : pOnnxGraph->nodes()) {
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

    if (n->outputs()[0]->uniqueName() == pONNCLast) {
      is_find_fallback = true;
    }
  }

  std::vector<xDimension> onncOutDim;
  for (auto n : pOnnxGraph->nodes()) {
    for (size_t i = 0; i < n->outputs().size(); ++i) {
      if (n->outputs()[i]->uniqueName() == pONNCLast) {
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
    jInputValue.insert("name", pONNCLast);

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
    jInputValue.insert("name", pONNCLast);

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
    jInputValue.insert("name", pONNCLast);

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

  return jExeSteps;
}

void BM188xCodeEmitter::genWeightBin(const std::string &pOutputFilename)
{
  BM188X::Weight weight;
  weight.genWeightBin(pOutputFilename, m_Instructions,
                      m_Backend->getMemOperands());
}

void BM188xCodeEmitter::encodeInstructions(std::ostream &pOS)
{
  if (m_Instructions.empty())
    return;

  ::bmnet::bmnet_asm::asm_context::get_context().set_fp(pOS);
  for (auto const &i : m_Instructions) {
    ::bmnet::bmnet_asm::asm_context::get_context().name = i->getLayerName();
    i->emit();
  }
}

void BM188xCodeEmitter::genRuntimeInfo(const xGraph *pOnnxGraph,
                                       std::ostream &pOS)
{
  if (m_Instructions.empty())
    return;

  onnc::json::Object jRoot;
  onnc::json::Object jMemLayout;
  onnc::json::Object jInputThres;
  onnc::json::Object jInputDim;
  onnc::json::Object jOutputLayer;
  onnc::json::Object jBatch;
  onnc::json::Object jFallback;
  // Generate memory layout.
  for (auto const &inst : m_Instructions) {
    onnc::json::Object jLayer;
    for (auto &mem : inst->getMemOperands()) {
      onnc::json::Object jMem;
      jMem.insert("addr", onnc::json::Value(mem->m_Addr));
      jMem.insert("size", onnc::json::Value(mem->m_Size));
      jLayer.insert(mem->m_Name, jMem);
    }
    DEBUG(dbgs() << "inst name == " << inst->getLayerName() << "\n");
    jMemLayout.insert(inst->getLayerName(), jLayer);
  }

  // Find the input of network.
  // The input of network should be in input list but not in initializers.
  const xValue *input;
  auto *pGraph = const_cast<xGraph *>(pOnnxGraph);
  for (const xValue *in : pOnnxGraph->inputs()) {
    const auto &initNames = pGraph->initializer_names();
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
      *m_Backend->getLayerCtable(dataLayerName);
  float threshold = dataCtable.blob_param(0).threshold_y();
  DEBUG(dbgs() << "data layer name = " << dataLayerName
               << ", threshold = " << threshold << "\n");
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

  // Generate output layer info
  const xValue* onnx_layer = pOnnxGraph->outputs()[0];
  std::string defaultOnnxOutLayerName = onnx_layer->uniqueName();
  std::string defaultOnncOutLayerName =
      findOnncLayerName(pOnnxGraph, onnx_layer);
  jOutputLayer = genOutputLayer(defaultOnncOutLayerName,
                                defaultOnnxOutLayerName, pOnnxGraph);
  jRoot.insert("output layer", jOutputLayer);

  // Generate fallback plan.
  jFallback = genFallbackPlan(defaultOnncOutLayerName, pOnnxGraph);
  jRoot.insert("cpu fallback", jFallback);

  // Insert all information to root.
  jRoot.insert("memory layout", jMemLayout);
  jRoot.insert("data layer threshold", jInputThres);
  jRoot.insert("batch", jBatch);
  jRoot.insert("data layer dim", jInputDim);

  // std::ofstream outfile(pOutputFilename, std::ofstream::binary);
  onnc::IndentOStream oss(pOS);
  jRoot.print(oss);

  return;
}
