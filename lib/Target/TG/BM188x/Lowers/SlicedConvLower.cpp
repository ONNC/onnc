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

int BM188X::SlicedConvLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.hasAttribute(::onnx::Symbol("is_sliced"))) {
    auto is_sliced = pNode.i(::onnx::Symbol("is_sliced"));
    if (is_sliced) {
      // higher than BM188X::Conv
      return kTargetHigh;
    }
  }
  return kNotMe;
}

onnc::ComputeOperator *
BM188X::SlicedConvLower::activate(ComputeGraph& pGraph, ::onnx::Node &pNode) const
{
  // check input/output name
  if (!pNode.inputs().empty() ||
      !pNode.outputs().empty())
    return nullptr;

  // check required attributes
  if (!pNode.hasAttribute(::onnx::Symbol("ifmap_laddr"))  ||
      !pNode.hasAttribute(::onnx::Symbol("ofmap_laddr"))  ||
      !pNode.hasAttribute(::onnx::Symbol("weight_laddr")) ||
      !pNode.hasAttribute(::onnx::Symbol("result_add"))   ||
      !pNode.hasAttribute(::onnx::Symbol("input_dim"))    ||
      !pNode.hasAttribute(::onnx::Symbol("output_dim"))   ||
      !pNode.hasAttribute(::onnx::Symbol("op_name")))
    return nullptr;

  // create operators
  BM188X::SlicedConv* op = pGraph.addOperator<onnc::BM188X::SlicedConv>(
      pNode.i(::onnx::Symbol("ifmap_laddr")),
      pNode.i(::onnx::Symbol("ofmap_laddr")),
      pNode.i(::onnx::Symbol("weight_laddr")),
      pNode.i(::onnx::Symbol("result_add")),
      pNode.is(::onnx::Symbol("input_dim")),
      pNode.is(::onnx::Symbol("output_dim")),
      pNode.s(::onnx::Symbol("op_name"))
  );

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("bias_laddr")))
    op->setBias(pNode.i(::onnx::Symbol("bias_laddr")));

  auto &weightDim = pNode.is(::onnx::Symbol("weight_dim"));
  IntsAttr kShape(2, 0); //< fill constructor {0, 0}
  kShape.vector()[0] = weightDim[2];
  kShape.vector()[1] = weightDim[3];
  op->setKernelShape(kShape);

  if (pNode.hasAttribute(::onnx::Symbol("group")))
    op->setGroups(pNode.i(::onnx::Symbol("group")));

  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape")))
    op->setKernelShape(pNode.is(::onnx::Symbol("kernel_shape")));

  if (pNode.hasAttribute(::onnx::Symbol("slice_pads")))
    op->setSlidePads(pNode.is(::onnx::Symbol("slice_pads")));
  else
    op->setSlidePads(IntsAttr(4, 0)); //< fill constructor {0, 0, 0, 0}

  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));
  else
    op->setStrides(IntsAttr(2, 1)); //< fill constructor {1, 1}

  if (pNode.hasAttribute(::onnx::Symbol("do_relu")))
    op->setDoRelu();

  return op;
}