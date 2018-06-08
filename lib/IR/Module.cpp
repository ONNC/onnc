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
  : m_pOnnxGraph(nullptr), m_OnnxInfo(), m_OnnxSetId(), m_OnnxMetaData(),
    m_ComputeGraph() {
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
