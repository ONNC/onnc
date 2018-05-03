//===- Host.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/Host.h>
#include <onnc/Config/Config.h>
#include <onnc/IR/Quadruple.h>

using namespace onnc;

//===----------------------------------------------------------------------===//
// namespace sys
//===----------------------------------------------------------------------===//
std::string onnc::sys::GetHostQuadruple()
{
  Quadruple quadruple(ONNC_DEFAULT_TARGET_QUADRUPLE);
  std::string result;
  quadruple.canonical(result);
  return result;
}
