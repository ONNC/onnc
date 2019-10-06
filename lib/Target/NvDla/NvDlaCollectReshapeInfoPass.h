//===- NvDlaCollectReshapeInfoPass.h --------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_NVDLA_NVDLA_COLLECT_RESHAPE_INFO_PASS_H_INCLUDED
#define ONNC_TARGET_NVDLA_NVDLA_COLLECT_RESHAPE_INFO_PASS_H_INCLUDED

#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class NvDlaCollectReshapeInfoPass
 *  \brief Collect information of Reshape operators
 */
class NvDlaCollectReshapeInfoPass : public CustomPass<NvDlaCollectReshapeInfoPass>
{
public:
  NvDlaCollectReshapeInfoPass(NvDlaBackendMeta& pMeta) noexcept;

  ReturnType runOnModule(Module& pModule) override;

private:
  NvDlaBackendMeta& meta;
};

} // namespace onnc

#endif
