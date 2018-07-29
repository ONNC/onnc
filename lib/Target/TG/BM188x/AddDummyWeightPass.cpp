//===- AddDummyWeightPass.cpp ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BM188xBackend.h"
#include "PatternMatch.h"
#include <algorithm>
#include <onnc/ADT/StringRef.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>
#include <vector>

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
