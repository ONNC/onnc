//===- Pass.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/Pass.h>
#include <onnc/Core/AnalysisResolver.h>
#include <onnc/Core/ModulePass.h>
#include <onnc/Support/Casting.h>
#include <onnc/Diagnostic/MsgHandling.h>
#include <onnc/Support/IOStream.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Pass
//===----------------------------------------------------------------------===//
Pass::~Pass()
{
  delete m_pResolver;
}

StringRef Pass::getPassName() const
{
  error(core_pass_no_name);
  return "unamed pass";
}

void Pass::print(OStream &pOS, const Module *pModule) const
{
  error(core_pass_no_print) << getPassName();
}

void Pass::dump()
{
  print(errs(), nullptr);
}

bool Pass::run(Module& pModule)
{
  ModulePass* pass = dyn_cast<ModulePass>(this);
  if (nullptr != pass) {
    return pass->runOnModule(pModule);
  }
  return false;
}

//===----------------------------------------------------------------------===//
// ModulePass
//===----------------------------------------------------------------------===//
ModulePass::~ModulePass()
{
  // Force out-of-line virtual method.
}
