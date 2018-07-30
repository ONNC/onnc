//===- XorLower.cpp -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Transforms/TensorSel/Lower.h>
#include <onnc/Transforms/TensorSel/Standards/XorLower.h>
#include <onnc/IR/Compute/Xor.h>
#include <onnc/IR/IRBuilder.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// XorLower
//===----------------------------------------------------------------------===//
XorLower::XorLower()
{
}

XorLower::~XorLower()
{
}

int XorLower::isMe(const ::onnx::Node& pNode) const
{
  if (pNode.kind() == ::onnx::Symbol("Xor"))
    return kStdLower;
  return kNotMe;
}

ComputeOperator*
XorLower::activate(ComputeGraph& pGraph, ::onnx::Node& pNode) const
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
  onnc::Xor* op = pGraph.addOperator<onnc::Xor>();

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
