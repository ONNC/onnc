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

#include <cstdint>

namespace onnc {

class AnalysisUsage;
class AnalysisResolver;

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

  enum PassResult : uint32_t {
    kModuleNoChanged = 0x0,
    kModuleChanged   = 0x1,
    kPassRetry       = 0x2,
    kPassFailure     = 0x4
  };

  /// Identity of a pass
  typedef std::intptr_t AnalysisID;

  /// ReturnType is an union of PassResult
  typedef std::uint32_t ReturnType;

public:
  explicit Pass(Kind pKind) noexcept
    : m_Kind{pKind}, m_pResolver{nullptr},
      m_TimeStep(0), m_Module(nullptr) { }

  virtual ~Pass();

  Kind getPassKind() const { return m_Kind; }

  virtual AnalysisID getPassID() const = 0;

  virtual StringRef getPassName() const;

  /// Virtual method overridden by subclasses to do any necessary
  /// initialization before any pass is run.
  virtual ReturnType doInitialization(Module& pModule) { return kModuleNoChanged; }

  /// Execute all of the passes scheduled for execution
  ReturnType run(Module& pModule);

  /// Virtual method overridden by subclasses to do any necessary
  /// finalization before any pass is run.
  virtual ReturnType doFinalization(Module& pModule) { return kModuleNoChanged; }

  virtual void clear() {}

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

  /// This function should be overridden by passes that need analysis information.
  virtual void getAnalysisUsage(AnalysisUsage& pUsage) const { }

  void setResolver(AnalysisResolver& pResolver) { m_pResolver = &pResolver; }

  AnalysisResolver* getResolver() const { return m_pResolver; }

  bool hasResolver() const { return (nullptr != m_pResolver); }

  template<class AnalysisType> AnalysisType* getAnalysis() const;

  /// the 1st bit is set
  static bool IsRevised(ReturnType pR) { return (0x0 != (pR & kModuleChanged)); }

  /// the 2nd bit is set
  static bool IsRetry(ReturnType pR) { return (0x0 != (pR & kPassRetry)); }

  /// the 3rd bit is set
  static bool IsFailed(ReturnType pR) { return (0x0 != (pR & kPassFailure)); }

  unsigned getTimeStep() const { return m_TimeStep; }

  void setTimeStep(unsigned pTime) { m_TimeStep = pTime; }

  const Module* getModule() const { return m_Module; }

  void setModule(Module* pM) { m_Module = pM; }

private:
  const Kind m_Kind;
  AnalysisResolver* m_pResolver;

  // Executing time step. m_TimeStep and m_Module are used by PassManager to
  // decide whether a pass needs to execute.
  // @see PassManager::m_TimeStep
  unsigned m_TimeStep;

  // Module that pass is processing.
  Module* m_Module;
};

} // namespace of onnc

#endif
