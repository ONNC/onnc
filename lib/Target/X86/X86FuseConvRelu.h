//===- X86FuseConvRelu.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_X86_X86_FUSE_CONV_RELU_H
#define TARGET_X86_X86_FUSE_CONV_RELU_H
#include <onnc/Core/ModulePass.h>

namespace onnc {

class Conv;
class Relu;
class X86ConvRelu;

/** \class X86FuseConvRelu
 *  \brief Fuse conv and relu to X86ConvRelu operator.
 */
class X86FuseConvRelu : public ModulePass
{
public:
  static char ID;

public:
  X86FuseConvRelu()
    : ModulePass(ID) {
  }

  ReturnType runOnModule(Module& pModule) override;

private:
  ReturnType runOnComputeGraph(ComputeGraph& pCG);

  /// Can we merge pConv and Relu which uses pConv as input.
  bool isFusible(ComputeOperator& pNode);

  /// Detach pConv and pRelu from Graph, replace it with new X86ConvRelu IR.
  ///
  /// original: .. -> pConv -> pRelu -> ..
  /// merged:   .. -> X86ConvRelu -> ..
  X86ConvRelu* mergeConvRelu(ComputeGraph& pCG, Conv& pConv, Relu& pRelu);
};

X86FuseConvRelu* CreateX86FuseConvReluPass();

} // namespace of onnc

#endif
