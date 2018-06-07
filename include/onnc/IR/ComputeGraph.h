//===- ComputeGraph.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_IR_COMPUTE_GRAPH_H
#define ONNC_IR_COMPUTE_GRAPH_H
#include <onnc/ADT/Digraph.h>
#include <onnc/Support/MemoryPool.h>
#include <onnc/IR/ComputeOperator.h>
#include <onnc/IR/ComputeOperand.h>
#include <onnc/IR/ComputeAttrOperand.h>
#include <onnc/IR/ComputeMemOperand.h>

namespace onnc {

/** \class ComputeGraph
 */
class ComputeGraph : public Digraph<ComputeOperator, ComputeOperand>
{
public:
  ComputeGraph();

  virtual ~ComputeGraph();

private:
  typedef MemoryPool<ComputeOperator>    OperatorFactory;
  typedef MemoryPool<ComputeOperand>     OperandFactory;
  typedef MemoryPool<ComputeAttrOperand> AttrOperandFactory;
  typedef MemoryPool<ComputeMemOperand>  MemOperandFactory;

private:
  OperatorFactory    m_OperatorFactory;
  OperandFactory     m_OperandFactory;
  AttrOperandFactory m_AttrOperandFactory;
  MemOperandFactory  m_MemOperandFactory;
};

} // namespace of onnc

#endif
