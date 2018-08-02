//===- Relu.cpp -----------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Relu.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_relu"

using namespace onnc;

char BM188X::Relu::ID = 0;

//===----------------------------------------------------------------------===//
// BM188X::Relu
//===----------------------------------------------------------------------===//
BM188X::Relu::Relu()
    : onnc::Relu(), m_NegativeSlope(0.0)
{
  setID(ID);
}

void BM188X::Relu::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Relu::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Relu::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}


bool BM188X::Relu::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
