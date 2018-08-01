//===- ${OperatorName}Lower.cpp -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/${OperatorName}Lower.h>
#include <onnc/IR/Compute/${OperatorName}.h>
#include "DefaultAttributes.h"
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// ${OperatorName}Lower
//===----------------------------------------------------------------------===//
${OperatorName}Lower::${OperatorName}Lower()
{
}

${OperatorName}Lower::~${OperatorName}Lower()
{
}

int ${OperatorName}Lower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("${OperatorName}"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
${OperatorName}Lower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output number
  if (${CheckInputNum})
    return nullptr;

  if (${CheckOutputNum})
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
  ${CheckDefaultAttributes}

  // create operators
  onnc::${OperatorName}* op = pGraph.addOperator<onnc::${OperatorName}>(${CreateOperator});

  // set default attributes
  SetDefaultAttributes(pNode, *op);

  // set optional attributes
  ${SetOptionalAttributes}

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
