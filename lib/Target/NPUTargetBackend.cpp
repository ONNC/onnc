//===-- NPUTargetBackend.cpp ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Target/NPUTargetBackend.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// NPUTargetBackend
//===----------------------------------------------------------------------===//
NPUTargetBackend::NPUTargetBackend(const TargetOptions& pOptions)
  : TargetBackend(pOptions) {
}

NPUTargetBackend::~NPUTargetBackend()
{
}
