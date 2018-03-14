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

namespace onnc {

class CompilerConfig;

class TargetBackend
{
public:
  TargetBackend(const CompilerConfig& pConfig){}

  virtual ~TargetBackend(){}

  virtual void addTensorSel(PassManager& pPM) { return; }

  virtual void addMemAlloc(PassManager& pPM) { return; }

  virtual void addCodeEmit(PassManager& pPM) { return; }
};

} // namespace of onnc

#endif
