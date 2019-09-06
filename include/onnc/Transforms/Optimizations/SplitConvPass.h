//===- SplitConvPass.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TRANSFORMS_OPTIMIZATIONS_SPLIT_CONV_PASS_H_INCLUDED
#define ONNC_TRANSFORMS_OPTIMIZATIONS_SPLIT_CONV_PASS_H_INCLUDED
#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Conv.h>

#include <functional>
#include <utility>

namespace onnc {

/** \class SplitConvPass
 *  \brief Split Conv on channel axis according to hardware config
 *
 *         1. Given a function: input Conv -> output Num of Max Channel
 *            This pass will split Conv according to Num of Max Channel
 *            from | -> Conv -> | into | -> Split -> Conv -> Sum -> |
 *                                     |          \  ...   /        |
 *                                     |           - Conv -         |
 *
 *            The input of Conv can be X, W, and Bias (optional)
 *            -> group number must be 1
 *
 *         2. The input Tensor and output Tensor will remain the same
 *
 *         3. Conv only supports 4 dimension Tensor: (N, C, H, W)
 *            X: (N, C, H, W)
 *            W: (M, C/group, kH, kW), group must be 1
 *            B: (M)
 *
 *            X Split to  t x (N, S, H, W)
 *                        1 x (N, C % S, H, W)
 *
 *            W Split to  t x (M, S, kH, kW)
 *                        1 x (M, C % S, kH, kW)    ->  tS + (C % S) = C
 *
 *            B is in shape of (M), and it will be added to one of new Conv
 *
 *            Output Shape is (N, M, oH, oW)
 */
class SplitConvPass : public CustomPass<SplitConvPass>
{
public:
  using MaxNumOfConvChannelsGetter = std::function<Tensor::Dimension(const Conv&)>;

  SplitConvPass() = delete;
  explicit SplitConvPass(MaxNumOfConvChannelsGetter maxNumOfConvChannelsGetter)
    : m_MaxNumOfConvChannelsGetter(std::move(maxNumOfConvChannelsGetter))
  {}

  ReturnType runOnModule(Module& pModule) override;

  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

  static void resetIndices() { splitIdx = convIdx = 0; }

private:
  Tensor::Dimension getMaxNumOfConvChannels(const Conv& pConv);

  ComputeOperator* genSplitConvsAndReturnSum(ComputeGraph& pCG, Conv& pConv, Tensor& tensorX, Tensor& tensorW,
                                             Tensor* biasTensor);

private:
  const MaxNumOfConvChannelsGetter m_MaxNumOfConvChannelsGetter;

  static std::string  splitPrefixName;
  static std::string  convPrefixName;
  static unsigned int splitIdx;
  static unsigned int convIdx;
};

} // namespace onnc

#endif // TARGET_NVDLA_SPLIT_CONV_H
