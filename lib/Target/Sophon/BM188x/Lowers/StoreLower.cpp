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

int BM188X::StoreLower::isMe(const xNode &pNode) const
{
  if (pNode.kind() == xSymbol("TLStore"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::StoreLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check required attributes
  if (!pNode.hasAttribute(xSymbol("dst_goffset")))
    return nullptr;
  if (!pNode.hasAttribute(xSymbol("src_laddr")))
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
  BM188X::Store* op = pGraph.addOperator<onnc::BM188X::Store>(
      pNode.i(xSymbol("dst_goffset")),
      pNode.i(xSymbol("src_laddr")),
      pNode.i(xSymbol("do_transpose")),
      pNode.i(xSymbol("is_aligned")),
      pNode.i(xSymbol("is_neuron")),
      pNode.is(xSymbol("local_dim")),
      pNode.is(xSymbol("global_dim")),
      pNode.s(xSymbol("op_name")));

  // Comment copy from BM188xISelLowering.cpp:
  //   FIXME(arcbbb): It's a workaround.
  //   not to violate SSA, we add output value as input.
  xValue* v = pNode.inputs()[0];
  onnc::Tensor* tensor = pGraph.getValue<onnc::Tensor>(v->uniqueName());
  if (nullptr == tensor)
    tensor = IRBuilder::CreateComputeTensor(pGraph, *v);
  op->addInput(*tensor);

  return op;
}
