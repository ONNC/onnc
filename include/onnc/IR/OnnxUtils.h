//===- OnnxUtils.h --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements onnx utilities.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ONNX_UTILS_H
#define ONNC_ONNX_UTILS_H
#include <onnx/common/ir.h>

namespace onnc {

using TensorSizes = std::vector<onnx::Dimension>;
using LongInts = std::vector<int64_t>;

/// Operator set whose output size equals to input size.
/// (FIXME: Currently, this also implies: y[i] = some operation on x[i])
const static std::unordered_set<onnx::NodeKind> g_OutputSizeIsInputSize = {
  onnx::Symbol("Relu"), onnx::Symbol("LRN"),
  onnx::Symbol("Dropout"), onnx::Symbol("Softmax"),
  onnx::kBatchNormalization,
  onnx::kMul, onnx::kDiv, onnx::kAdd, onnx::kSub, onnx::kNeg,
  onnx::Symbol("Sum")
};

inline bool OutputSizeIsInputSize(onnx::Node& pNode)
{
  return g_OutputSizeIsInputSize.count(pNode.kind()) != 0;
}

inline void GetAttrVals(onnx::Node& pNode, onnx::BuiltinSymbol pAttr,
                        LongInts& pVal)
{
  if (pNode.hasAttribute(pAttr)) {
    const auto &attr = pNode.is(pAttr);
    pVal.resize(attr.size());
    for (int i = 0; i < attr.size(); ++i)
      pVal[i] = attr[i];
  }
}

inline void GetPads(onnx::Node& pNode, LongInts& pPadsB, LongInts& pPadsE)
{
  if (pNode.hasAttribute(onnx::kpads)) {
    // get pads begin and offset to pads end.
    const auto& pads = pNode.is(onnx::kpads);
    const size_t padEndOffset = pads.size() / 2;

    pPadsB.resize(padEndOffset);
    pPadsE.resize(padEndOffset);
    for (int i = 0; i < padEndOffset; ++i) {
      pPadsB[i] = pads[i];
      pPadsE[i] = pads[i + padEndOffset];
    }
  }
}

inline void GetConvKernelShape(onnx::Node& pNode, LongInts& pKShape)
{
  assert(pNode.kind() == onnx::kConv && "This is not a convolution node.");
  const TensorSizes& wDim = pNode.inputs()[1]->sizes();
  const size_t numAxis = wDim.size() - 2;

  if (pNode.hasAttribute(onnx::kkernel_shape)) {
    GetAttrVals(pNode, onnx::kkernel_shape, pKShape);
  } else {
    pKShape.resize(numAxis);
    // If the kernel shape is not present, it should be inferred from input W.
    for (int i = 0; i < numAxis; ++i)
      pKShape[i] = wDim[i + 2].dim;
  }
}

/// @param pAttr Can be 'onnx::ktransA', 'onnx::ktransB'
inline bool IsTranspose(onnx::Node& pNode, onnx::BuiltinSymbol pAttr)
{
  assert((pAttr == onnx::ktransA || pAttr == onnx::ktransB) &&
         "This is not transpose attribute.");
  if (pNode.hasAttribute(pAttr) &&
      pNode.i(pAttr))
    return true;
  return false;
}

}

#endif
