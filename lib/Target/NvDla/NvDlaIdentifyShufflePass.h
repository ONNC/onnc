//===- NvDlaIdentifyShufflePass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===------------------------------------------------------------------------------===//
#ifndef NVDLA_IDENTIFY_SHUFFLE_PASS_H
#define NVDLA_IDENTIFY_SHUFFLE_PASS_H

#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {

class NvDlaIdentifyShufflePass : public CustomPass<NvDlaIdentifyShufflePass>
{
public:
  NvDlaIdentifyShufflePass() = default;

  ReturnType runOnModule(Module& pModule) override;

private:
  bool is_shuffle(Reshape* shape);
};

} // namespace onnc

#endif // MODELSIM_IDENTIFY_SHUFFLE_PASS_H
