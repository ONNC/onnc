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
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/AnalysisUsage.h>

#include <cassert>

//===----------------------------------------------------------------------===//
// Implementation of Pass's template member functions
//===----------------------------------------------------------------------===//
namespace onnc {

template<class AnalysisType>
AnalysisType* Pass::getAnalysis() const
{
  static_assert(std::is_base_of<Pass, AnalysisType>::value, "type argument should be derived class of Pass");

  assert(hasResolver() && "Pass not reside a PassManager object!");
  return static_cast<AnalysisType*>(getResolver()->find(AnalysisType::id()));
}

} // namespace onnc

#endif
