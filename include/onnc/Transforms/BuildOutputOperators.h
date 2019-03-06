//===- BuildOutputOperators.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_BUILD_OUTPUT_OPERATORS_H
#define ONNC_TRANSFORM_BUILD_OUTPUT_OPERATORS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/Transforms/GraphBuildingPass.h>

namespace onnc {

/** \class BuildOutputOperators
 *  \brief BuildOutputOperators add OutputOperator objects in ComputeGraph.
 */
class BuildOutputOperators : public CustomPass<BuildOutputOperators, GraphBuildingPass>
{
public:
  BuildOutputOperators() = default;

  ~BuildOutputOperators() = default;

  Pass::ReturnType runOnGraphs(xGraph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "BuildOutputOperators"; }
};

ModulePass *CreateBuildOutputOperators();

} // namespace of onnc

#endif
