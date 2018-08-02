//===- BM188xVisitor.cpp --------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "BM188xVisitor.h"

using namespace onnc;
using namespace onnc::BM188X;

char BM188xVisitor::ID = 0;

//===----------------------------------------------------------------------===//
// BM188xVisitor
//===----------------------------------------------------------------------===//
BM188xVisitor::BM188xVisitor()
    : ComputeVisitor(ID)
{
}

bool BM188xVisitor::classof(const ComputeVisitor* pVisitor)
{
  if (nullptr != pVisitor && pVisitor->hasVisitorID())
    return (pVisitor->getVisitorID() == &ID);
  return false;
}
