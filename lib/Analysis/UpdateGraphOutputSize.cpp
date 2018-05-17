//===- UpdateGraphOutputSize.cpp ------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Analysis/UpdateGraphOutputSize.h>
#include <onnc/Core/InitializePasses.h>
#include <onnc/Support/IOStream.h>
#include <onnx/common/interned_strings.h>
#include <onnc/IR/ONNXUtils.h>

#include <unordered_set>

using TensorSizes = std::vector<::onnx::Dimension>;
using TP_DataTy = ::onnx::TensorProto_DataType;
using LongIntVec = std::vector<int64_t>;

using namespace onnc;

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//

static void UpdateOutputInfo(::onnx::Node *pNode, const TensorSizes &pSize,
                             TP_DataTy pTy)
{
  for (::onnx::Value* out : pNode->outputs()) {
    out->setElemType(pTy);
    if (out->sizes().empty())
      out->setSizes(pSize);
  }
}

static void UpdateOutputInfoByInput(::onnx::Node* pNode)
{
  const ::onnx::Value* in = pNode->inputs()[0];

  if (in->elemType() == ::onnx::TensorProto_DataType_UNDEFINED) {
    errs() << in->uniqueName() << ": Undefined element type.\n";
    return;
  }

  //assert(in->elemType() != ::onnx::TensorProto_DataType_UNDEFINED &&
  //       "Undefined element type.");

  if (in->sizes().empty()) {
    errs() << "No available input size to update output size.\n"
           << "  Input value : " << in->uniqueName() << "\n"
           << "  Output value : " << pNode->outputs()[0]->uniqueName() << "\n";
    return;
  }

  UpdateOutputInfo(pNode, in->sizes(), in->elemType());
}

static void GetAttrVals(::onnx::Node* pNode, ::onnx::BuiltinSymbol pAttr,
                        LongIntVec& pVal)
{
  if (pNode->hasAttribute(pAttr)) {
    const auto &attr = pNode->is(pAttr);
    pVal.resize(attr.size());
    for (int i = 0; i < attr.size(); ++i)
      pVal[i] = attr[i];
  }
}

static void GetPads(::onnx::Node* pNode, LongIntVec& pPadsB, LongIntVec& pPadsE)
{
  if (pNode->hasAttribute(::onnx::kpads)) {
    // get pads begin and offset to pads end.
    const auto &pads = pNode->is(::onnx::kpads);
    const size_t padEndOffset = pads.size() / 2;

    for (int i = 0; i < padEndOffset; ++i) {
      pPadsB[i] = pads[i];
      pPadsE[i] = pads[i + padEndOffset];
    }
  }
}

static void UpdateConvOutputInfo(::onnx::Node* pNode)
{
  const TensorSizes &xDim = pNode->inputs()[0]->sizes(),
                    &wDim = pNode->inputs()[1]->sizes();

  //assert(!xDim.empty() && !wDim.empty() &&
  //       "No available input dimension for Conv.");
  if (xDim.empty() || wDim.empty())
    return;

  const size_t numAxis = xDim.size() - 2;
  std::vector<int64_t> kShape(numAxis),
                       strides(numAxis, 1),
                       padsB(numAxis, 0), padsE(numAxis, 0);

  if (pNode->hasAttribute(::onnx::kkernel_shape)) {
    GetAttrVals(pNode, ::onnx::kkernel_shape, kShape);
  } else {
    // If the kernel shape is not present, it should be inferred from input W.
    for (int i = 0; i < numAxis; ++i)
      kShape[i] = wDim[i + 2].dim;
  }

  GetAttrVals(pNode, ::onnx::kstrides, strides);
  GetPads(pNode, padsB, padsE);

  // output dimensions.
  TensorSizes yDim(xDim.size(), ::onnx::Dimension(0));

  // setup output N, C.
  // Note: Channel of ouput (yDim) is equal to the number of feature weight,
  //       i.e. channel of weight (wDim).
  yDim[0] = xDim[0]; // N
  yDim[1] = wDim[0]; // C

  // Calculate output size for each dimension.
  for (int i = 0; i < numAxis; ++i) {
    // Formula:
    //   (xDim[i+2] - (kernelShap[i] / 2) * 2
    //              + padsBegin[i] + padsEnd[i]) / strides[i] + 1
    //
    // e.g. dim = 2 (i.e. W, H), xDim = [5, 5], kernel shape = [3, 3],
    //      pads = [1, 1, 1, 1], strides = [1, 1]
    //
    //      yDim (output) = [(5-3+1+1)/1 + 1, (5-3+1+1)/1 + 1] = [5, 5]
    int64_t d = xDim[i + 2].dim - kShape[i] + padsB[i] + padsE[i];
    d /= strides[i];
    d += 1;
    yDim[i + 2] = ::onnx::Dimension(d);
  }
  UpdateOutputInfo(pNode, yDim, pNode->inputs()[0]->elemType());
}

static void UpdatePoolOutputInfo(::onnx::Node* pNode)
{
  const TensorSizes &xDim = pNode->inputs()[0]->sizes();

  //assert(!xDim.empty() && !wDim.empty() &&
  //       "No available input dimension for Conv.");
  // FIXME: remove this after supporting all kinds of nodes.
  if (xDim.empty())
    return;

  const size_t numAxis = xDim.size() - 2;
  LongIntVec kShape,
             strides(numAxis, 1),
             padsB(numAxis, 0), padsE(numAxis, 0);

  GetAttrVals(pNode, ::onnx::kkernel_shape, kShape);
  GetAttrVals(pNode, ::onnx::kstrides, strides);
  GetPads(pNode, padsB, padsE);

  // output dimensions.
  TensorSizes yDim(xDim.size(), ::onnx::Dimension(0));

  // setup output N, C.
  yDim[0] = xDim[0]; // N
  yDim[1] = xDim[1]; // C

  // Calculate output size for each dimension.
  for (int i = 0; i < numAxis; ++i) {
    int64_t d = xDim[i + 2].dim - kShape[i] + padsB[i] + padsE[i];
    d /= strides[i];
    d += 1;
    yDim[i + 2] = ::onnx::Dimension(d);
  }

  UpdateOutputInfo(pNode, yDim, pNode->inputs()[0]->elemType());
}

static void UpdateGemmOutputInfo(::onnx::Node* pNode)
{
  // Directly use matrix C's dimension if kbroadcast attribute has no effect.
  if (!pNode->hasAttribute(::onnx::kbroadcast) ||
      !pNode->i(::onnx::kbroadcast)) {
    const TensorSizes &MatCdim = pNode->inputs()[2]->sizes();
    UpdateOutputInfo(pNode, MatCdim, pNode->inputs()[0]->elemType());
  }
  else {
    const TensorSizes &aDim = pNode->inputs()[0]->sizes(),
                      &bDim = pNode->inputs()[1]->sizes();
    // A: M x K
    ::onnx::Dimension oM = aDim[0];
    if (pNode->hasAttribute(::onnx::ktransA) &&
        pNode->i(::onnx::ktransA))
      oM = aDim[1].dim;

    // B: K x N
    ::onnx::Dimension oN = bDim[1];
    if (pNode->hasAttribute(::onnx::ktransB) &&
        pNode->i(::onnx::ktransB))
      oN = bDim[0].dim;
    UpdateOutputInfo(pNode, {oM, oN}, pNode->inputs()[0]->elemType());
  }
}

static void UpdateReshapeOutputInfo(::onnx::Node *pNode)
{
  // second input is a shape tensor
  const ::onnx::Value *input1 = pNode->inputs()[1];
  const ::onnx::Tensor shapeTensor =
      ::onnc::onnx::getTensor(input1->uniqueName(), *pNode->owningGraph());
  TensorSizes dims;

  // tensor is raw data
  if (0 == shapeTensor.int64s().size()) {
    size_t size = ::onnc::onnx::getTotalCount(shapeTensor.sizes());
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
  // First input is the data tensor
  const ::onnx::Value *in = pNode->inputs()[0];
  UpdateOutputInfo(pNode, dims, in->elemType());
}

//===----------------------------------------------------------------------===//
// UpdateGraphOutputSize
//===----------------------------------------------------------------------===//
UpdateGraphOutputSize::UpdateGraphOutputSize()
  : ModulePass(ID) {
}

/// Operator set whose output size equals to input size.
static std::unordered_set<::onnx::NodeKind> g_InputSizeIsOutputSize = {
  ::onnx::Symbol("Relu"), ::onnx::Symbol("LRN"),
  ::onnx::Symbol("Dropout"), ::onnx::Symbol("Softmax")
};

Pass::ReturnType UpdateGraphOutputSize::runOnModule(Module& pModule)
{
  for (::onnx::Node *n : pModule.getGraph()->nodes()) {
    const auto kind = n->kind();
    std::cout << "update inst Kind:" << kind.toString() << "\n";
    if (g_InputSizeIsOutputSize.count(kind)) {
      UpdateOutputInfoByInput(n);
      continue;
    }

    if (kind == ::onnx::kConv) {
      UpdateConvOutputInfo(n);
    } else if (kind == ::onnx::Symbol("MaxPool")) {
      UpdatePoolOutputInfo(n);
    } else if (kind == ::onnx::kGemm) {
      UpdateGemmOutputInfo(n);
    } else if (kind == ::onnx::kReshape) {
      UpdateReshapeOutputInfo(n);
    } else {
      errs() << "Unsupported type :" << kind.toString() << "\n";
    }
  }

  return kModuleChanged;
}

//===----------------------------------------------------------------------===//
// Factory method
//===----------------------------------------------------------------------===//
char UpdateGraphOutputSize::ID = 0;

namespace onnc
{
  INITIALIZE_PASS(UpdateGraphOutputSize, "UpdateGraphOutputSize")
}

UpdateGraphOutputSize *onnc::CreateUpdateGraphOutputSizePass()
{
  return new UpdateGraphOutputSize();
}
