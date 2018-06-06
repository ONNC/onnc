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
Module::Module()
    : m_OnnxIRVersion(0), m_OnnxProducerName(""), m_OnnxProducerVersion(""),
      m_OnnxDomain(""), m_OnnxModelVersion(0), m_OnnxDocString(""),
      m_pOnnxGraph(nullptr)
{
}

Module::~Module()
{
  m_pOnnxGraph.reset();
}

Module &Module::delegateGraph(std::unique_ptr< ::onnx::Graph> pGraph)
{
  m_pOnnxGraph = std::move(pGraph);
  return *this;
}
