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
  AddDummyWeight(BM1880Backend *pBackend) : ModulePass(ID), m_pBackend(pBackend)
  {
  }

  StringRef getPassName() const override { return "AddDummyWeight"; }
  
  Pass::ReturnType runOnModule(Module &pModule) override;

private:
  BM1880Backend *m_pBackend; // NOLINT
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

Pass::ReturnType AddDummyWeight::runOnModule(Module &pModule)
{
  if (not m_pBackend->getOption().m_AddDummyWeight)
    return Pass::kModuleNoChanged;

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

char AddDummyWeight::ID = 0;

ModulePass *onnc::createAddDummyWeightPass(BM1880Backend *pBackend)
{
  return new AddDummyWeight(pBackend);
}
