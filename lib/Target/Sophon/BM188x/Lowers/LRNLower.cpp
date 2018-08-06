//===- LRNLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LRNLower.h"
#include "../Compute/LRN.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// LRNLower
//===----------------------------------------------------------------------===//
BM188X::LRNLower::LRNLower()
{
}

BM188X::LRNLower::~LRNLower()
{
}

int BM188X::LRNLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("LRN"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::LRNLower::activate(ComputeGraph& pGraph,
                                                   xNode &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
    return nullptr;

  for (xValue* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (xValue* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check required attributes
  if (!pNode.hasAttribute(xSymbol("size")))
    return nullptr;

  // create operators
  BM188X::LRN *op =
    pGraph.addOperator<onnc::BM188X::LRN>(pNode.i(xSymbol("size")));

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("alpha")))
    op->setAlpha(pNode.f(xSymbol("alpha")));
  if (pNode.hasAttribute(xSymbol("beta")))
    op->setBeta(pNode.f(xSymbol("beta")));
  if (pNode.hasAttribute(xSymbol("bias")))
    op->setBias(pNode.f(xSymbol("bias")));

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
