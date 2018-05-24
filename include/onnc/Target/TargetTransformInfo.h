//===- TargetTransformInfo.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TRANSFORM_INFO_H
#define ONNC_TARGET_TRANSFORM_INFO_H
#include <onnx/common/ir.h>
#include <onnc/Target/TargetMemInfo.h>
#include <vector>

namespace onnc {

using LongInts = std::vector<int64_t>;

/** \class TargetTransformInfo
 *  \brief TargetTransformInfo provides interfaces to explore target
 *         information which is used by target dependent passes.
 */
class TargetTransformInfo {
public:
  enum TargetCostKind {
    kCycleCount,  ///< Get graph (or compute) IR cycle count.
  };

  /// Get coarse-grained (approximately) cost of onnx node.
  virtual int getOperatorCost(const onnx::Node *pNode,
                              enum TargetCostKind kind) const
  {
    return 0;
  }

  /// Get coarse-grained (approximately) total memory usage of onnx node.
  ///
  /// Memory usage of onnx node is target dependent. E.g. a gemm node with
  /// A [N x K], B [K x M], C [N x M], its total memory usage is
  /// (N * K + K * N + N * M) * (Element Size), however, target's might
  /// choose to load part of rows and columns to compute a tile, so its
  /// real memory usage will be reduced a lot.
  virtual MemSize getOperatorMemUsage(const onnx::Node *pNode) const
  {
    return MemSize();
  }

  virtual MemSize getOperatorMemUsage(const onnx::Node *pNode,
                                const std::vector<LongInts> &pInputSizes,
                                const std::vector<LongInts> &pOutputSizes) const
  {
    return MemSize();
  }

  /// @param pIdx Get required memory size of input[pIdx].
  /// @param pNewInputSize Based on new input size, get actual required size.
  virtual MemSize getOperatorInputMemUsage(const onnx::Node *pNode,
                                unsigned pIdx,
                                const LongInts &pNewInputSize) const
  {
    return MemSize();
  }

  virtual MemSize getOperatorOutputMemUsage(const onnx::Node *pNode,
                                unsigned pIdx,
                                const LongInts &pNewOutputSize) const
  {
    return MemSize();
  }

  virtual int getWarpSize() const { return 0; }

  virtual int getProcessingUnitCount() const { return 0; }
};

} // namespace of onnc

#endif
