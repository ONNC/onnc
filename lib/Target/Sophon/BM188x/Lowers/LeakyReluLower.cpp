//===- LeakyReluLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LeakyReluLower.h"
#include "../Compute/LeakyRelu.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// LeakyReluLower
//===----------------------------------------------------------------------===//
BM188X::LeakyReluLower::LeakyReluLower()
{
}

BM188X::LeakyReluLower::~LeakyReluLower()
{
}

int BM188X::LeakyReluLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("LeakyRelu"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::LeakyReluLower::activate(ComputeGraph& pGraph,
                                                      xNode &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
    return nullptr;

  xValue* ox = pNode.inputs()[0];
  if (!ox->has_unique_name())
    return nullptr;

  if (1 != pNode.outputs().size())
    return nullptr;
  xValue* oy = pNode.outputs()[0];
  if (!oy->has_unique_name())
    return nullptr;

  const auto &inDim = ox->sizes();
  if (inDim.size() != 4 && inDim.size() != 2) {
    errs() << "BM188X::ReluLower: Invalid dim size.\n";
    return nullptr;
  }

  // create operators
  BM188X::LeakyRelu *op =
    pGraph.addOperator<onnc::BM188X::LeakyRelu>(
      pNode.f(xSymbol("alpha")));

  // set dims.
  IntsAttr dim = IntsAttr(4, 1);
  if (inDim.size() == 4) {
    for (int i = 0; i < 4; ++i)
      dim.vector()[i] = inDim[i].dim;
  } else {
    dim.vector()[0] = inDim[0].dim;
    dim.vector()[2] = inDim[1].dim;
  }

  op->setDims(dim);

  // set input/output
  for (xValue* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (xValue* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }

  return op;
}
