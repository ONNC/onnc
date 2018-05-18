//===- Module.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnx/common/ir.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Module
//===----------------------------------------------------------------------===//
Module::Module() : m_OnnxGraph(nullptr) {}

Module::~Module() {}

Module &Module::delegateGraph(std::unique_ptr<::onnx::Graph> pGraph)
{
  m_OnnxGraph = std::move(pGraph);
  return *this;
}
