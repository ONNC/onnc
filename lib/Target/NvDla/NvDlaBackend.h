//===- NvDlaBackend.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_NVDLA_BACKEND_H
#define TARGET_NVDLA_BACKEND_H

#include "CodeEmitVisitor.h"
#include "NvDlaDefine.h"
#include "NvDlaMeta.h"
#include "Version.h"

#include <onnc/IR/ComputeVisitorPass.h>
#include <onnc/Target/TargetBackend.h>
#include <onnc/Target/TargetOptions.h>

#include <string>

namespace onnc {

class NvDlaBackend : public TargetBackend, private NvDlaConstants
{
private:
  static const Version LOADABLE_VERSION;
  static const Version BLOB_DLA_VERSION;
  static const Version BLOB_EMU_VERSION;

private:
  using CodeEmitVisitorPass = ComputeVisitorPass<nvdla::CodeEmitVisitor>;

public:
  explicit NvDlaBackend(const TargetOptions& pOptions);

  virtual ~NvDlaBackend() = default;

  void addTensorSel(PassManager& pPM) override;

  void addOnncIrOptimization(PassManager& pPM, OptimizationOptions& options) override;

  void addTensorSched(PassManager& pPM) override;

  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;

private:
  Tensor::Dimension getMaxNumOfConvChannels(const Conv& pConv);
  CbufAllocType getCbufAllocType(const NvDlaCubeInfo& xinfo, const NvDlaCubeInfo& winfo, Tensor::Dimension yDilation,
                                 unsigned& minNumNeededDataBanks);

private:
  const unsigned         m_VerboseLevel;
  NvDlaBackendMeta       m_pMeta;
  nvdla::CodeEmitVisitor m_CodeEmitVisitor;
};

} // namespace onnc

#endif
