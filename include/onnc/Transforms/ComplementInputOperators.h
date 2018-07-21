//===- ComplementInputOperators.h -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_COMPLEMENT_INPUT_OPERATORS_H
#define ONNC_TRANSFORM_COMPLEMENT_INPUT_OPERATORS_H
#include <onnc/Transforms/GraphBuildingPass.h>

namespace onnc {

/** \class ComplementInputOperators
 *  \brief ComplementInputOperators add InputOperator objects in ComputeGraph.
 */
class ComplementInputOperators : public GraphBuildingPass
{
public:
  static char ID;

public:
  ComplementInputOperators() : GraphBuildingPass(ID) { }

  ~ComplementInputOperators() { }

  Pass::ReturnType runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "ComplementInputOperators"; }
};

ModulePass *CreateComplementInputOperators();

} // namespace of onnc

#endif
