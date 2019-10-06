//===- NvDlaCollectReshapeInfoPass.cpp ------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "NvDlaCollectReshapeInfoPass.h"

#include <onnc/IR/Compute/Initializer.h>
#include <onnc/IR/Compute/Reshape.h>
#include <onnc/IR/Compute/Tensor.h>
#include <onnc/Support/Casting.h>

namespace onnc {

NvDlaCollectReshapeInfoPass::NvDlaCollectReshapeInfoPass(NvDlaBackendMeta& meta) noexcept
  : meta(meta)
{}

Pass::ReturnType NvDlaCollectReshapeInfoPass::runOnModule(Module& pModule)
{
  for (ComputeOperator& cm : *pModule.getRootComputeGraph()) {
    if (const Reshape* const reshape = dyn_cast<Reshape>(&cm)) {
      const Tensor* const          shape         = reshape->getShape();
      const ComputeOperator* const shapeProducer = static_cast<const ComputeOperator*>(shape->getDefine());
      assert(isa<Initializer>(shapeProducer));

      const Tensor* const input  = reshape->getInput(0);
      const Tensor* const output = reshape->getOutput(0);
      meta.markAsReshaped(*input, *output);
    }
  }

  return Pass::kModuleNoChanged;
}
} // namespace onnc
