//===- CortexMBackend.h
//-------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CORTEXM_BACKEND_H
#define TARGET_CORTEXM_BACKEND_H
#include "CodeEmitVisitor.h"
#include "CortexmBackendMeta.h"

#include <onnc/Target/TargetBackend.h>

#include <string>

namespace onnc {

class CortexmBackend : public TargetBackend
{
public:
  CortexmBackend(const TargetOptions& pOptions);

  virtual ~CortexmBackend() = default;

  void addTensorSel(PassManager& pPM) override;

  void addTensorSched(PassManager& pPM) override;

  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;

private:
  CortexmBackendMeta m_pMeta;

  cortexm::CodeEmitVisitor m_CodeEmitVisitor;
};

} // namespace onnc

#endif
