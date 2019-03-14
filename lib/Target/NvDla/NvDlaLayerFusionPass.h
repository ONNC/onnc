//===- NvDlaLayerFusionPass.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_FUSION_PASS_H
#define TARGET_NVDLA_NVDLA_FUSION_PASS_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

class Conv;
class Relu;
class Gemm;
class MaxPool;
class NvDlaConvRelu;
class NvDlaGemmRelu;
class NvDlaConvReluMaxPool;
class NvDlaMaxPool;

/** \class NvDlaLayerFusionPass
 *  \brief Fuse conv and relu to NvDlaConvRelu operator.
 */
class NvDlaLayerFusionPass : public CustomPass<NvDlaLayerFusionPass>
{
public:
  NvDlaLayerFusionPass() = default;

  ReturnType runOnModule(Module& pModule) override;

private:
  ReturnType runOnComputeGraph(ComputeGraph& pCG);

  /// Can we merge pConv and Relu which uses pConv as input.
  bool isFusibleForConvRelu(ComputeOperator& pNode);
  bool isFusibleForGemmRelu(ComputeOperator& pNode);
  bool isFusibleForConvReluMaxPool(ComputeOperator& pNode);

  /// Detach pConv and pRelu from Graph, replace it with new ConvRelu IR.
  ///
  /// original: .. -> pConv -> pRelu -> ..
  /// merged:   .. -> ConvRelu -> ..
  NvDlaConvRelu*        mergeConvRelu(ComputeGraph& pCG, Conv& pConv, Relu& pRelu);
  NvDlaGemmRelu*        mergeGemmRelu(ComputeGraph& pCG, Gemm& pGemm, Relu& pRelu);
  NvDlaConvReluMaxPool* mergeConvReluMaxPool(ComputeGraph& pCG, Conv& pConv, Relu& pRelu, MaxPool& pMaxPool);
  NvDlaMaxPool*         replaceMaxPool(ComputeGraph& pCG, MaxPool& pMaxPool);
};

} // namespace onnc

#endif
