//===- TargetBackend.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_BACKEND_H
#define ONNC_TARGET_TARGET_BACKEND_H
#include <onnc/Core/PassManager.h>
#include <onnc/Support/Path.h>

namespace onnc {

class TargetMemInfo;
class TargetOptions;

class TargetBackend
{
public:
  TargetBackend(const TargetOptions& pOptions){}

  virtual ~TargetBackend(){}

  /// These enums are meant to be passed into addPassesToEmitFile to indicate
  /// what type of file to emit, and returned by it to indicate what type of
  /// file could actually be made.
  enum CodeGenFileType {
    AssemblyFile,
    ObjectFile,
    Null // Do not emit any output.
  };

  virtual bool addPassesToEmitFile(PassManager &pPM, Path output, CodeGenFileType fileType);

  virtual void addTensorSel(PassManager& pPM) { return; }

  virtual void addMemAlloc(PassManager& pPM) { return; }

  virtual void addCodeEmit(PassManager& pPM, Path &output, CodeGenFileType &fileType) { return; }

  /// This is not a constant function. Because there in case memory emulator must change its
  /// internal state.
  TargetMemInfo* getMemInfo() { return m_pMemInfo; }

  const TargetMemInfo* getMemInfo() const { return m_pMemInfo; }

private:
  TargetMemInfo* m_pMemInfo;
};

} // namespace of onnc

#endif
