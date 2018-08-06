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

int BM188X::LoadLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("TLLoad"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::LoadLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check required attributes
  if (!pNode.hasAttribute(xSymbol("src_goffset")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("dst_laddr")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("local_dim")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("global_dim")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("do_transpose")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("is_aligned")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("is_neuron")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("op_name")))
    return nullptr;

  // create operators
  onnc::BM188X::Load* op = pGraph.addOperator<onnc::BM188X::Load>(
      pNode.i(xSymbol("src_goffset")),
      pNode.i(xSymbol("dst_laddr")),
      pNode.i(xSymbol("do_transpose")),
      pNode.i(xSymbol("is_aligned")),
      pNode.i(xSymbol("is_neuron")),
      pNode.is(xSymbol("local_dim")),
      pNode.is(xSymbol("global_dim")),
      pNode.s(xSymbol("op_name")));

  // set input
  xValue* v = pNode.inputs()[0];
  onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(v->uniqueName());
  if (nullptr == tensor)
    tensor = IRBuilder::CreateComputeTensor(pGraph, *v);
  op->addInput(*tensor);

  return op;
}
