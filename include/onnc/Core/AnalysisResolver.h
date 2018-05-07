//===- AnalysisResolver.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_ANALYSIS_RESOLVER_H
#define ONNC_CORE_ANALYSIS_RESOLVER_H
#include <onnc/Core/PassManager.h>
#include <vector>
#include <utility>

namespace onnc {

/** \class AnalysisResolver
 *  \brief AnalysisResolver tracks the required passes of a certain pass.
 */
class AnalysisResolver
{
public:
  explicit AnalysisResolver(PassManager& pPM)
    : m_DepPasses(), m_PM(pPM) {
  }

  Pass* find(Pass::AnalysisID pID);

  void add(Pass::AnalysisID pID, Pass& pPass);

  unsigned int getNumOfPasses() const { return m_DepPasses.size(); }

private:
  // vector is fast enough.
  typedef std::vector<std::pair<Pass::AnalysisID, Pass*> > DepPassList;

private:
  DepPassList m_DepPasses;
  PassManager& m_PM;
};

} // namespace of onnc

#endif
