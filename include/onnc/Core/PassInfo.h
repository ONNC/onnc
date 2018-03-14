//===- PassInfo.h ---------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_INFO_H
#define ONNC_CORE_PASS_INFO_H
#include <onnc/ADT/StringRef.h>
#include <onnc/ADT/Uncopyable.h>
#include <onnc/Core/Pass.h>

namespace onnc {

class TargetBackend;

/** \class onnc::PassInfo
 */
class PassInfo : private Uncopyable
{
public:
  typedef Pass *(*PassCtorFn)(void);
  typedef Pass *(*PassTargetCtorFn)(TargetBackend*);

public:
  PassInfo(StringRef pName, Pass::AnalysisID pID);

  PassInfo(StringRef pName, Pass::AnalysisID pID,
           PassCtorFn pFn, PassTargetCtorFn pTrgFn = nullptr);

  StringRef getPassName() const { return m_Name; }

  Pass::AnalysisID getPassID() const { return m_PassID; }

  PassCtorFn getPassCtor() const { return m_PassCtor; }

  void setPassCtor(PassCtorFn pCtor) { m_PassCtor = pCtor; }

  PassTargetCtorFn getPassTargetCtor() const { return m_PassTargetCtor; }

  void setPassTargetCtor(PassTargetCtorFn pCtor) { m_PassTargetCtor = pCtor; }

  bool hasTargetCtor() const { return (nullptr != m_PassTargetCtor); }

  /// Using PassTargetCtorFn to create a Pass only if @ref pTB is
  /// not null. Otherwise, using PassCtorFn.
  Pass *makePass(TargetBackend* pTB = nullptr) const;

private:
  /// Using PassCtorFn to create a pass
  Pass *createPass() const;

  /// Using PassTargetCtorFn to create a Pass
  Pass *createPass(TargetBackend* pTB) const;

private:
  StringRef m_Name;
  Pass::AnalysisID m_PassID;
  PassCtorFn m_PassCtor;
  PassTargetCtorFn m_PassTargetCtor;
};

} // namespace of onnc

#endif
