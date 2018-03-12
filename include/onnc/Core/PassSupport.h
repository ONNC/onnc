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

#define INITIALIZE_PASS(pass_name, name)                                      \
  static void *Initialize##pass_name##Pass(PassRegistry &pRegistry) {         \
    PassInfo *info = new PassInfo(                                            \
        name,                                                                 \
        &pass_name::ID,                                                       \
        PassInfo::PassCtorFn(callDefaultCtor<pass_name>));                    \
    pRegistry.registerPass(*info);                                            \
    return info;                                                              \
  }

/// Default constructor wrapper. A wrapper to convert a constructor into normal
/// function.
template <typename PassName> Pass *callDefaultCtor() { return new PassName(); }

} // namespace of onnc

#endif
