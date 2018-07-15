#include "BM188xBackend.h"
#include "PatternMatch.h"
#include <algorithm>
#include <onnc/Core/ModulePass.h>
#include <onnc/Core/PassSupport.h>
#include <onnc/Target/TG/BM188x/common_calibration2.pb.h>
#include <onnx/common/ir.h>

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
static bool isConstantInput(const onnx::Node *pNode, size_t pIndex)
{
  // clang-format off
  std::map<std::string, std::vector<size_t> > table = { { "Conv", { 1, 2 } },
                                                        { "BatchNormalization",
                                                          { 1, 2, 3, 4 } },
                                                        { "Mul", { 1 } },
                                                        { "Add", { 1 } },
                                                        { "PRelu", { 1 } },
                                                        { "Unsqueeze", { 0 } },
                                                        { "Gemm", { 1, 2 } } };
  // clang-format on
  for (auto &t : table)
    if (match(pNode, mSymbol(t.first))) {
      if (std::find(t.second.begin(), t.second.end(), pIndex) != t.second.end())
        return true;
    }
  return false;
}

Pass::ReturnType AddDummyWeight::runOnModule(Module &pModule)
{
  if (not m_pBackend->getOption().m_AddDummyWeight)
    return Pass::kModuleNoChanged;
  onnx::Graph *graph = pModule.getGraphIR().get();
  for (const auto *node : graph->nodes()) {
    for (size_t i = 0; i < node->inputs().size(); i++)
      if (isConstantInput(node, i))
        addInitializerBase(graph, node->inputs()[i]);
  }
  return Pass::kModuleNoChanged;
}

char AddDummyWeight::ID = 0;

ModulePass *onnc::createAddDummyWeightPass(BM1880Backend *pBackend)
{
  return new AddDummyWeight(pBackend);
}
