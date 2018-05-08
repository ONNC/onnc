//===- PassSupport.h ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_PASS_SUPPORT_H
#define ONNC_CORE_PASS_SUPPORT_H
#include <onnc/Core/PassRegistry.h>

namespace onnc {

class TargetBackend;

#define INITIALIZE_PASS(pass_name, name)                                      \
  static void *Initialize##pass_name##Pass(PassRegistry &pRegistry) {         \
    PassInfo *info = new PassInfo(                                            \
        name,                                                                 \
        &pass_name::ID,                                                       \
        PassInfo::PassCtorFn(callDefaultCtor<pass_name>));                    \
    pRegistry.registerPass(*info);                                            \
    return info;                                                              \
  }

#define INITIALIZE_TB_PASS_BASE(pass_name, backend_type, name)                \
  static void *Initialize##pass_name##Pass(PassRegistry &pRegistry) {         \
    PassInfo *info = new PassInfo(                                            \
      name,                                                                   \
      &pass_name::ID,                                                         \
      PassInfo::PassCtorFn(callDefaultCtor<pass_name>),                       \
      PassInfo::PassTargetCtorFn(                                             \
          callTargetBackendCtor<pass_name, ##backend_type##>));               \
    pRegistry.registerPass(*info);                                            \
    return info;                                                              \
  }

#define INITIALIZE_TB_PASS(pass_name, name) \
   INITIALIZE_TB_PASS_BASE(pass_name, TargetBackend, name)

#define INITIALIZE_DLA_PASS(pass_name, name) \
   INITIALIZE_TB_PASS_BASE(pass_name, DLATargetBackend, name)

#define INITIALIZE_NPU_PASS(pass_name, name) \
   INITIALIZE_TB_PASS_BASE(pass_name, NPUTargetBackend, name)

/// Default constructor wrapper. A wrapper to convert a constructor into normal
/// function.
template <typename PassName> Pass *callDefaultCtor() { return new PassName(); }

template <typename PassName, typename BackendType>
Pass *callTargetBackendCtor(BackendType* pTB) {
  return new PassName(pTB);
}

} // namespace of onnc

#endif
