//===- ReplaceFlattenByReshape.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===------------------------------------------------------------------------------===//
#ifndef REPLACE_FLATTEN_BY_RESHAPE_H
#define REPLACE_FLATTEN_BY_RESHAPE_H

#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {
class Flatten;

namespace nvdla {

class ReplaceFlattenByReshape : public CustomPass<ReplaceFlattenByReshape>
{
public:
  ReplaceFlattenByReshape() = default;

  ReturnType runOnModule(Module& pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  void replace_flatten_by_reshape(Flatten* flatten, ComputeGraph& pCG);
  void remove_flatten(Flatten* flatten, ComputeGraph& pCG);
  
  static const std::string shapePrefixName;
  static unsigned shapeIdx;
  
  bool is_remove_pattern(Flatten* flatten);
};

} // namespace nvdla
} // namespace onnc

#endif // MODELSIM_REPLACE_FLATTEN_BY_RESHAPE_H
