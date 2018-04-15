//===- NPUTargetBackend.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_NETWORK_PROCESSING_UNIT_TARGET_BACKEND_H
#define ONNC_TARGET_NETWORK_PROCESSING_UNIT_TARGET_BACKEND_H
#include <onnc/Target/TargetBackend.h>

namespace onnc {

class TargetOptions;

class NPUTargetBackend : public TargetBackend
{
public:
  NPUTargetBackend(const TargetOptions& pOptions);

  virtual ~NPUTargetBackend();
};

} // namespace of onnc

#endif
