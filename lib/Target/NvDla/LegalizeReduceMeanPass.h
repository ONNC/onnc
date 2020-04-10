//===- LegalizeReduceMeanPass.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LEGALIZE_REDUCE_MEAN_PASS_H
#define LEGALIZE_REDUCE_MEAN_PASS_H

#include "NvDlaDefine.h"

#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/GlobalAveragePool.h>
#include <onnc/IR/Compute/ReduceMean.h>

namespace onnc {
namespace nvdla {

class LegalizeReduceMeanPass : public CustomPass<LegalizeReduceMeanPass>
{
public:
  LegalizeReduceMeanPass() = default;
  ReturnType runOnModule(Module& pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  GlobalAveragePool* replace(ReduceMean* reducemean, ComputeGraph& pCG);
  void add_reshape(GlobalAveragePool* op, ComputeGraph& pCG);
  bool is_replace_pattern(ReduceMean* op);

  static const std::string shapePrefixName;
  static unsigned shapeIdx;
};

} // namespace nvdla
} // namespace onnc

#endif
