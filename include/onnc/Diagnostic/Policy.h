//===- DiagnosticPolicy.h -------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_DIAGNOSTIC_POLICY_H
#define ONNC_DIAGNOSTIC_POLICY_H

namespace onnc {
namespace diagnostic {

class Diagnostic;
class GeneralOptions;

/** \class DefaultPolicy
 *  \brief DefaultyPolicy defines the default policy to revise the severity
 *   of diagnostic result.
 */
class DefaultPolicy
{
public:
  static bool process(const GeneralOptions& pOptions, Diagnostic& pDiag);
};

} // namespace of diagnostic
} // namespace of onnc

#endif
