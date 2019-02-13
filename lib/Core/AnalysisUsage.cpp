//===- AnalysisUsage.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/AnalysisUsage.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// AnalysisUsage
//===----------------------------------------------------------------------===//
AnalysisUsage& AnalysisUsage::addRequiredID(Pass::AnalysisID pID)
{
  m_Required.push_back(pID);
  return *this;
}
