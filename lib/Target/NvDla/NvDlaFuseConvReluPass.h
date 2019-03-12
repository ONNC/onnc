//===- NvDlaFuseConvReluPass.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_FUSE_CONV_RELU_PASS_H
#define TARGET_NVDLA_NVDLA_FUSE_CONV_RELU_PASS_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

class Conv;
class Relu;
class NvDlaConvRelu;

/** \class NvDlaFuseConvReluPass
 *  \brief Fuse conv and relu to NvDlaConvRelu operator.
 */
class NvDlaFuseConvReluPass : public CustomPass<NvDlaFuseConvReluPass>
{
public:
  NvDlaFuseConvReluPass() = default;

  ReturnType runOnModule(Module& pModule) override;

private:
  ReturnType runOnComputeGraph(ComputeGraph& pCG);

  /// Can we merge pConv and Relu which uses pConv as input.
  bool isFusible(ComputeOperator& pNode);

  /// Detach pConv and pRelu from Graph, replace it with new ConvRelu IR.
  ///
  /// original: .. -> pConv -> pRelu -> ..
  /// merged:   .. -> ConvRelu -> ..
  NvDlaConvRelu* mergeConvRelu(ComputeGraph& pCG, Conv& pConv, Relu& pRelu);
};

} // namespace onnc

#endif
