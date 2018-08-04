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
  if (1 > pNode.inputs().size())
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
  else
    op->setDilations(IntsAttr(2, 1)); // {1, 1}

  if (pNode.hasAttribute(::onnx::Symbol("group")))
    op->setGroup(pNode.i(::onnx::Symbol("group")));
  else
    op->setGroup(IntAttr(1)); // {1}

  if (pNode.hasAttribute(::onnx::Symbol("kernel_shape")))
    op->setKernelShape(pNode.is(::onnx::Symbol("kernel_shape")));
  if (pNode.hasAttribute(::onnx::Symbol("pads")))
    op->setPads(pNode.is(::onnx::Symbol("pads")));
  else
    op->setPads(IntsAttr(2, 0)); // {0, 0}

  if (pNode.hasAttribute(::onnx::Symbol("strides")))
    op->setStrides(pNode.is(::onnx::Symbol("strides")));
  else
    op->setStrides(IntsAttr(2, 1)); // {1, 1}

  op->setDoRelu(pm::match(&pNode, pm::mTrueAttr("do_relu")));

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

  unsigned inputCnt = 2; // input, weight.
  bool doScale = pm::match(&pNode, pm::mTrueAttr("do_scale"));
  bool doScaleBias = pm::match(&pNode, pm::mTrueAttr("do_scale_bias"));

  if (doScale) ++inputCnt;
  if (doScaleBias) ++inputCnt;

  bool doBias = false;
  if (pNode.inputs().size() != inputCnt) {
    assert((pNode.inputs().size() - inputCnt == 1) &&
           "[BM188X::ConvLower] unexpected input cnt.");
    doBias = true;
  }

  // get input tensor.
  onnc::Tensor *bias = nullptr, *scale = nullptr,
               *scaleBias = nullptr;
  int inputIdx = 2;
  if (doBias) bias = op->getInput(inputIdx++);
  if (doScale) scale = op->getInput(inputIdx++);
  if (doScaleBias) scaleBias = op->getInput(inputIdx++);

  op->setBias(bias);
  op->setScale(scale);
  op->setScaleBias(scaleBias);

  if (doScale)
    op->setConvOutputThreshold(pNode.f(onnx::Symbol("conv_output_threshold")));

  return op;
}
