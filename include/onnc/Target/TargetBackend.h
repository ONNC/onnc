//===- TargetBackend.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_BACKEND_H
#define ONNC_TARGET_TARGET_BACKEND_H
#include <memory>

#include <onnc/Core/PassManager.h>
#include <onnc/Target/TargetMemInfo.h>
#include <onnc/Target/TargetOptions.h>
#include <onnc/Transforms/Optimizations/OptimizationOptions.h>
#include <onnc/Support/Path.h>
#include <onnc/Runtime/Interpreter.h>

#include <memory>

namespace onnc {

class TargetOptions;
class TargetTransformInfo;
class LowerRegistry;

class TargetBackend
{
public:
  TargetBackend(const TargetOptions& pOptions)
    : m_TargetOptions(pOptions) {
  }

  virtual ~TargetBackend() { }

  virtual void addTensorSel(PassManager& pPM) { return; }

  virtual void addOnncIrOptimization(PassManager& pPM, OptimizationOptions& options)
  {
    OptimizationOptions::add(pPM, options);
  }

  virtual void addTensorSched(PassManager& pPM) { return; }

  virtual void addMemAlloc(PassManager& pPM) { return; }

  virtual void addCodeEmit(PassManager& pPM, const Path& pOutput) { return; }

  virtual const TargetTransformInfo* getTTI() const { return nullptr; }

  virtual Interpreter *createTargetInterpreter() const { return new Interpreter(); }

  /// For the backend using standard TensorSel pass.
  virtual void RegisterLowers(LowerRegistry& pRegistry) const { return; }

  const TargetOptions& options() const { return m_TargetOptions; }

  /// This is not a constant function. Because there in case memory emulator
  /// must change its internal state.
  TargetMemInfo* getMemInfo() { return m_pMemInfo.get(); }

  const TargetMemInfo* getMemInfo() const { return m_pMemInfo.get(); }

protected:
  std::unique_ptr<TargetMemInfo> m_pMemInfo;

private:
  const TargetOptions& m_TargetOptions;
};

} // namespace of onnc

#endif
