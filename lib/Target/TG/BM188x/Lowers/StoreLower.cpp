//===- StoreLower.cpp -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "StoreLower.h"
#include "../Compute/Store.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// StoreLower
//===----------------------------------------------------------------------===//
BM188X::StoreLower::StoreLower()
{
}

BM188X::StoreLower::~StoreLower()
{
}

int BM188X::StoreLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("TLStore"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::StoreLower::activate(ComputeGraph& pGraph, ::onnx::Node &pNode) const
{
  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("dst_goffset")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("src_laddr")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("local_dim")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("global_dim")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("do_transpose")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("is_aligned")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("is_neuron")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("op_name")))
    return nullptr;

  // create operators
  BM188X::Store* op = pGraph.addOperator<onnc::BM188X::Store>(
      pNode.i(::onnx::Symbol("dst_goffset")),
      pNode.i(::onnx::Symbol("src_laddr")),
      pNode.i(::onnx::Symbol("do_transpose")),
      pNode.i(::onnx::Symbol("is_aligned")),
      pNode.i(::onnx::Symbol("is_neuron")),
      pNode.is(::onnx::Symbol("local_dim")),
      pNode.is(::onnx::Symbol("global_dim")),
      pNode.s(::onnx::Symbol("op_name")));

  return op;
}
