//===- BuildInputOperators.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_BUILD_INPUT_OPERATORS_H
#define ONNC_TRANSFORM_BUILD_INPUT_OPERATORS_H
#include <onnc/Transforms/GraphBuildingPass.h>

namespace onnc {

/** \class BuildInputOperators
 *  \brief BuildInputOperators add InputOperator objects in ComputeGraph.
 */
class BuildInputOperators : public GraphBuildingPass
{
public:
  static char ID;

public:
  BuildInputOperators() : GraphBuildingPass(ID) { }

  ~BuildInputOperators() { }

  Pass::ReturnType runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "BuildInputOperators"; }
};

ModulePass *CreateBuildInputOperators();

} // namespace of onnc

#endif
