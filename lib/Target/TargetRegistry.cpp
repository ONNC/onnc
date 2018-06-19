//===-- TargetRegistry.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Target/TargetRegistry.h>
#include <onnc/Support/ManagedStatic.h>
#include <onnc/IR/Quadruple.h>
#include <onnc/ADT/Rope.h>
#include <algorithm>

using namespace onnc;

static onnc::ManagedStatic<onnc::TargetRegistry::TargetList> s_TargetList;

//===----------------------------------------------------------------------===//
// onnc::TargetRegistry
//===----------------------------------------------------------------------===//
onnc::TargetRegistry::iterator TargetRegistry::Begin()
{
  return s_TargetList->begin();
}

onnc::TargetRegistry::iterator TargetRegistry::End()
{
  return s_TargetList->end();
}

size_t onnc::TargetRegistry::Size()
{
  return s_TargetList->size();
}

bool onnc::TargetRegistry::IsEmpty()
{
  return s_TargetList->empty();
}

void onnc::TargetRegistry::RegisterTarget(Target& pTarget,
                                    const char* pName,
                                    const char* pShortDesc,
                                    Target::QuadrupleMatchFnTy pArchMatchFn)
{
  pTarget.m_Name = pName;
  pTarget.m_ShortDesc = pShortDesc;
  pTarget.m_ArchMatchFn = pArchMatchFn;
  s_TargetList->push_back(&pTarget);
}

const onnc::Target*
onnc::TargetRegistry::Lookup(const std::string& pQuadruple, std::string& pError)
{
  // Provide special warning when no targets are initialized.
  if (IsEmpty()) {
    pError = "Unable to find target for this quadruple (no targets are registered)";
    return nullptr;
  }

  Quadruple input(pQuadruple);
  unsigned int candidateScore = 0;
  onnc::TargetRegistry::iterator candidate = End();
  for (auto cb = Begin(), ce = End(); cb != ce; ++cb) {
    auto curScore = (*cb)->matchArch(input);
    if (0 != curScore && curScore > candidateScore) {
      candidate = cb;
      candidateScore = curScore;
    }
  }

  if (End() == candidate) {
    pError = "No available targets are compatible with this quadruple.";
    return nullptr;
  }

  return *candidate;
}
