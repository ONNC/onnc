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

int BM188X::GemmLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Gemm"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::GemmLower::activate(ComputeGraph& pGraph,
                                                   ::onnx::Node &pNode) const
{
  // check input/output name
  if (3 == pNode.inputs().size())
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

  // create operators
  BM188X::Gemm* op = pGraph.addOperator<onnc::BM188X::Gemm>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("alpha")))
    op->setAlpha(pNode.f(::onnx::Symbol("alpha")));
  if (pNode.hasAttribute(::onnx::Symbol("beta")))
    op->setBeta(pNode.f(::onnx::Symbol("beta")));
  if (pNode.hasAttribute(::onnx::Symbol("transA")))
    op->setTransA(pNode.i(::onnx::Symbol("transA")));
  if (pNode.hasAttribute(::onnx::Symbol("transB")))
    op->setTransB(pNode.i(::onnx::Symbol("transB")));

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

  op->init(pNode);
  return op;
}
