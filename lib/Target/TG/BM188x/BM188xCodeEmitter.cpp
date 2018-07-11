#define DEBUG_TYPE "bm188x_code_emitter"
#include "BM188xCodeEmitter.h"
#include "TGConv.h"
#include <cstdint>
#include <fstream>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/Debug.h>
#include <onnc/Target/TG/BM188x/bmkernel_api.h>
#include <onnc/Target/TG/io.hpp>

#include <onnc/JSON/Object.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/Value.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

static onnc::json::Object genFallbackPlan(std::string pONNCLast,
                                          std::string pONNXLast,
                                          const ::onnx::Graph *pOnnxGraph)
{
  bool is_find_fallback = false;
  int step = 0;

  onnc::json::Object jExeSteps;
  for (auto n : pOnnxGraph->nodes()) {
    // Find the left layers info for fallback.
    if (is_find_fallback) {
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

  return jExeSteps;
}

BM188xCodeEmitter::BM188xCodeEmitter(BM1880Backend *pBackend)
    : TGCodeEmitter(pBackend), m_Backend(pBackend)
{
}

//#define DEBUG_WEIGHT_BIN

void BM188xCodeEmitter::prepare8bitWeight(const MemOperand *pMemOp,
                                          std::vector<int8_t> &pWeight)
{
  const onnx::Tensor &tensor = onnc::getTensor(pMemOp->m_Value->uniqueName(),
                                               *pMemOp->m_Value->owningGraph());

  const std::string &raw = tensor.raw();
  std::copy(raw.begin(), raw.end(), std::back_inserter(pWeight));
#ifdef DEBUG_WEIGHT_BIN
  std::vector<int8_t> data;
  std::copy(raw.begin(), raw.end(), std::back_inserter(data));
  std::cout << pMemOp->m_Value->uniqueName() << "\n";
  for (auto i : data) {
    std::cout << (int32_t)i << ",";
  }
  std::cout << "\n";
#endif
}

void BM188xCodeEmitter::prepare16bitWeight(const MemOperand *pMemOp,
                                           std::vector<int8_t> &pWeight)
{
  assert(pMemOp->m_Type == onnx::TensorProto_DataType_INT16);
  const onnx::Tensor &tensor = onnc::getTensor(pMemOp->m_Value->uniqueName(),
                                               *pMemOp->m_Value->owningGraph());

  const std::string &raw = tensor.raw();
  size_t count = onnc::getTotalCount(tensor.sizes());
  std::vector<int16_t> int16_vector(count);
  memcpy(int16_vector.data(), raw.data(), count * sizeof(int16_t));
#ifdef DEBUG_WEIGHT_BIN
  std::cout << pMemOp->m_Value->uniqueName() << "\n";
  for (auto i : int16_vector) {
    std::cout << i << ",";
  }
  std::cout << "\n";
#endif
  size_t offset = pWeight.size();
  pWeight.resize(offset + count * 2);
  for (size_t i = 0; i < count; ++i) {
    pWeight[offset + i] = (int8_t)(int16_vector[i] & 0xff);
    pWeight[offset + i + count] = (int8_t)((int16_vector[i] >> 8) & 0xff);
  }
}

void BM188xCodeEmitter::prepareWeight(std::vector<int8_t> &pWeight)
{
  size_t weight_size = 0;
  size_t weight_count = 0;
  for (auto *mem_op : m_Backend->getMemOperands()) {
    if (mem_op->m_MemType == MemType::NEURON)
      continue;
    weight_size += mem_op->m_Size;
    ++weight_count;
  }

  pWeight.reserve(weight_size);

  for (auto &inst : m_Backend->getInsts()) {

    // handle special case
    if (inst->getTypeName() == "Conv") {
      std::vector<int8_t> weight;
      auto *tgconv = dynamic_cast< ::onnc::BM188X::TGConv *>(inst.get());
      tgconv->prepareWeight(weight);
      pWeight.insert(pWeight.end(), weight.begin(), weight.end());
      continue;
    }

    for (auto *mem_op : inst->getMemOperands()) {
      if (mem_op->m_MemType == MemType::NEURON)
        continue;

      if (mem_op->m_Type == ::onnx::TensorProto_DataType_INT8) {
        prepare8bitWeight(mem_op, pWeight);
      } else {
        prepare16bitWeight(mem_op, pWeight);
      }
    }
  }
#ifdef DEBUG_WEIGHT_BIN
  int n = 0;
  std::cout << "out_weight:\n";
  for (auto i : pWeight) {
    std::cout << (int)i << ",";
    if (++n % 10 == 0)
      std::cout << "\n";
  }
  std::cout << "\n";
#endif
}

void BM188xCodeEmitter::genWeightBin(const Path &pOutputPath)
{
  prepareWeight(m_WeightData);
  if (!m_WeightData.empty())
    bmnet::WriteInt8DataToBinaryFile(&m_WeightData,
                                     pOutputPath.native().c_str());
}

void BM188xCodeEmitter::encodeInstructions(const Path &pOutputPath)
{
  auto &instList = m_Backend->getInsts();
  if (instList.empty())
    return;

  Path output_path("cmdbuf.bin");
  if (!pOutputPath.empty())
    output_path = pOutputPath;

  std::fstream fp;
  if (m_Backend->getOption().m_DumpASM) {
    if (m_Backend->getOption().m_ASMOutput.empty())
      ::bmnet::bmnet_asm::asm_context::get_context().set_fp(onnc::outs());
    else {
      fp.open(m_Backend->getOption().m_ASMOutput, std::ios::out);
      ::bmnet::bmnet_asm::asm_context::get_context().set_fp(fp);
    }
  }
  for (auto const &i : instList) {
    ::bmnet::bmnet_asm::asm_context::get_context().name = i->getLayerName();
    i->emit();
  }
  instList.clear();
}

void BM188xCodeEmitter::genRuntimeInfo(const ::onnx::Graph *pOnnxGraph)
{
  onnc::json::Object jRoot;
  onnc::json::Object jMemLayout;
  onnc::json::Object jInputThres;
  onnc::json::Object jInputDim;
  onnc::json::Object jOutputThres;
  onnc::json::Object jOutputLayer;
  onnc::json::Object jBatch;
  onnc::json::Object jFallback;
  // Generate memory layout.
  auto &instList = m_Backend->getInsts();
  if (instList.empty())
    return;
  for (auto const &inst : instList) {
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
  const onnx::Value *input;
  auto *pGraph = const_cast< ::onnx::Graph *>(pOnnxGraph);
  for (const onnx::Value *in : pOnnxGraph->inputs()) {
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
  auto sizes = pOnnxGraph->inputs()[0]->sizes();
  onnc::json::Array jDimArr;
  for (size_t i = 0; i < sizes.size(); ++i) {
    jDimArr.push_back(onnc::json::Value(sizes[i].dim));
  }
  jInputDim.insert("dim", jDimArr);

  // Generate batch size of the input.
  auto batchSize = sizes[0].dim;
  jBatch.insert("size", batchSize);

  // Generate output layer name.
  std::string onnxOutputLayerName = pOnnxGraph->outputs()[0]->uniqueName();
  jOutputLayer.insert("onnx output", onnxOutputLayerName);
  std::string onncOutputLayerName =
      instList[instList.size() - 1]->getLayerName();
  jOutputLayer.insert("onnc output", onncOutputLayerName);

  // Generate fallback plan if need.
  if (onnxOutputLayerName != onncOutputLayerName) {
    jFallback =
        genFallbackPlan(onncOutputLayerName, onnxOutputLayerName, pOnnxGraph);
    jRoot.insert("cpu fallback", jFallback);
  }

  // Generate the threshold of onnc out layer for de-quantization.
  const tg::bm1880::LayerCalibrationParameter &outCtable =
      *m_Backend->getLayerCtable(onncOutputLayerName);
  for (int i = 0; i < dataCtable.blob_param_size(); i++) {
    if (outCtable.blob_param(i).name() == onncOutputLayerName) {
      threshold = outCtable.blob_param(i).threshold_y();
      jOutputThres.insert("threshold", threshold);
      break;
    }
  }

  // Insert all information to root.
  jRoot.insert("memory layout", jMemLayout);
  jRoot.insert("data layer threshold", jInputThres);
  jRoot.insert("output layer", jOutputLayer);
  jRoot.insert("batch", jBatch);
  jRoot.insert("onnc out layer threshold", jOutputThres);
  jRoot.insert("data layer dim", jInputDim);

  std::string sOutPath = pOnnxGraph->has_name()
                             ? (pOnnxGraph->name() + "_runtime.json")
                             : "runtime.json";
  std::ofstream outfile(sOutPath, std::ofstream::binary);
  onnc::IndentOStream oss(outfile);
  jRoot.print(oss);
  outfile.close();

  return;
}
