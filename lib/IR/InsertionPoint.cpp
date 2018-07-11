//===- InsertionPoint.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/IR/InsertionPoint.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// InsertionPoint
//===----------------------------------------------------------------------===//
InsertionPoint::InsertionPoint()
  : m_pModule(nullptr),
    m_pTensorGraph(nullptr),
    m_pTensorNode(nullptr),
    m_pComputeGraph(nullptr),
    m_pComputeNode(nullptr) {
}

InsertionPoint::InsertionPoint(const InsertionPoint& pCopy)
  : m_pModule(pCopy.m_pModule),
    m_pTensorGraph(pCopy.m_pTensorGraph),
    m_pTensorNode(pCopy.m_pTensorNode),
    m_pComputeGraph(pCopy.m_pComputeGraph),
    m_pComputeNode(pCopy.m_pComputeNode) {
}

InsertionPoint& InsertionPoint::operator=(const InsertionPoint& pCopy)
{
  m_pModule = pCopy.m_pModule;
  m_pTensorGraph = pCopy.m_pTensorGraph;
  m_pTensorNode = pCopy.m_pTensorNode;
  m_pComputeGraph = pCopy.m_pComputeGraph;
  m_pComputeNode = pCopy.m_pComputeNode;
  return *this;
}

InsertionPoint& InsertionPoint::setTensorGraph(::onnx::Graph& pGraph)
{
  m_pTensorGraph = &pGraph;
  return *this;
}
