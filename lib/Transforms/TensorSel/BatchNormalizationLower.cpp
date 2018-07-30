//===- BatchNormalizationLower.cpp ----------------------------------------===//
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
  // check input/output name
  if (5 != pNode.inputs().size())
    return nullptr;
  for (::onnx::Value* xv : pNode.inputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

  if (6 > pNode.outputs().size() || pNode.outputs().empty()) //< [1,5]
    return nullptr;
  for (::onnx::Value* xv : pNode.outputs()) {
    if (!xv->has_unique_name())
      return nullptr;
  }

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
  onnc::Tensor* x = pGraph.getValue<onnc::Tensor>(
      pNode.inputs()[onnc::BatchNormalization::kX]->uniqueName());
  onnc::Tensor* scale = pGraph.getValue<onnc::Tensor>(
      pNode.inputs()[onnc::BatchNormalization::kScale]->uniqueName());
  onnc::Tensor* b = pGraph.getValue<onnc::Tensor>(
      pNode.inputs()[onnc::BatchNormalization::kB]->uniqueName());
  onnc::Tensor* in_mean = pGraph.getValue<onnc::Tensor>(
      pNode.inputs()[onnc::BatchNormalization::kInMean]->uniqueName());
  onnc::Tensor* in_var = pGraph.getValue<onnc::Tensor>(
      pNode.inputs()[onnc::BatchNormalization::kInVar]->uniqueName());

  op->setX(*x);
  op->setY(*scale);
  op->setB(*b);
  op->setInMean(*in_mean);
  op->setInVar(*in_var);

  onnc::Tensor* y = pGraph.getValue<onnc::Tensor>(
      pNode.outputs()[onnc::BatchNormalization::kY]->uniqueName());

  op->setY(*y);

  onnc::Tensor* out_mean = nullptr;
  if (pNode.outputs().size() >= BatchNormalization::kOutMean) {
    out_mean = pGraph.getValue<onnc::Tensor>(
        pNode.outputs()[onnc::BatchNormalization::kOutMean]->uniqueName());
    op->setOutMean(*out_mean);
  }

  onnc::Tensor* out_var = nullptr;
  if (pNode.outputs().size() >= BatchNormalization::kOutVar) {
    out_var = pGraph.getValue<onnc::Tensor>(
        pNode.outputs()[onnc::BatchNormalization::kOutVar]->uniqueName());
    op->setOutVar(*out_var);
  }

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
