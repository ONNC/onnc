//===- ConcatLower.cpp ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ConcatLower.h"
#include "PatternMatch.h"
#include "../Compute/Concat.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

namespace pm = onnc::PatternMatch;

//===----------------------------------------------------------------------===//
// ConcatLower
//===----------------------------------------------------------------------===//
BM188X::ConcatLower::ConcatLower()
{
}

BM188X::ConcatLower::~ConcatLower()
{
}

int BM188X::ConcatLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Concat"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::ConcatLower::activate(ComputeGraph& pGraph,
                                                   ::onnx::Node &pNode) const
{
  // check input/output name
  if (pNode.inputs().empty())
    return nullptr;

  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (1 != pNode.outputs().size())
    return nullptr;

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("axis")))
    return nullptr;

  // create operators
  BM188X::Concat* op = pGraph.addOperator<onnc::BM188X::Concat>(
    pNode.i(::onnx::Symbol("axis")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addInput(*tensor);
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(xv->uniqueName());
    if (nullptr == tensor)
      tensor = IRBuilder::CreateComputeTensor(pGraph, *xv);
    op->addOutput(*tensor);
  }
  return op;
}
