//===- Sophon.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_SOPHON_SOPHON_H
#define TARGET_SOPHON_SOPHON_H
#include <string>

namespace onnc {

extern onnc::Target TheSophonTarget;

TargetBackend* CreateSophonBackend(const TargetOptions& pOptions);

}  // namespace onnc

#endif  // TARGET_SOPHON_SOPHON_H
