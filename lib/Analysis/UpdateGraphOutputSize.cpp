//===- UpdateGraphOutputSize.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Support/IOStream.h>
#include <onnx/common/interned_strings.h>

#include <unordered_set>

using TensorSize = std::vector<onnx::Dimension>;
using TP_DataTy = onnx::TensorProto_DataType;

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
static void UpdateOutputInfo(onnx::Node* pNode, const TensorSize &pSize,
                             TP_DataTy pTy)
{
  for (onnx::Value* out : pNode->outputs()) {
    out->setElemType(pTy);
    if (out->sizes().empty())
      out->setSizes(pSize);
  }
}

static void UpdateOutputInfoByInput(onnx::Node* pNode)
{
  const onnx::Value* in = pNode->inputs()[0];

  if (in->elemType() == onnx::TensorProto_DataType_UNDEFINED) {
    outs() << in->uniqueName() << ": Undefined element type.\n";
    return;
  }

  //assert(in->elemType() != onnx::TensorProto_DataType_UNDEFINED &&
  //       "Undefined element type.");

  if (in->sizes().empty()) {
    outs() << "No available input size to update output size.\n"
           << "  Input value : " << in->uniqueName() << "\n"
           << "  Output value : " << pNode->outputs()[0]->uniqueName() << "\n";
    return;
  }

  UpdateOutputInfo(pNode, in->sizes(), in->elemType());
}

//===----------------------------------------------------------------------===//
// UpdateGraphOutputSize
//===----------------------------------------------------------------------===//
UpdateGraphOutputSize::UpdateGraphOutputSize()
  : ModulePass(ID) {
}

/// Operator set whose output size equal to input size.
static std::unordered_set<onnx::NodeKind> g_InputSizeIsOutputSize = {
  onnx::Symbol("Relu"), onnx::Symbol("kLRN"),
  onnx::Symbol("Dropout"), onnx::Symbol("Softmax")
};

bool UpdateGraphOutputSize::runOnModule(Module& pModule)
{
  for (onnx::Node *n : pModule.getGraph()->nodes()) {
    if (g_InputSizeIsOutputSize.count(n->kind())) {
      UpdateOutputInfoByInput(n);
      continue;
    }
  }

  return true;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char UpdateGraphOutputSize::ID = 0;

UpdateGraphOutputSize *onnc::CreateUpdateGraphOutputSizePass()
{
  return new UpdateGraphOutputSize();
}
