//===- AnalysisUsage.h -----------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_ANALYSIS_USAGE_H
#define ONNC_CORE_ANALYSIS_USAGE_H
#include <onnc/Core/Pass.h>
#include <vector>

namespace onnc {

/** \class AnalysisUsage
 *  \brief AnalysisUsage represents the analysis usage information of a pass.
 *
 *  An AnalysisUsage object analyses that the pass REQUIRES (must ran before the
 *  pass runs).
 */
class AnalysisUsage
{
public:
  typedef std::vector<Pass::AnalysisID> IDList;

public:
  AnalysisUsage() { }

  AnalysisUsage& addRequiredID(Pass::AnalysisID pID);

  AnalysisUsage& addRequiredID(char& pID);

  template<class PassClass>
  AnalysisUsage& addRequired() {
    return addRequiredID(PassClass::ID);
  }

  const IDList& getRequiredSet() const { return m_Required; }

private:
  IDList m_Required;
};

} // namespace of onnc

#endif
