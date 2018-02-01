//===- TargetBackend.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_BACKEND_H
#define ONNC_TARGET_TARGET_BACKEND_H

namespace onnc {

class CompilerConfig;

class TargetBackend
{
public:
  TargetBackend(const CompilerConfig& pConfig);

  virtual ~TargetBackend();
};

} // namespace of onnc

#endif
