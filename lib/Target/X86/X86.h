//===- X86.h --------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef TARGET_X86_X86_H
#define TARGET_X86_X86_H
#include <string>

namespace onnc {

class Target;
class TargetLDBackend;

extern onnc::Target TheX86_32Target;
extern onnc::Target TheX86_64Target;

TargetBackend* CreateX86Backend(const CompilerConfig& pConfig);

}  // namespace onnc

#endif  // TARGET_X86_X86_H_
