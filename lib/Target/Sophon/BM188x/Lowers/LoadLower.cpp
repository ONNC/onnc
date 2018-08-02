//===- LoadLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "LoadLower.h"
#include "../Compute/Load.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// LoadLower
//===----------------------------------------------------------------------===//
BM188X::LoadLower::LoadLower()
{
}

BM188X::LoadLower::~LoadLower()
{
}

int BM188X::LoadLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("TLLoad"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::LoadLower::activate(ComputeGraph& pGraph, ::onnx::Node &pNode) const
{
  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("src_goffset")))
    return nullptr;
  if (!pNode.hasAttribute(::onnx::Symbol("dst_laddr")))
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
  onnc::BM188X::Load* op = pGraph.addOperator<onnc::BM188X::Load>(
      pNode.i(::onnx::Symbol("src_goffset")),
      pNode.i(::onnx::Symbol("dst_laddr")),
      pNode.i(::onnx::Symbol("do_transpose")),
      pNode.i(::onnx::Symbol("is_aligned")),
      pNode.i(::onnx::Symbol("is_neuron")),
      pNode.is(::onnx::Symbol("local_dim")),
      pNode.is(::onnx::Symbol("global_dim")),
      pNode.s(::onnx::Symbol("op_name")));

  // set input
  ::onnx::Value* v = pNode.inputs()[0];
  onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(v->uniqueName());
  if (nullptr == tensor)
    tensor = IRBuilder::CreateComputeTensor(pGraph, *v);
  op->addInput(*tensor);

  return op;
}
