//===- Pass.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Core/Pass.h>
#include <onnc/Support/IOStream.h>
#include <onnc/Diagnostic/MsgHandling.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// Pass
//===----------------------------------------------------------------------===//
Pass::~Pass()
{
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
