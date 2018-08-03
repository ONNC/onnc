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
#include "BM188xBackend.h"
#include "PatternMatch.h"
#include <algorithm>
#include <onnc/ADT/StringRef.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>
#include <vector>
#include <fstream>

using namespace onnc;

using namespace PatternMatch;

namespace {

class AddDummyWeight : public ModulePass
{
public:
  static char ID;

public:
  AddDummyWeight()
    : ModulePass(ID) {
  }

  StringRef getPassName() const override { return "AddDummyWeight"; }
  
  Pass::ReturnType runOnModule(Module &pModule) override;
};

} // namespace

static void addInitializerBase(onnx::Graph *pGraph, const onnx::Value *pValue)
{
  std::string name = pValue->uniqueName();
  auto &names = pGraph->initializer_names();
  if (std::find(names.begin(), names.end(), name) == names.end()) {
    auto count = 1;
    onnx::Tensor newTensor;
    for (auto dim : pValue->sizes()) {
      count *= dim.dim;
      newTensor.sizes().push_back(dim.dim);
    }
    {
      std::vector<float> data(count, 1.0);
      std::string rawData(reinterpret_cast<const char *>(data.data()),
                          count * sizeof(float));
      newTensor.set_raw_data(rawData);
    }
    newTensor.setName(name);
    assert(pValue->elemType() == onnx::TensorProto_DataType_FLOAT);
    newTensor.elem_type() = onnx::TensorProto_DataType_FLOAT;
    pGraph->addInitializer(newTensor, name);
  }
}

char AddDummyWeight::ID = 0;

Pass::ReturnType AddDummyWeight::runOnModule(Module &pModule)
{
  auto &meta_data = pModule.getMetaData();
  auto it = meta_data.find("initializers");
  if (it == meta_data.end())
    return Pass::kModuleNoChanged;

  std::vector<StringRef> inits;
  StringRef(it->second).split(inits, ',');
  onnx::Graph *graph = pModule.getGraphIR().get();
  for (auto init : inits) {
    bool found = false;
    for (const onnx::Value *val : graph->inputs())
      if (val->uniqueName() == init) {
        addInitializerBase(graph, val);
        found = true;
        break;
      }
    if (not found) {
      errs() << "error: initializer " << init
             << " in meta_data does not exist in inputs!\n";
      assert(0);
      exit(1);
    }
  }
  return Pass::kModuleNoChanged;
}

//===----------------------------------------------------------------------===//
// Factory Method
//===----------------------------------------------------------------------===//
ModulePass *onnc::CreateAddDummyWeightPass()
{
  return new AddDummyWeight();
}


//---
namespace {

class ONNXDumpOpt : public ModulePass
{
public:
  static char ID;
  //void updateONNXInt8(onnx::Graph *pGraph);

public:
  ONNXDumpOpt(BM1880Backend *pBackend) : ModulePass(ID), m_pBackend(pBackend)
  {
  }

  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  BM1880Backend *m_pBackend; // NOLINT
};

} // namespace

char ONNXDumpOpt::ID = 0;

ModulePass *onnc::createONNXDumpOptPass(BM1880Backend *pBackend)
{
  return new ONNXDumpOpt(pBackend);
}

Pass::ReturnType ONNXDumpOpt::runOnModule(Module &pModule)
{
  std::string ooo;

  ooo = m_pBackend->getOption().m_OutputOptOnnx;

  if (ooo.empty()) {
    errs() << "error:" << __func__ << "\n";
    exit(1);
  }

  //  onnx::Graph *graph = pModule.getGraphIR().get();
  //updateONNXInt8(graph);

  onnx::ModelProto modelProto;
  onnc::ExportModelProto(modelProto, pModule);
    // wrtie file
  std::fstream output(ooo,
                      std::ios::out | std::ios::trunc | std::ios::binary);
  modelProto.SerializeToOstream(&output);
  return Pass::kModuleNoChanged;


  /*
      errs() << "error: initializer " << init
             << " in meta_data does not exist in inputs!\n";
      assert(0);
      exit(1);
   */
  ///
}

#if 0
void ONNXDumpOpt::updateONNXInt8(onnx::Graph *pGraph)
{
  // update elemType
  for (auto input : pGraph->inputs()) {
    auto elemType = input->elemType();

    if (elemType == onnx::TensorProto_DataType_FLOAT) {
      auto name = input->uniqueName();

      if (m_QWeights.count(name)) {
        input->setElemType(onnx::TensorProto_DataType_INT8);
      } else if (m_QBias.count(name)) {
        input->setElemType(onnx::TensorProto_DataType_INT16);
      } else {
        // input data default is INT8
        input->setElemType(onnx::TensorProto_DataType_INT8);
      }
    } else {
      // FIXME
      std::cout << "FIXME: unsupported quantize type:"
                << TensorProto_DataType_Name(elemType) << std::endl;
      exit(1);
    }
  }

  // update node's output elementType
  for (onnx::Node *node : pGraph->nodes()) {
    for (onnx::Value *out : node->outputs()) {
      out->setElemType(onnx::TensorProto_DataType_INT8);
    }
  }

  // update Tensor
  std::unordered_map<std::string, onnx::Tensor> valueTensorMap;
  const std::vector< onnx::Tensor> initTensors = pGraph->initializers();
  const std::vector<std::string> tensorNames = pGraph->initializer_names();

  for (size_t i = 0; i < initTensors.size(); i++) {
    auto valueName = tensorNames[i];
    auto oldTensor = initTensors[i];

    if (m_QWeights.count(valueName) == 1) {
      onnx::Tensor newTensor;
      copyTensor(newTensor, oldTensor, valueName,
                 onnx::TensorProto_DataType_INT8);
      assert(m_QWeights[valueName].size() ==
             ::onnc::getTotalCount(oldTensor.sizes()));
      copyData2Tensor(newTensor, m_QWeights[valueName]);
      valueTensorMap.emplace(valueName, newTensor);
      continue;
    } else if (1 == m_QBias.count(valueName)) {
      onnx::Tensor newTensor;
      copyTensor(newTensor, oldTensor, valueName,
                 onnx::TensorProto_DataType_INT16);
      assert(m_QBias[valueName].size() ==
             ::onnc::getTotalCount(oldTensor.sizes()));
      copyData2Tensor(newTensor, m_QBias[valueName]);
      valueTensorMap.emplace(valueName, newTensor);
      continue;
    }
    // FIXME
    std::cout << "FIXME: unsupported tensor:" << oldTensor.name() << std::endl;
    valueTensorMap.insert({ valueName, oldTensor });
  }

  pGraph->clearInitializers();
  for (auto &kv : valueTensorMap) {
    pGraph->addInitializer(kv.second, kv.first);
  }
}
#endif
