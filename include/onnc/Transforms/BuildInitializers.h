//===- BuildInitializers.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_BUILD_INITIALIZERS_H
#define ONNC_TRANSFORM_BUILD_INITIALIZERS_H
#include <onnc/Transforms/GraphBuildingPass.h>

namespace onnc {

/** \class BuildInitializers
 *  \brief BuildInitializers creates ComputeGraph objects and converts ONNX's
 *  initializers to ComputeOperators
 */
class BuildInitializers : public GraphBuildingPass
{
public:
  static char ID;

public:
  BuildInitializers() : GraphBuildingPass(ID) { }

  ~BuildInitializers() { }

  Pass::ReturnType runOnGraphs(xGraph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "BuildInitializers"; }
};

ModulePass *CreateBuildInitializers();

} // namespace of onnc

#endif
