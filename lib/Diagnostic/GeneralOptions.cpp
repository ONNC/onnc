//===- GeneralOptions.cpp -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Diagnostic/GeneralOptions.h>

using namespace onnc;
using namespace onnc::diagnostic;

//===----------------------------------------------------------------------===//
// GeneralOptions
//===----------------------------------------------------------------------===//
GeneralOptions::GeneralOptions()
  : m_bIsFatalWarnings(false), m_Verbose(0),
    m_MaxErrorNum(10), m_MaxWarnNum(10) {
}
