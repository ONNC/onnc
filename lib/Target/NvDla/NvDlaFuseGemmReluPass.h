//===- NvDlaFuseGemmReluPass.h --------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_FUSE_GEMM_RELU_PASS_H
#define TARGET_NVDLA_NVDLA_FUSE_GEMM_RELU_PASS_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

class Gemm;
class Relu;
class NvDlaGemmRelu;

/** \class NvDlaFuseGemmReluPass
 *  \brief Fuse conv and relu to NvDlaConvRelu operator.
 */
class NvDlaFuseGemmReluPass : public CustomPass<NvDlaFuseGemmReluPass>
{
public:
  NvDlaFuseGemmReluPass() = default;

  ReturnType runOnModule(Module& pModule) override;

private:
  ReturnType runOnComputeGraph(ComputeGraph& pCG);

  /// Can we merge pConv and Relu which uses pConv as input.
  bool isFusible(ComputeOperator& pNode);

  /// Detach pGemm and pRelu from Graph, replace it with new GemmRelu IR.
  ///
  /// original: .. -> pGemm -> pRelu -> ..
  /// merged:   .. -> GemmRelu -> ..
  NvDlaGemmRelu* mergeGemmRelu(ComputeGraph& pCG, Gemm& pGemm, Relu& pRelu);
};

} // namespace of onnc

#endif
