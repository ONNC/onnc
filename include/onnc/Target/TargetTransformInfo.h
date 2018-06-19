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

namespace onnc {

/** \class TargetTransformInfo
 *  \brief TargetTransformInfo provides interfaces to explore target
 *         information which is used by target dependent passes.
 */
class TargetTransformInfo {
public:
  enum TargetCostKind : unsigned {
    kCycleCount, ///< Get graph (or compute) IR cycle count.
    BUILTIN_COST_KIND_END
  };

  /// Get coarse-grained (approximately) cost of onnx node.
  virtual uint64_t getOperatorCost(const ::onnx::Node *pNode,
                                   unsigned kind) const
  {
    return 0;
  }

  virtual int getWarpSize() const { return 0; }

  virtual int getProcessingUnitCount() const { return 0; }
};

} // namespace of onnc

#endif
