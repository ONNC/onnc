//===- InsertPoint.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_INSERTION_POINT_H
#define ONNC_IR_INSERTION_POINT_H
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeGraph.h>
#include <onnc/ADT/StringMap.h>
#include <onnx/common/ir_pb_converter.h>
#include <onnx/common/ir.h>

namespace onnc {

/** \class InsertionPoint
 */
class InsertionPoint
{
public:
  typedef StringMap<::onnx::Value*> CreatedValues;

public:
  InsertionPoint();

  ~InsertionPoint() { }

  InsertionPoint& operator=(const InsertionPoint& pISP);

  InsertionPoint(const InsertionPoint& pCopy);

  InsertionPoint& setTensorGraph(::onnx::Graph& pGraph);

  /// Get the target module
  Module* getModule() { return m_pModule; }

  /// Get the target module
  const Module* getModule() const { return m_pModule; }

  void setModule(Module& pModule) { m_pModule = &pModule; }

  bool hasModule() const { return (nullptr != m_pModule); }

  /// get current insertion point of tensor graph.
  /// @retval nullptr not set
  ::onnx::Graph* getTensorGraph() { return m_pTensorGraph; }

  /// get current insertion point of tensor graph.
  /// @retval nullptr not set
  const ::onnx::Graph* getTensorGraph() const { return m_pTensorGraph; }

  bool hasTensorGraph() const { return (nullptr != m_pTensorGraph); }

  ::onnx::Node* getTensorNode() { return m_pTensorNode; }

  const ::onnx::Node* getTensorNode() const { return m_pTensorNode; }

  void setTensorNode(::onnx::Node& pNode) { m_pTensorNode = &pNode; }

  bool hasTensorNode() const { return (nullptr != m_pTensorNode); }

  /// get current insertion point of compute graph
  /// @retval nullptr not set.
  ComputeGraph* getComputeGraph() { return m_pComputeGraph; }

  const ComputeGraph* getComputeGraph() const { return m_pComputeGraph; }

  bool hasComputeGraph() const { return (nullptr != m_pComputeGraph); }

  /// change the insertion point to @ref pCG
  void setComputeGraph(ComputeGraph* pCG) { m_pComputeGraph = pCG; }

  ComputeOperator* getComputeOperator() { return m_pComputeNode; }

  const ComputeOperator* getComputeOperator() const { return m_pComputeNode; }

  void setComputeOperator(ComputeOperator& pOp) { m_pComputeNode = &pOp; }

  bool hasComputeOperator() const { return (nullptr != m_pComputeNode); }

  CreatedValues& getCreatedValues() { return m_CreatedValues; }

  const CreatedValues& getCreatedValues() const { return m_CreatedValues; }

private:
  Module* m_pModule;

  /// current target tensor graph
  ::onnx::Graph* m_pTensorGraph;

  /// current target tensor graph node
  ::onnx::Node* m_pTensorNode;

  /// current target compute graph
  ComputeGraph* m_pComputeGraph;

  /// current target compute operator
  ComputeOperator* m_pComputeNode;

  /// all ::onnx::Value created
  CreatedValues m_CreatedValues;
};

} // namespace onnc

#endif
