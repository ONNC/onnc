//===- AnalysisResolver.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/AnalysisResolver.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AnalysisResolver
//===----------------------------------------------------------------------===//
Pass* AnalysisResolver::find(Pass::AnalysisID pID)
{
  // sequential search
  for (const auto& pass_pair : m_DepPasses) {
    if (pass_pair.first == pID) {
      return pass_pair.second;
    }
  }
  return nullptr;
}

void AnalysisResolver::add(Pass::AnalysisID pID, Pass& pPass)
{
  if (nullptr != find(pID)) //< already added
    return;

  m_DepPasses.push_back(std::make_pair(pID, &pPass));
}
