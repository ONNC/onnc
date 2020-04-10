//===- LegalizeClipToMinMaxPass.h ---------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef LEGALIZE_CLIP_TO_MIN_MAX_PASS_H
#define LEGALIZE_CLIP_TO_MIN_MAX_PASS_H

#include "NvDlaDefine.h"

#include <onnc/Core/CustomPass.h>
#include <onnc/IR/Compute/Clip.h>
#include <onnc/IR/Compute/Max.h>
#include <onnc/IR/Compute/Min.h>

namespace onnc {
namespace nvdla {

class LegalizeClipToMinMaxPass : public CustomPass<LegalizeClipToMinMaxPass>
{
public:
  LegalizeClipToMinMaxPass() = default;
  ReturnType runOnModule(Module& pModule) override;
  ReturnType runOnComputeGraph(ComputeGraph& pCG) override;

private:
  void legalize(Clip* clip, ComputeGraph& pCG);
  static unsigned clipIdx;

};

} // namespace nvdla
} // namespace onnc

#endif
