//===- Pass.h -------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_H
#define ONNC_CORE_PASS_H
#include <onnc/IR/Module.h>
#include <onnc/ADT/StringRef.h>
#include <onnc/Support/OStream.h>

namespace onnc {

class AnalysisUsage;

/** \class onnc::Pass
 *  \brief encapsulate transformation algorithms.
 */
class Pass
{
public:
  enum Kind {
    kPT_Module,
    kPT_Tensor
  };

  /// Identity of a pass
  typedef const void* AnalysisID;

public:
  explicit Pass(Kind pKind, char& pPassID)
    : m_Kind(pKind), m_PassID(&pPassID) { }

  virtual ~Pass();

  Kind getPassKind() const { return m_Kind; }

  AnalysisID getPassID() const { return m_PassID; }

  virtual StringRef getPassName() const;

  /// Virtual method overridden by subclasses to do any necessary
  /// initialization before any pass is run.
  ///
  /// @retval true If we modified the module
  /// @retval false We didn't modify the module
  virtual bool doInitialization(Module& pModule) { return false; }

  /// Execute all of the passes scheduled for execution
  /// @retval true The pass modifies the module.
  /// @retval false The pass didn't modify the module.
  bool run(Module& pModule);

  /// Virtual method overridden by subclasses to do any necessary
  /// finalization before any pass is run.
  ///
  /// @retval true We modified the module
  /// @retval false We didn't modify the module
  virtual bool doFinalization(Module& pModule) { return false; }

  /// Print out the internal state of the pass.
  /// Beware that the module pointer MAY be null.
  /// This automatically forwards to a virtual function that does not
  /// provide the Module* in case the analysis doesn't need it.
  ///
  /// @param[in] pModule The module MAY be null in case analysis should ignore
  ///                    it.
  virtual void print(OStream &pOS, const Module *pModule) const;

  /// A convenient function to print to stderr.
  void dump();

  /// This function should be overriden by passes that need analysis information.
  virtual void getAnalysisUsage(AnalysisUsage& pUsage) const { }

private:
  Kind m_Kind;
  AnalysisID m_PassID;
};

} // namespace of onnc

#endif
