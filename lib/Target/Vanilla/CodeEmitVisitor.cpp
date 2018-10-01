//===- CodeEmitVisitor.cpp ------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include <onnc/Support/IOStream.h>
#include "CodeEmitVisitor.h"

using namespace onnc;
using namespace onnc::vanilla;

void CodeEmitVisitor::visit(Conv& pConv)
{
  errs() << "visit Conv\n";
}

void CodeEmitVisitor::visit(const Conv& pConv)
{
  errs() << "visit const Conv\n";
}
