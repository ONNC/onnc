//===- InterpreterPass.h --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_NVDLAMEMINFO_PASS_H
#define ONNC_NVDLAMEMINFO_PASS_H

#include <onnc/Core/ModulePass.h>

#include "NvdlaMeta.h"

namespace onnc {

class TargetBackend;

// XXX: Experimental

/** \class InterpreterPass
 *  \brief Run interpreter.
 */
class NvdlaMemInfoPass : public ModulePass
{
public:
  static char ID;

public:
  NvdlaMemInfoPass(TargetBackend *pBackend,
                  NvdlaBackendMeta *pMeta);

  ReturnType runOnModule(Module& pModule) override;

private:

  TargetBackend *m_pBackend;
  NvdlaBackendMeta *m_pMeta;
};

// XXX: Experimental
NvdlaMemInfoPass *CreateNvdlaMemInfoPass(TargetBackend *pBackend,
                                          NvdlaBackendMeta *pMeta);

} // namespace of onnc

#endif
