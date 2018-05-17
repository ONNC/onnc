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

/** \class onnc::Pass
 *  \brief encapsulate transformation algorithms.
 */
class ModulePass : public Pass
{
public:
  explicit ModulePass(char& pPassID)
    : Pass(kPT_Module, pPassID) { }

  ~ModulePass() override;

  /// Virtual method overriden by subclasses to process the module
  /// being operated on.
  virtual Pass::ReturnType runOnModule(Module &pModule) = 0;

  static bool classof(const Pass* pPass) {
      return Pass::kPT_Module == pPass->getPassKind();
  }
};

} // namespace of onnc

#endif
