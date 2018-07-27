//===- GlobalAveragePoolLower.cpp -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "GlobalAveragePoolLower.h"
#include "../Compute/GlobalAveragePool.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// GlobalAveragePoolLower
//===----------------------------------------------------------------------===//
BM188X::GlobalAveragePoolLower::GlobalAveragePoolLower()
{
}

BM188X::GlobalAveragePoolLower::~GlobalAveragePoolLower()
{
}

int BM188X::GlobalAveragePoolLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("GlobalAveragePool"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::GlobalAveragePoolLower::activate(ComputeGraph& pGraph,
                                   ::onnx::Node &pNode) const
{
  // check input/output name
  if (1 != pNode.inputs().size())
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
  BM188X::GlobalAveragePool* op =
    pGraph.addOperator<onnc::BM188X::GlobalAveragePool>();

  if (pNode.hasAttribute(::onnx::Symbol("enable_relu")))
    op->setEnableRelu(pNode.i(::onnx::Symbol("enable_relu")));

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
