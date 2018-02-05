//===- X86TargetInfo.h ----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_X86_TARGET_INFO_H
#define ONNC_TARGET_X86_TARGET_INFO_H
#include <onnc/Target/Target.h>

namespace onnc {

onnc::Target& getTheX86_32Target();

onnc::Target& getTheX86_64Target();

} // namespace of onnc

#endif
