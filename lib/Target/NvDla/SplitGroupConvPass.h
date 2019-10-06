//===- SplitGroupConvPass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_OPTIMIZATIONS_SPLIT_GROUP_CONV_PASS_H
#define ONNC_TRANSFORMS_OPTIMIZATIONS_SPLIT_GROUP_CONV_PASS_H
#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Conv.h>

#include <utility>

namespace onnc {

/** \class SplitGroupConvPass
 *  \brief Split a Conv with attribute group > 1 into multiple smaller Conv with group = 1.
 *
 *         1. This pass will split Conv according to attribute group.
 *            from | -> Conv -> | into | -> Split -> Conv -> Concat -> |
 *                                     |          \  ...   /           |
 *                                     |           - Conv -            |
 *
 *            The input of Conv can be X, W, and Bias (optional)
 *            -> group number must be > 1
 *
 *         2. The input Tensor and output Tensor will remain the same
 *
 *         3. Conv only supports 4 dimension Tensor: (N, C, H, W)
 *            X: (N, C, H, W)
 *            W: (M, C/group, kH, kW), group must be > 1
 *            B: (M)
 *
 *            X Split to  group x (N, C / group, H, W)
 *
 *            W Split to  group x (M / group, C / group, kH, kW)
 *
 *            B is in shape of (M / group)
 *
 *            Output Shape is (N, M, oH, oW)
 */
class SplitGroupConvPass : public CustomPass<SplitGroupConvPass>
{
public:
  SplitGroupConvPass() = default;

  ReturnType runOnModule(Module& pModule) override;

  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

  static void resetIndices() { splitIdx = convIdx = 0; }

private:
  ComputeOperator* genSplitConvsAndReturnConcat(ComputeGraph& pCG, Conv& pConv, Tensor* tensorX, Tensor* tensorW,
                                                Tensor* biasTensor);

private:
  static std::string  splitPrefixName;
  static std::string  convPrefixName;
  static unsigned int splitIdx;
  static unsigned int convIdx;
};

} // namespace onnc

#endif
