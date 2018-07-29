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
class TargetTransformInfo;
class LowerRegistry;

class TargetBackend
{
public:
  TargetBackend(const TargetOptions& pOptions) { }

  virtual ~TargetBackend() { }

  virtual void addTensorSel(PassManager& pPM) { return; }

  virtual void addTensorSched(PassManager& pPM) { return; }

  virtual void addMemAlloc(PassManager& pPM) { return; }

  virtual void addCodeEmit(PassManager& pPM, const Path& pOutput) { return; }

  virtual const TargetTransformInfo* getTTI() const { return nullptr; }

  /// For the backend using standard TensorSel pass.
  virtual void RegisterLowers(LowerRegistry& pRegistry) const { return; }
};

} // namespace of onnc

#endif
