//===- X86FuseConvRelu.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_X86_X86_FUSE_CONV_RELU_H
#define TARGET_X86_X86_FUSE_CONV_RELU_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

class Conv;
class Relu;
class X86ConvRelu;

/** \class X86FuseConvRelu
 *  \brief Fuse conv and relu to X86ConvRelu operator.
 */
class X86FuseConvRelu : public CustomPass<X86FuseConvRelu>
{
public:
  X86FuseConvRelu() = default;

  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  /// Can we merge pConv and Relu which uses pConv as input.
  bool isFusible(ComputeOperator& pNode);

  /// Detach pConv and pRelu from Graph, replace it with new X86ConvRelu IR.
  ///
  /// original: .. -> pConv -> pRelu -> ..
  /// merged:   .. -> X86ConvRelu -> ..
  X86ConvRelu* mergeConvRelu(ComputeGraph& pCG, Conv& pConv, Relu& pRelu);
};

} // namespace of onnc

#endif
