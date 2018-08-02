//===- ConvLower.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "ConvLower.h"
#include "PatternMatch.h"
#include "../Compute/Conv.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;
using namespace onnc::BM188X;

namespace pm = onnc::PatternMatch;

//===----------------------------------------------------------------------===//
// ConvLower
//===----------------------------------------------------------------------===//
BM188X::ConvLower::ConvLower()
{
}

BM188X::ConvLower::~ConvLower()
{
}

int BM188X::ConvLower::isMe(const ::onnx::Node &pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Conv"))
    return kTargetNormal;
  return kNotMe;
}

onnc::ComputeOperator *BM188X::ConvLower::activate(ComputeGraph& pGraph,
                                                   ::onnx::Node &pNode) const
{
  // check input/output name
  if (1 > pNode.inputs().size() || pNode.inputs().size() > 4)
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
  BM188X::Conv* op = pGraph.addOperator<onnc::BM188X::Conv>();


  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("auto_pad")))
    op->setAutoPad(pNode.s(::onnx::Symbol("auto_pad")));

  if (pNode.hasAttribute(::onnx::Symbol("dilations")))
    op->setDilations(pNode.is(::onnx::Symbol("dilations")));

  if (pNode.hasAttribute(::onnx::Symbol("group")))
    op->setGroup(pNode.i(::onnx::Symbol("group")));
  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape")))
    op->setKernelShape(pNode.is(::onnx::Symbol("kernel_shape")));
  if (pNode.hasAttribute(::onnx::Symbol("pads")))
    op->setPads(pNode.is(::onnx::Symbol("pads")));
  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));

  // set input/output
  for (::onnx::Value* xv : pNode.inputs())
    op->addInput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  for (::onnx::Value* xv : pNode.outputs())
    op->addOutput(*pGraph.getValue<onnc::Tensor>(xv->uniqueName()));

  if (pm::match(&pNode, pm::mTrueAttr("do_scale")))
    op->setDoScale(1);

  if (pm::match(&pNode, pm::mTrueAttr("do_scale_bias")))
    op->setDoScaleBias(1);

  int idx = 3;
  if (pNode.inputs().size() - op->getDoScale() - op->getDoScaleBias() == 3) {
    op->setDoBias(1);
    op->setBiasIdx(idx++);
  }

  if (op->isDoScale()) {
    op->setScaleIdx(idx++);
    op->setConvOutputThreshold(pNode.f(onnx::Symbol("conv_output_threshold")));
  }

  if (op->isDoScaleBias())
    op->setScaleBiasIdx(idx++);

  return op;
}
