//===- NvDlaMemInfoPass.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAMEMINFO_PASS_H
#define ONNC_NVDLAMEMINFO_PASS_H

#include "NvDlaDefine.h"
#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {

/** \class NvDlaMemInfoPass
 *  \brief Allocate memory for tensors
 */
class NvDlaMemInfoPass : public CustomPass<NvDlaMemInfoPass>, private NvDlaConstants
{
public:
  NvDlaMemInfoPass(const NvDlaConstants& constants, NvDlaBackendMeta* pMeta) noexcept;

  ReturnType runOnModule(Module& pModule) override;

private:
  NvDlaBackendMeta* m_pMeta;
};

} // namespace onnc

#endif
