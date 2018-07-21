//===- ComplementInitializers.h -------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORM_COMPLEMENT_INITIALIZERS_H
#define ONNC_TRANSFORM_COMPLEMENT_INITIALIZERS_H
#include <onnc/Transforms/GraphBuildingPass.h>

namespace onnc {

/** \class ComplementInitializers
 *  \brief ComplementInitializers creates ComputeGraph objects and converts ONNX's
 *  initializers to ComputeOperators
 */
class ComplementInitializers : public GraphBuildingPass
{
public:
  static char ID;

public:
  ComplementInitializers() : GraphBuildingPass(ID) { }

  ~ComplementInitializers() { }

  Pass::ReturnType runOnGraphs(::onnx::Graph& pTG, ComputeGraph& pCG) override;

  StringRef getPassName() const override { return "ComplementInitializers"; }
};

ModulePass *CreateComplementInitializers();

} // namespace of onnc

#endif
