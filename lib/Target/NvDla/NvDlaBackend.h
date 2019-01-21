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

  virtual ~NvDlaBackend();

  void addTensorSel(PassManager& pPM) override;

  void addTensorSched(PassManager& pPM) override;

  void addMemAlloc(PassManager& pPM) override;

  void addCodeEmit(PassManager& pPM, const Path& pOutput) override;

  void RegisterLowers(LowerRegistry& pRegistry) const override;

#if 0
  // memory allocation information for runtime (firmwares, memory buffer)
  std::vector<ILoadable::MemoryListEntry> m_MemoryListEntries;
  // addresses used in firmware
  std::vector<ILoadable::AddressListEntry> m_AddressListEntries;
  // input, output specific descriptor
  std::vector<ILoadable::TensorDescListEntry> m_TensorDescListEntries;
  // relocation information of input/output
  std::vector<ILoadable::RelocEntry> m_RelocEntries;

  // blobs, firmware binary (operators, initializer data)
  std::map<std::string, Loadable::Symbol> m_Symbols;

  // DLA or EMU batch tasks
  std::vector<ILoadable::TaskListEntry> m_TaskListEntries;
  // batch task submit order
  std::vector<ILoadable::SubmitListEntry> m_SubmitListEntries;
  // events between submits
  std::vector<ILoadable::EventListEntry> m_EventListEntries;
#endif

  NvDlaBackendMeta m_pMeta;
  nvdla::CodeEmitVisitor m_CodeEmitVisitor;
  //Loadable m_loadable;
};

}  // namespace onnc

#endif
