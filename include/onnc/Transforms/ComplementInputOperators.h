//===- ComplementInputOperators.h -----------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_COMPLEMENT_INPUT_OPERATORS_H
#define ONNC_TRANSFORM_COMPLEMENT_INPUT_OPERATORS_H
#include <onnc/Transforms/GraphPairPass.h>

namespace onnc {

/** \class ComplementInputOperators
 *  \brief ComplementInputOperators add InputOperator objects in ComputeGraph.
 */
class ComplementInputOperators : public GraphPairPass
{
public:
  static char ID;

public:
  ComplementInputOperators() : GraphPairPass(ID) { }

  ~ComplementInputOperators() { }

  Pass::ReturnType runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "ComplementInputOperators"; }
};

ModulePass *CreateComplementInputOperators();

} // namespace of onnc

#endif
