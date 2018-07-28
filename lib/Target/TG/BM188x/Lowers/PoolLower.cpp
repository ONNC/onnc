//===- PoolLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "PoolLower.h"
#include "../Compute/Pool.h"
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/Compute/Attributes.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// PoolLower
//===----------------------------------------------------------------------===//
BM188X::PoolLower::PoolLower()
{
}

BM188X::PoolLower::~PoolLower()
{
}

int BM188X::PoolLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.hasAttribute(::onnx::Symbol("is_sliced"))) {
    auto is_sliced = pNode.i(::onnx::Symbol("is_sliced"));
    if (is_sliced) {
      // higher than BM188X::MaxPool and BM188X::AveragePool.
      return kTargetHigh;
    }
  }
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::PoolLower::activate(ComputeGraph& pGraph, ::onnx::Node &pNode) const
{
  // check input/output name
  if (!pNode.inputs().empty())
    return nullptr;

  if (!pNode.outputs().empty())
    return nullptr;

  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("ifmap_laddr")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("ofmap_laddr")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("input_dim")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("output_dim")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("is_avg_pooling")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("op_name")))
    return nullptr;

  // create operators
  BM188X::Pool* op = pGraph.addOperator<onnc::BM188X::Pool>(
      pNode.i(::onnx::Symbol("ifmap_laddr")),
      pNode.i(::onnx::Symbol("ofmap_laddr")),
      pNode.is(::onnx::Symbol("input_dim")),
      pNode.is(::onnx::Symbol("output_dim")),
      pNode.i(::onnx::Symbol("is_avg_pooling")),
      pNode.s(::onnx::Symbol("op_name"))
  );

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("slice_pads")))
    op->setSlidePads(pNode.is(::onnx::Symbol("slice_pads")));
  else
    op->setSlidePads(IntsAttr(4, 0)); //< fill constructor {0, 0, 0, 0}

  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));
  else
    op->setStrides(IntsAttr(2, 1)); //< fill constructor {1, 1}

  return op;
}
