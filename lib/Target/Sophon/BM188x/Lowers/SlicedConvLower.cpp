//===- SlicedConvLower.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "SlicedConvLower.h"
#include "../Compute/SlicedConv.h"
#include <onnc/IR/IRBuilder.h>
#include <onnc/IR/Compute/Attributes.h>

using namespace onnc;
using namespace onnc::BM188X;

//===----------------------------------------------------------------------===//
// SlicedConvLower
//===----------------------------------------------------------------------===//
BM188X::SlicedConvLower::SlicedConvLower()
{
}

BM188X::SlicedConvLower::~SlicedConvLower()
{
}

int BM188X::SlicedConvLower::isMe(const xNode &pNode) const
{
  if (pNode.hasAttribute(xSymbol("is_sliced"))) {
    auto is_sliced = pNode.i(xSymbol("is_sliced"));
    if (is_sliced) {
      // higher than BM188X::Conv
      return kTargetHigh;
    }
  }
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::SlicedConvLower::activate(ComputeGraph& pGraph, xNode &pNode) const
{
  // check input/output name
  if (!pNode.inputs().empty() ||
      !pNode.outputs().empty())
    return nullptr;

  // check required attributes
  if (!pNode.hasAttribute(xSymbol("ifmap_laddr"))  ||
      !pNode.hasAttribute(xSymbol("ofmap_laddr"))  ||
      !pNode.hasAttribute(xSymbol("weight_laddr")) ||
      !pNode.hasAttribute(xSymbol("result_add"))   ||
      !pNode.hasAttribute(xSymbol("input_dim"))    ||
      !pNode.hasAttribute(xSymbol("output_dim"))   ||
      !pNode.hasAttribute(xSymbol("op_name")))
    return nullptr;

  // create operators
  BM188X::SlicedConv* op = pGraph.addOperator<onnc::BM188X::SlicedConv>(
      pNode.i(xSymbol("ifmap_laddr")),
      pNode.i(xSymbol("ofmap_laddr")),
      pNode.i(xSymbol("weight_laddr")),
      pNode.i(xSymbol("result_add")),
      pNode.is(xSymbol("input_dim")),
      pNode.is(xSymbol("output_dim")),
      pNode.s(xSymbol("op_name"))
  );

  // set optional attributes
  if (pNode.hasAttribute(xSymbol("bias_laddr")))
    op->setBias(pNode.i(xSymbol("bias_laddr")));

  auto &weightDim = pNode.is(xSymbol("weight_dim"));
  IntsAttr kShape(2, 0); //< fill constructor {0, 0}
  kShape.vector()[0] = weightDim[2];
  kShape.vector()[1] = weightDim[3];
  op->setKernelShape(kShape);

  if (pNode.hasAttribute(xSymbol("group")))
    op->setGroups(pNode.i(xSymbol("group")));

  if (pNode.hasAttribute(xSymbol("kernel_shape")))
    op->setKernelShape(pNode.is(xSymbol("kernel_shape")));

  if (pNode.hasAttribute(xSymbol("slice_pads")))
    op->setSlidePads(pNode.is(xSymbol("slice_pads")));
  else
    op->setSlidePads(IntsAttr(4, 0)); //< fill constructor {0, 0, 0, 0}

  if (pNode.hasAttribute(xSymbol("strides")))
    op->setStrides(pNode.is(xSymbol("strides")));
  else
    op->setStrides(IntsAttr(2, 1)); //< fill constructor {1, 1}

  if (pNode.hasAttribute(xSymbol("do_relu")))
    op->setDoRelu();

  return op;
}
