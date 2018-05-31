#define DEBUG_TYPE "bm188x_code_emitter"
#include "BM188xCodeEmitter.h"
#include "TGConv.h"
#include "utils/io.hpp"
#include <bm_kernel.h>
#include <cstdint>
#include <fstream>
#include <onnc/IR/ONNXUtils.h>
#include <onnc/Support/Debug.h>

#include <onnc/JSON/Object.h>
#include <onnc/JSON/Reader.h>
#include <onnc/JSON/Value.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

BM188xCodeEmitter::BM188xCodeEmitter(BM1880Backend *pBackend)
    : TGCodeEmitter(pBackend), m_Backend(pBackend)
{
}

//#define DEBUG_WEIGHT_BIN
void BM188xCodeEmitter::prepareWeight(std::vector<int8_t> &pWeight)
{
  // TODO use elegant method to get onnx::Graph
  const ::onnx::Graph *graph =
      m_Backend->getMemOperands()[0]->m_Value->owningGraph();
  // check weight tensor is exist or not
  if (const_cast< ::onnx::Graph *>(graph)->initializers().empty())
    return;

  size_t weight_size = 0;
  for (auto *mem_op : m_Backend->getMemOperands()) {
    if (mem_op->m_MemType == MemType::NEURON)
      continue;
    weight_size += mem_op->m_Size;
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

      const ::onnx::Tensor &tensor = ::onnc::getTensor(
          mem_op->m_Value->uniqueName(), *mem_op->m_Value->owningGraph());
      if (mem_op->m_Type == ::onnx::TensorProto_DataType_INT8) {
        const std::string &raw = tensor.raw();
        std::copy(raw.begin(), raw.end(), std::back_inserter(pWeight));
#ifdef DEBUG_WEIGHT_BIN
        std::vector<int8_t> data;
        std::copy(raw.begin(), raw.end(), std::back_inserter(data));
        std::cout << mem_op->m_Value->uniqueName() << "\n";
        for (auto i : data) {
          std::cout << (int32_t)i << ",";
        }
        std::cout << "\n";
#endif
      } else {
        assert(mem_op->m_Type == ::onnx::TensorProto_DataType_INT16);
        const std::string &raw = tensor.raw();
        size_t count = ::onnc::getTotalCount(tensor.sizes());
        std::vector<int16_t> int16_vector(count);
        memcpy(int16_vector.data(), raw.data(), count * sizeof(int16_t));
#ifdef DEBUG_WEIGHT_BIN
        std::cout << mem_op->m_Value->uniqueName() << "\n";
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

void BM188xCodeEmitter::encodeInstructions(const Path &pOutputPath)
{
  Path output_path("cmdbuf.bin");
  if (!pOutputPath.empty())
    output_path = pOutputPath;

  // emit weight file
  std::vector<int8_t> weight_data;
  prepareWeight(weight_data);
  if (!weight_data.empty())
    bmnet::WriteInt8DataToBinaryFile(&weight_data, "cmdbuf.weight.bin");

  auto &instList = m_Backend->getInsts();
  if (m_Backend->getOption().PrintMachineCode) {
    for (auto const &i : instList)
      i->print(onnc::outs());
  }

  // ReadInt8DataFromBinaryFile(weight, weight_data);
  bmnet::BM188xBackendContext ctx(BM_CHIP_BM1880, 1, weight_data);
  bm1880_kernel::getInstance().m_CTX = &ctx;
  // StartInst::encode()
  kernel_enter(ctx.get_bmkernel_handle());

  for (auto const &i : instList)
    i->emit();
  instList.clear();
  // EndInst::encode()
  kernel_submit();
  kernel_exit();
  // output
  std::vector<uint8_t> cmdbuf;
  ctx.read_cmdbuf(cmdbuf);
  bmnet::WriteFloatDataToBinaryFile(cmdbuf.data(), cmdbuf.size(),
                                    output_path.generic_string());
}

void BM188xCodeEmitter::genRuntimeInfo(const ::onnx::Graph *pOnnxGraph)
{
  onnc::json::Object jRoot;
  onnc::json::Object jMemLayout;
  onnc::json::Object jInputThres;
  onnc::json::Object jOutputLayer;
  onnc::json::Object jBatch;
  // Generate memory layout.
  auto &instList = m_Backend->getInsts();
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

  // Generate the threshold of data_layer for quantization.
  std::string dataLayerName = pOnnxGraph->inputs()[0]->uniqueName();
  const tg::bm1880::LayerCalibrationParameter &layerCtable =
      m_Backend->getCtableLayerParam(dataLayerName);
  const float threshold = layerCtable.blob_param(0).threshold_y();
  DEBUG(dbgs() << "data layer name = " << dataLayerName
               << ", threshold = " << threshold << "\n");
  jInputThres.insert("data layer threshold", threshold);

  // Generate batch size of the input.
  auto sizes = pOnnxGraph->inputs()[0]->sizes();
  auto batchSize = sizes[0].dim;
  jBatch.insert("size", batchSize);

  // Generate output layer name.
  std::string onnxOutputLayerName = pOnnxGraph->outputs()[0]->uniqueName();
  jOutputLayer.insert("onnx output", onnxOutputLayerName);
  std::string onncOutputLayerName =
      instList[instList.size() - 1]->getLayerName();
  jOutputLayer.insert("onnc output", onncOutputLayerName);

  // Insert all information to root.
  jRoot.insert("memory layout", jMemLayout);
  jRoot.insert("layer threshold", jInputThres);
  jRoot.insert("output layer", jOutputLayer);
  jRoot.insert("batch", jBatch);

  // FIXME: Remove the hardcode path.
  std::ofstream outfile("runtime.json", std::ofstream::binary);
  onnc::IndentOStream oss(outfile);
  jRoot.print(oss);
  outfile.close();

  return;
}
