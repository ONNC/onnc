//===- EliminateIdentity.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_ELIMINATE_IDENTITY
#define ONNC_ELIMINATE_IDENTITY
#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class EliminateIdentity
 *  \brief Remove identity nodes in the graph
 */
class EliminateIdentity: public CustomPass<EliminateIdentity>
{
public:
  EliminateIdentity() = default;

  ReturnType runOnModule(Module& pModule) override;

private:
  ReturnType runOnComputeGraph(ComputeGraph& pCG);
};

} // namespace of onnc

#endif // ONNC_ELIMINATE_IDENTITY
