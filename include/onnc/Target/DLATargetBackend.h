//===- DLATargetBackend.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_DL_ACCELERATOR_TARGET_BACKEND_H
#define ONNC_TARGET_DL_ACCELERATOR_TARGET_BACKEND_H
#include <onnc/Target/TargetBackend.h>

namespace onnc {

class TargetOptions;

class DLATargetBackend : public TargetBackend
{
public:
  DLATargetBackend(const TargetOptions &pOptions) : TargetBackend(pOptions) {}

  virtual ~DLATargetBackend(){}
};

} // namespace of onnc

#endif
