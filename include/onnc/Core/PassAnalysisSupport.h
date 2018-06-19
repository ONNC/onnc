//===- PassAnalysisSupport.h ----------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_ANALYSIS_SUPPORT_H
#define ONNC_CORE_PASS_ANALYSIS_SUPPORT_H
#include <onnc/Core/Pass.h>


//===----------------------------------------------------------------------===//
// Implementation of Pass's template member functions
//===----------------------------------------------------------------------===//
template<class AnalysisType>
AnalysisType* onnc::Pass::getAnalysis() const
{
  assert(hasResolver() && "Pass not reside a PassManager object!");
  return (AnalysisType*)getResolver()->find(&AnalysisType::ID);
}

#endif
