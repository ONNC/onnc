//===- NvDlaBackend.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_BACKEND_H
#define TARGET_NVDLA_BACKEND_H
#include <string>
#include <onnc/Target/TargetBackend.h>

#include "NvDlaMeta.h"
#include "CodeEmitVisitor.h"

namespace onnc {

class NvDlaBackend : public TargetBackend
{
public:
  NvDlaBackend(const TargetOptions& pOptions);

  virtual ~NvDlaBackend() = default;

  void addTensorSel(PassManager& pPM) override;

  void addTensorSched(PassManager& pPM) override;

  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;

private:
  NvDlaBackendMeta m_pMeta;
  nvdla::CodeEmitVisitor m_CodeEmitVisitor;
};

}  // namespace onnc

#endif
