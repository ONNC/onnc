//===- ModulePass.h -------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#ifndef ONNC_CORE_MODULE_PASS_H
#define ONNC_CORE_MODULE_PASS_H
#include <onnc/Core/Pass.h>

namespace onnc {

/** \class onnc::ModulePass
 *  \brief encapsulate transformation algorithms.
 */
class ModulePass : virtual public Pass
{
public:
  ModulePass() = default;

  virtual ~ModulePass() = default;

  Kind getPassKind() const override {
    return Pass::kPT_Module;
  }

  /// Virtual method overridden by subclasses to process the module
  /// being operated on.
  virtual Pass::ReturnType runOnModule(Module &pModule) = 0;

  static bool classof(const Pass* pPass) {
      return Pass::kPT_Module == pPass->getPassKind();
  }
};

} // namespace of onnc

#endif
