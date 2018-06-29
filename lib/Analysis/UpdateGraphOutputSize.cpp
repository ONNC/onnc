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

typedef std::vector<::onnx::Dimension> TensorSizes;
typedef ::onnx::TensorProto_DataType TP_DataTy;

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static void UpdateOutputInfo(::onnx::Node *pNode, const TensorSizes &pSize,
                             TP_DataTy pTy)
{
  for (::onnx::Value* out : pNode->outputs()) {
    out->setElemType(pTy);
    out->setSizes(pSize);
  }
}

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
void UpdateGraphOutputSize::updateReshapeOutputInfo(::onnx::Node *pNode)
{
  // second input is a shape tensor
  const ::onnx::Value *input1 = pNode->inputs()[1];
  const ::onnx::Tensor shapeTensor =
      getTensor(input1->uniqueName(), *pNode->owningGraph());
  TensorSizes dims;

  // tensor is raw data
  if (0 == shapeTensor.int64s().size()) {
    size_t size = getTotalCount(shapeTensor.sizes());
    std::vector<int64_t> int64Dims;
    int64Dims.reserve(size);
    int64Dims.resize(size);
    memcpy(int64Dims.data(), shapeTensor.raw().data(), size * sizeof(int64_t));
    for (auto &i : int64Dims) {
      dims.push_back(::onnx::Dimension(i));
    }
  } else {
    for (auto &i : shapeTensor.int64s()) {
      dims.push_back(::onnx::Dimension(i));
    }
  }
  // user specific batch_size
  if (m_BatchSize > 0)
    dims[0] = m_BatchSize;
  // First input is the data tensor
  const ::onnx::Value *in = pNode->inputs()[0];
  UpdateOutputInfo(pNode, dims, in->elemType());
}

//===----------------------------------------------------------------------===//
// UpdateGraphOutputSize
//===----------------------------------------------------------------------===//
UpdateGraphOutputSize::UpdateGraphOutputSize()
  : ModulePass(ID) {
  m_BatchSize = getBatchSize();
}

void UpdateGraphOutputSize::updateInputBatchSize(onnx::Graph *pGraph)
{
  // update input batch size
  std::unordered_set<std::string> initializer_names(
      pGraph->initializer_names().begin(), pGraph->initializer_names().end());
  for (int i = 0; i < pGraph->inputs().size(); ++i) {
    onnx::Value *v = pGraph->inputs()[i];
    // ignore weight
    if (0 != initializer_names.count(v->uniqueName()))
      continue;
    // update valueInfo
    auto sizes = v->sizes();
    sizes[0] = m_BatchSize;
    v->setSizes(sizes);
  }
}

void UpdateGraphOutputSize::updateOutputValueInfo(onnx::Graph *pGraph)
{
  auto outputs = pGraph->outputs();
  std::unordered_set<onnx::Value *> output_set(outputs.begin(), outputs.end());
  // reset output valueInfo
  for (onnx::Node *n : pGraph->nodes()) {
    onnx::Value *out = n->outputs()[0];
    // do not reset graph's outputs valueInfo
    if (output_set.count(out))
      continue;
    // reset dimension and elemType
    std::vector<onnx::Dimension> sizes;
    out->setSizes(sizes);
    out->setElemType(onnx::TensorProto_DataType_UNDEFINED);
  }

  // update graph's outputs valueInfo
  for (onnx::Value *out : outputs) {
    auto sizes = out->sizes();
    sizes[0] = m_BatchSize;
    out->setSizes(sizes);
  }
}

Pass::ReturnType UpdateGraphOutputSize::runOnModule(Module &pModule)
{
  onnx::Graph *graph = pModule.getGraphIR().get();

  // update input batch size and reset old output valueInfo
  if (m_BatchSize > 0) {
    updateInputBatchSize(graph);
    updateOutputValueInfo(graph);
  }

  // onnc only update reshape's output valueInfo
  for (::onnx::Node *n : graph->nodes()) {
    const auto kind = n->kind();
    if (kind == ::onnx::kReshape) {
      updateReshapeOutputInfo(n);
    }
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
}

void onnc::InitializeUpdateGraphOutputSizePassOptions()
{
  // initialisze all options of UpdateGraphOutputSize pass.
  getBatchSize();
}

UpdateGraphOutputSize *onnc::CreateUpdateGraphOutputSizePass()
{
  return new UpdateGraphOutputSize();
}
