//===- GeneralOptions.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_GENERAL_OPTIONS_H
#define ONNC_DIAGNOSTIC_GENERAL_OPTIONS_H
#include <onnc/Support/DataTypes.h>

namespace onnc {
namespace diagnostic {

/** \class GeneralOptions
 *  \brief GeneralOptions collects the frequent-used options.
 */
class GeneralOptions
{
public:
  GeneralOptions();

  bool isFatalWarnings() const { return m_bIsFatalWarnings; }

  void setFatalWarnings(bool pEnable = true) { m_bIsFatalWarnings = pEnable; }

  /// return the verbose level [-1, 0, 1, 2]
  int verbose() const { return m_Verbose; }

  /// --verbose[=-1, 0, 1, 2]
  void setVerbose(int pVerbose = -1) { m_Verbose = pVerbose; }

  /// get the limit of maximum number of errors.
  unsigned int getMaxErrorNum() const { return m_MaxErrorNum; }

  /// --error-limit=N
  void setMaxErrorNum(unsigned int pMaxErrorNum) { m_MaxErrorNum = pMaxErrorNum; }

  /// get the limit of maximum number of warnings.
  unsigned int getMaxWarnNum() const { return m_MaxWarnNum; }

  /// --warning-limit=N
  void setMaxWarnNum(unsigned int pMaxWarnNum) { m_MaxWarnNum = pMaxWarnNum; }

private:
  bool m_bIsFatalWarnings : 1; ///< --fatal-warnings
  int m_Verbose;               ///< --verbose[=0,1,2]
  unsigned int m_MaxErrorNum;       ///< --error-limit=N
  unsigned int m_MaxWarnNum;        ///< --warning-limit=N
};

} // namespace of diagnostic
} // namespace of onnc

#endif
