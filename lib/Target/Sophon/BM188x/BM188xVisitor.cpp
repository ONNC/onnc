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

//===----------------------------------------------------------------------===//
// BM188xVisitor
//===----------------------------------------------------------------------===//
bool BM188xVisitor::classof(const ComputeVisitor* pVisitor)
{
  if (nullptr != pVisitor)
    return (pVisitor->getVisitorID() == id());
  return false;
}
