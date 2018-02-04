//===- TargetRegistry.h ---------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_TARGET_TARGET_REGISTRY_H
#define ONNC_TARGET_TARGET_REGISTRY_H
#include <onnc/IR/Quadruple.h>
#include <onnc/Target/Target.h>
#include <vector>

namespace onnc {

/** \class TargetRegistry
 *  \brief TargetRegistry bookkeeps Target objects.
 */
class TargetRegistry
{
public:
   typedef std::vector<Target*> TargetList;
   typedef TargetList::iterator iterator;

public:
  static iterator Begin();

  static iterator End();

  static size_t Size();

  static bool IsEmpty();

  /// Register a given target.
  static void RegisterTarget(Target& pTarget,
                             const char* pName,
                             const char* pShortDesc,
                             Target::QuadrupleMatchFnTy pArchMatchFn);

  /// Register a target backend.
  static void RegisterTargetBackend(Target& pTarget,
                                    Target::TargetBackendCtorFnTy pFn) {
    pTarget.m_TargetBackendCtorFn = pFn;
  }
};

} // namespace of onnc

#endif
