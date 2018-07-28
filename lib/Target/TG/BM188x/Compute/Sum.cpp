//===- Sum.cpp ------------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Sum.h"
#include "../BM188xVisitor.h"
#include <onnc/Support/Casting.h>

#define DEBUG_TYPE "tg_sum"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::Sum::ID = 0;

//===----------------------------------------------------------------------===//
// BM188X::Sum
//===----------------------------------------------------------------------===//
BM188X::Sum::Sum()
    : onnc::Sum(), m_RShiftWidth(0),  m_DoRelu(0), m_ThresholdXQuantized()
{
  setID(ID);
}

void BM188X::Sum::print(std::ostream& pOS) const
{
  // TODO
}

void BM188X::Sum::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Sum::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Sum::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
