//===- GemmLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GemmLower.h"
#include "PatternMatch.h"
#include "../Compute/Gemm.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

namespace pm = onnc::PatternMatch;

//===----------------------------------------------------------------------===//
// GemmLower
//===----------------------------------------------------------------------===//
BM188X::GemmLower::GemmLower()
{
}

BM188X::GemmLower::~GemmLower()
{
}

int BM188X::GemmLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("Gemm"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::GemmLower::activate(ComputeGraph& pGraph,
                                                   xNode &pNode) const
{
  // check input/output name
  if (3 == pNode.inputs().size())
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

  // create operators
  BM188X::Gemm* op = pGraph.addOperator<onnc::BM188X::Gemm>();

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("alpha")))
    op->setAlpha(pNode.f(xSymbol("alpha")));
  if (pNode.hasAttribute(xSymbol("beta")))
    op->setBeta(pNode.f(xSymbol("beta")));
  if (pNode.hasAttribute(xSymbol("transA")))
    op->setTransA(pNode.i(xSymbol("transA")));
  if (pNode.hasAttribute(xSymbol("transB")))
    op->setTransB(pNode.i(xSymbol("transB")));

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

  op->init(pNode);
  return op;
}
