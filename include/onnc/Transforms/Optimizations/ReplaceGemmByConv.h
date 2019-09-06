//===- ReplaceGemmByConv.h ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_REPLACE_GEMM_BY_CONV_H
#define ONNC_REPLACE_GEMM_BY_CONV_H
#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class ReplaceGemmByConv
 *  \brief Replace Gemm by Conv when Gemm meets some conditions:
 *
 *    1. Gemm = aAB + bC
 *       Both B, C are Initializers
 *       Both a, b are constants
 *       There must be A' -> some shaping nodes -> A
 *       and A' is in 4 dimensions
 *
 *    2. Node A can be the output of shaping nodes, and these shaping nodes
 *       will be eliminated (they must form a single linked list)
 *
 *       Current version:
 *         Before:
 *           A' -> Reshape -> ... -> Unsqueeze -> A -> Gemm
 *                                                      /
 *                                              B, C ---
 *         Special Case:
 *           A' -> Reshape -> ReLU -> A -> Gemm
 *                                          /
 *                                  B, C ---
 *
 *         After:
 *           A' -> Conv -> Reshape
 *                 /
 *           B', C
 *
 *    // current only support 4 dims, i.e. exactly d0, d1
 *    3. A must be in shape of (1, k) -> A' is (1, L, d0, d1)
 *       B must be in shape of (k, M) or (M, k) -> B' is (M, L, d0, d1)
 *       C can be in shape of (1), (1, 1), (1, M) or (M) -> (1, M)(Gemm) -> (M)(Conv)
 *
 *    4. TransA in Gemm must be 0
 *       TransB in Gemm can be either 0 or 1
 *
 *    5. the output size of Conv will be (1, M, 1, 1) and Reshape to (1, M)
 *
 *    6. If C not only have values on the last dim, operation of add "bC" cannot
 *       merge into Bias of Conv input!
 *       (It's because output of Conv is (1, M, 1, 1) and add "bC" is applied on
 *        dimension M)
 *
 */
class ReplaceGemmByConv: public CustomPass<ReplaceGemmByConv>
{
public:
  ReplaceGemmByConv() = default;

  ReturnType runOnModule(Module& pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  Tensor* addReshapeBefore(ComputeGraph& pCG, Tensor* inputTensor);

  ComputeOperator* constructConvAndReshape(ComputeGraph& pCG,
                                           Tensor* tA, Tensor* tB, Tensor* tC);
private:
  static const std::string convPrefixName;
  static const std::string reshapePrefixName;
  static const std::string shapePrefixName;

  static unsigned convIdx;
  static unsigned shapeIdx;
  static unsigned reshapeIdx;
};

} // namespace of onnc

#endif // ONNC_REPLACE_GEMM_BY_CONV_H
