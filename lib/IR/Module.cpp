//===- Module.cpp ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/Module.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnx/common/ir.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Module
//===----------------------------------------------------------------------===//
Module::Module()
  : m_pOnnxGraph(),
    m_OnnxInfo(),
    m_OnnxSetId(),
    m_OnnxMetaData(),
    m_ComputeGraph() {
}

Module::Module(std::unique_ptr< ::onnx::Graph> pGraph)
  : m_pOnnxGraph(std::move(pGraph)),
    m_OnnxInfo(),
    m_OnnxSetId(),
    m_OnnxMetaData(),
    m_ComputeGraph() {
}

Module::~Module()
{
  if (1 < m_pOnnxGraph.use_count()) {
    // display error because Module should response for the life cycle of IR.
    error(onnx_graph_alive);
  }
  m_pOnnxGraph.reset();
}

Module& Module::delegate(std::unique_ptr< ::onnx::Graph> pGraph)
{
  m_pOnnxGraph = std::move(pGraph);
  return *this;
}

void Module::print(std::ostream& pOS) const
{
  // TODO
}

//===----------------------------------------------------------------------===//
// Module::OnnxInfo
//===----------------------------------------------------------------------===//
Module::OnnxInfo::OnnxInfo()
  : m_IRVersion(0),
    m_ProducerName(),
    m_ProducerVersion(),
    m_Domain(),
    m_ModelVersion(0),
    m_DocString() {
}

void Module::OnnxInfo::setProducer(const std::string& pName,
                                   const std::string& pVersion)
{
  m_ProducerName = pName;
  m_ProducerVersion = pVersion;
}

void Module::OnnxInfo::print(std::ostream& pOS) const
{
  // TODO
}
