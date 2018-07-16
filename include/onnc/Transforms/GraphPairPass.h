//===- GraphPairPass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_GRAPH_PAIR_H
#define ONNC_TRANSFORM_GRAPH_PAIR_H
#include <onnc/Core/ModulePass.h>
#include <onnc/IR/ComputeGraph.h>
#include <onnx/common/ir.h>

namespace onnc {

/** \class GraphPairPass
 *  \brief GraphPairPass converts ONNX node to ComputeOperator and creates
 *  ComputeGraph objects for subgraph in ONNX.
 */
class GraphPairPass : public ModulePass
{
public:
  GraphPairPass(char& pPassID);

  virtual ~GraphPairPass() { }

  Pass::ReturnType runOnModule(::onnc::Module &pModule) override;

  virtual Pass::ReturnType runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG) = 0;
};

} // namespace of onnc

#endif
