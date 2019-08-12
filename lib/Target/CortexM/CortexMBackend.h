//===- CortexMBackend.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CORTEXM_CORTEXM_BACKEND_H
#define TARGET_CORTEXM_CORTEXM_BACKEND_H
#include <string>
#include <onnc/Target/TargetBackend.h>
#include "CortexMMeta.h"
#include "CodeEmitVisitor.h"

namespace onnc {

class CortexMBackend : public TargetBackend
{
public:
  CortexMBackend(const TargetOptions& pOptions);

  virtual ~CortexMBackend() = default;

  void addTensorSel(PassManager& pPM) override;

  void addTensorSched(PassManager& pPM) override;
  
  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;

  private:
    CortexMBackendMeta m_pMeta;
    
    cortexm::CodeEmitVisitor m_CodeEmitVisitor;
};

}  // namespace onnc

#endif
