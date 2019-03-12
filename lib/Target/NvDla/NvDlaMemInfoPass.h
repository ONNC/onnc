//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAMEMINFO_PASS_H
#define ONNC_NVDLAMEMINFO_PASS_H

#include "NvDlaMeta.h"

#include <onnc/Core/CustomPass.h>

namespace onnc {

class TargetBackend;

// XXX: Experimental

/** \class InterpreterPass
 *  \brief Run interpreter.
 */
class NvDlaMemInfoPass : public CustomPass<NvDlaMemInfoPass>
{
public:
  explicit NvDlaMemInfoPass(NvDlaBackendMeta* pMeta);

  ReturnType runOnModule(Module& pModule) override;

private:
  NvDlaBackendMeta* m_pMeta;
};

} // namespace onnc

#endif
