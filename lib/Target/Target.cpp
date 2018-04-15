//===-- Target.cpp --------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Target/Target.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Target
//===----------------------------------------------------------------------===//
onnc::Target::Target()
  : m_Name(),
    m_ShortDesc(),
    m_ArchMatchFn(nullptr),
    m_TargetBackendCtorFn(nullptr) {
}

unsigned int onnc::Target::matchArch(const Quadruple& pQuadruple) const
{
  return m_ArchMatchFn(pQuadruple);
} 

onnc::TargetBackend*
onnc::Target::createBackend(const TargetOptions& pOptions) const
{
  return m_TargetBackendCtorFn(pOptions);
}
