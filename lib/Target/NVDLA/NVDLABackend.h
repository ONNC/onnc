//===- NVDLABackend.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_NVDLA_BACKEND_H
#define TARGET_NVDLA_NVDLA_BACKEND_H
#include <string>
#include <onnc/Target/DLATargetBackend.h>

namespace onnc {

class NVDLABackend : public DLATargetBackend
{
public:
  NVDLABackend(const TargetOptions& pOptions);

  virtual ~NVDLABackend();

  void addTensorSel(PassManager& pPM) override;

  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;
};

}  // namespace onnc

#endif
