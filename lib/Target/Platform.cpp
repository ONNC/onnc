//===-- Platform.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Target/Platform.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Platform
//===----------------------------------------------------------------------===//
onnc::Platform::Platform(const std::string& pName)
  : m_Name(pName),
    m_ShortDesc(),
    m_TargetBackends() {
}
