//===- PropagateConstWithDiffShape.h --------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_PROPAGATE_CONST_WITH_DIFFSHAPE
#define ONNC_PROPAGATE_CONST_WITH_DIFFSHAPE
#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class PropagateConstWithDiffShape
 *  \brief Remove all shaping Nodes after Initializer
 */
class PropagateConstWithDiffShape: public CustomPass<PropagateConstWithDiffShape>
{
public:
  PropagateConstWithDiffShape() = default;

  ReturnType runOnModule(Module& pModule) override;

  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;
};

} // namespace of onnc

#endif // ONNC_PROPAGATE_CONST_WITH_DIFFSHAPE
