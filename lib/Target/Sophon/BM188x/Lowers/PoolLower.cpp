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

int BM188X::PoolLower::isMe(const xNode &pNode) const
{
  if (pNode.hasAttribute(xSymbol("is_sliced"))) {
    auto is_sliced = pNode.i(xSymbol("is_sliced"));
    if (is_sliced) {
      // higher than BM188X::MaxPool and BM188X::AveragePool.
      return kTargetHigh;
    }
  }
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::PoolLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check input/output name
  if (!pNode.inputs().empty())
    return nullptr;

  if (!pNode.outputs().empty())
    return nullptr;

  // check required attributes
  if (!pNode.hasAttribute(xSymbol("ifmap_laddr")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("ofmap_laddr")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("input_dim")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("output_dim")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("is_avg_pooling")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("op_name")))
    return nullptr;

  // create operators
  BM188X::Pool* op = pGraph.addOperator<onnc::BM188X::Pool>(
      pNode.i(xSymbol("ifmap_laddr")),
      pNode.i(xSymbol("ofmap_laddr")),
      pNode.is(xSymbol("input_dim")),
      pNode.is(xSymbol("output_dim")),
      pNode.i(xSymbol("is_avg_pooling")),
      pNode.s(xSymbol("op_name"))
  );

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("slice_pads")))
    op->setSlidePads(pNode.is(xSymbol("slice_pads")));
  else
    op->setSlidePads(IntsAttr(4, 0)); //< fill constructor {0, 0, 0, 0}

  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));
  else
    op->setStrides(IntsAttr(2, 1)); //< fill constructor {1, 1}

  return op;
}
