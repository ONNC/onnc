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
static void UpdateOutputInfo(onnx::Node *pNode, const TensorSizes &pSize,
                             TP_DataTy pTy)
{
  for (::onnx::Value* out : pNode->outputs()) {
    out->setElemType(pTy);
    out->setSizes(pSize);
  }
}

static std::vector<int64_t> getReshapeOpDestDims(onnx::Node *pReshapeOp)
{
  // second input is a shape tensor
  const onnx::Value *input1 = pReshapeOp->inputs()[1];
  const onnx::Tensor &shapeTensor =
      getTensor(input1->uniqueName(), *pReshapeOp->owningGraph());
  std::vector<int64_t> dims;
  // tensor is raw data
  if (0 == shapeTensor.int64s().size()) {
    size_t size = getTotalCount(shapeTensor.sizes());
    dims.resize(size);
    memcpy(dims.data(), shapeTensor.raw().data(), size * sizeof(int64_t));
  } else {
    for (auto &i : shapeTensor.int64s()) {
      dims.push_back(i);
    }
  }
  return dims;
}

static int64_t getDimensionTotalCount(std::vector<onnx::Dimension> &pDims)
{
  int64_t count = 1;
  for (auto &i : pDims) {
    assert(i.is_int);
    count *= i.dim;
  }
  return count;
}

static void updateReshapeOpDestDimension(onnx::Graph *pGraph, onnx::Node *pNode,
                                         std::vector<int64_t> &pDestDims)
{
  // calculate new shape dims
  std::vector<onnx::Dimension> input_dims = pNode->inputs()[0]->sizes();
  std::vector<int64_t> new_shape_dims;
  int remaining_idx = -1;
  size_t cur_total_count = 1;
  for (size_t i = 0; i < pDestDims.size(); ++i) {
    // take dimension from input tensor
    if (0 == pDestDims[i]) {
      assert(input_dims[i].is_int);
      new_shape_dims.push_back(input_dims[i].dim);
      cur_total_count *= input_dims[i].dim;
    } else if (-1 == pDestDims[i]) {
      // at most one dimension can be -1
      assert(remaining_idx == -1);
      remaining_idx = i;
      // update later
      new_shape_dims.push_back(-1);
    } else {
      assert(pDestDims[i] > 0);
      new_shape_dims.push_back(pDestDims[i]);
      cur_total_count *= (size_t)pDestDims[i];
    }
  }
  // update remaining dimension
  size_t dim_total_count = getDimensionTotalCount(input_dims);
  if (remaining_idx != -1) {
    assert(dim_total_count % cur_total_count == 0);
    new_shape_dims[remaining_idx] = dim_total_count / cur_total_count;
  }
  pDestDims = new_shape_dims;

  // update reshape's shape tensor
  onnx::Tensor dest_dims;
  dest_dims.sizes().push_back(new_shape_dims.size());
  dest_dims.elem_type() = onnx::TensorProto_DataType_INT64;
  dest_dims.set_raw_data(std::string((char *)new_shape_dims.data(),
                         new_shape_dims.size() * sizeof(int64_t)));
  std::string name = pNode->inputs()[0]->uniqueName() + "reshape";
  onnx::Value *new_dest_dims_value =
      pGraph->addInitializerAndInput(dest_dims, name);
  auto *old_dest_dims_value = pNode->inputs()[1];
  pNode->replaceInput(1, new_dest_dims_value);
  if (old_dest_dims_value->uses().size() == 0) {
    pGraph->eraseInitializerAndInput(old_dest_dims_value);
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

bool UpdateGraphOutputSize::updateReshapeOutputInfo(onnx::Graph *pGraph)
{
  bool require_onnxInferShape = false;
  for (onnx::Node *node : pGraph->nodes()) {
    const auto kind = node->kind();
    if (kind != onnx::kReshape)
      continue;

    // check special case, the output dimension(shape) can be 0 or -1
    // At most one dimension of the new shape can be -1. In this case, the
    // value is inferred from the size of the tensor and the remaining
    // dimensions. A dimension could also be 0, in which case the actual
    // dimension value is unchanged (i.e. taken from the input tensor).
    // this case require input tensor dimension value
    std::vector<int64_t> dest_dims = getReshapeOpDestDims(node);
    if (std::find(dest_dims.begin(), dest_dims.end(), 0) != dest_dims.end() or
        std::find(dest_dims.begin(), dest_dims.end(), -1) != dest_dims.end()) {
      onnx::Value *input0 = node->inputs()[0];
      if (input0->sizes().size() == 0) {
        // input tensor dimension is empty, need to run onnx shape inference
        // first
        return true;
      }
      // update shape tensor with exact dimension value
      updateReshapeOpDestDimension(pGraph, node, dest_dims);
      require_onnxInferShape = true;
    }

    // user specific batch_size
    if (m_BatchSize > 0)
      dest_dims[0] = m_BatchSize;

    // First input is the data tensor
    const onnx::Value *in = node->inputs()[0];
    TensorSizes dims;
    for (auto &i : dest_dims) {
      dims.push_back(onnx::Dimension(i));
    }
    UpdateOutputInfo(node, dims, in->elemType());
  }
  return require_onnxInferShape;
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
    for (onnx::Value *out : n->outputs()) {
      // do not reset graph's outputs valueInfo
      if (output_set.count(out))
        continue;
      // reset dimension and elemType
      std::vector<onnx::Dimension> sizes;
      out->setSizes(sizes);
      out->setElemType(onnx::TensorProto_DataType_UNDEFINED);
    }
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
  onnx::Graph *graph = pModule.getRootTensorGraph();

  // update input batch size and reset old output valueInfo
  if (m_BatchSize > 0) {
    updateInputBatchSize(graph);
    updateOutputValueInfo(graph);
  }

  bool run_onnxInferShape = false;
  do {
    run_onnxInferShape = updateReshapeOutputInfo(graph);
    if (run_onnxInferShape) {
      // onnx inference will create the new module
      onnxInferShape(pModule);
      graph = pModule.getGraphIR().get();
    }
  } while (run_onnxInferShape);

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
