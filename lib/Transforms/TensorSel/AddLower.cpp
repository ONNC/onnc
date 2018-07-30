//===- AddLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/AddLower.h>
#include <onnc/IR/Compute/Add.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AddLower
//===----------------------------------------------------------------------===//
AddLower::AddLower()
{
}

AddLower::~AddLower()
{
}

int AddLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Add"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
AddLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
{
  // check input/output name
  if (2 != pNode.inputs().size())
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
  onnc::Add* op = pGraph.addOperator<onnc::Add>();

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
