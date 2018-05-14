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
  enum TargetCostKind {
    kCycleCount,  ///< Get graph (or compute) IR cycle count.
  };

  virtual int getOperatorCost(const onnx::Node *pNode,
                              enum TargetCostKind kind) const { return 0; }

  virtual int getOperatorMemorySize(onnx::Value *pValue) { return 0; }

  virtual int getWarpSize() const { return 0; }

  virtual int getProcessingUnitCount() const { return 0; }
};

} // namespace of onnc

#endif
