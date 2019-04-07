//===- CLangBackend.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_CLANG_CLANG_BACKEND_H
#define TARGET_CLANG_CLANG_BACKEND_H

#include <string>

#include <onnc/Target/TargetBackend.h>

#include "CLangMeta.h"

namespace onnc {

class CLangBackend : public TargetBackend
{
public:
  CLangBackend(const TargetOptions& pOptions);

  virtual ~CLangBackend() = default;

  void addTensorSel(PassManager& pPM) override;

  void addTensorSched(PassManager& pPM) override;
  
  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;

private:
  CLangMeta m_pMeta;
};

}  // namespace onnc

#endif
