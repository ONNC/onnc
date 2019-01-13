//===- BuildInputOperators.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_BUILD_INPUT_OPERATORS_H
#define ONNC_TRANSFORM_BUILD_INPUT_OPERATORS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Transforms/GraphBuildingPass.h>

namespace onnc {

/** \class BuildInputOperators
 *  \brief BuildInputOperators add InputOperator objects in ComputeGraph.
 */
class BuildInputOperators : public CustomPass<BuildInputOperators, GraphBuildingPass>
{
public:
  BuildInputOperators() = default;

  ~BuildInputOperators() = default;

  Pass::ReturnType runOnGraphs(xGraph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "BuildInputOperators"; }
};

ModulePass *CreateBuildInputOperators();

} // namespace of onnc

#endif
