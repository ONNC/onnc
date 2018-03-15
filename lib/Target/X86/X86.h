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

TargetBackend* CreateX86Backend(const TargetOptions& pOptions);

}  // namespace onnc

#endif  // TARGET_X86_X86_H_
