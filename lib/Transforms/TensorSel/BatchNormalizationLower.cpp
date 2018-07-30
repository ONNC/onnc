//===- BatchNormalizationLower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/BatchNormalizationLower.h>
#include <onnc/IR/Compute/BatchNormalization.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// BatchNormalizationLower
//===----------------------------------------------------------------------===//
BatchNormalizationLower::BatchNormalizationLower()
{
}

BatchNormalizationLower::~BatchNormalizationLower()
{
}

int BatchNormalizationLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("BatchNormalization"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
BatchNormalizationLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (pNode.inputs().size() != 5)
    return nullptr;

  if (pNode.outputs().size() < 1 || 5 < pNode.outputs().size())
    return nullptr;

  // check input/output name
  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  // check default attributes
  

  // create operators
  onnc::BatchNormalization* op = pGraph.addOperator<onnc::BatchNormalization>();

  // set optional attributes
  if (pNode.hasAttribute(::onnx::Symbol("epsilon")))
    op->setEpsilon(pNode.f(::onnx::Symbol("epsilon")));
  if (pNode.hasAttribute(::onnx::Symbol("momentum")))
    op->setMomentum(pNode.f(::onnx::Symbol("momentum")));
  if (pNode.hasAttribute(::onnx::Symbol("spatial")))
    op->setSpatial(pNode.i(::onnx::Symbol("spatial")));

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
  return op;
}
