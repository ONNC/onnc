//===- TG.h -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_TG_TG_H
#define TARGET_TG_TG_H
#include <string>

namespace onnc {

extern onnc::Target TheTGTarget;

TargetBackend* CreateTGBackend(const CompilerConfig& pConfig);

}  // namespace onnc

#endif  // TARGET_TG_TG_H
