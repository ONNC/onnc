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

/** \class onnc::PassInfo
 */
class PassInfo : private Uncopyable
{
public:
  typedef Pass *(*PassCtorFn)(void);

public:
  PassInfo(StringRef pName, const void* pID);

  PassInfo(StringRef pName, const void* pID, PassCtorFn pFn);

  StringRef getPassName() const { return m_Name; }

  const void *getPassID() const { return m_PassID; }

  PassCtorFn getPassCtor() const { return m_PassCtor; }

  void setPassCtor(PassCtorFn pCtor) { m_PassCtor = pCtor; }

  Pass *createPass() const;

private:
  StringRef m_Name;
  const void* m_PassID;
  PassCtorFn m_PassCtor;
};

} // namespace of onnc

#endif
