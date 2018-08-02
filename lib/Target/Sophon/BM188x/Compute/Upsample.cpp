//===- Upsample.cpp ------------------------------------------------------===//
//
//                             The ONNC Project
//
// See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "Upsample.h"
#include "../BM188xVisitor.h"

#define DEBUG_TYPE "tg_upsample"

using namespace onnc;
using namespace onnc::BM188X;

char BM188X::Upsample::ID = 0;

//===----------------------------------------------------------------------===//
// Upsample
//===----------------------------------------------------------------------===//
BM188X::Upsample::Upsample(int pScale, const FloatAttr& pHeightScale,
                           const FloatAttr& pWidthScale)
    : onnc::Upsample(pHeightScale, pWidthScale), m_Scale(pScale)
{
  setID(ID);
}

BM188X::Upsample::~Upsample()
{
}

void BM188X::Upsample::print(std::ostream &pOS) const
{
  // TODO
}

void BM188X::Upsample::accept(ComputeVisitor& pV)
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

void BM188X::Upsample::accept(ComputeVisitor& pV) const
{
  BM188xVisitor* visitor = dyn_cast<BM188xVisitor>(&pV);
  if (nullptr != visitor)
    visitor->visit(*this);
}

bool BM188X::Upsample::classof(const ComputeOperator* pOp)
{
  if (nullptr == pOp)
    return false;
  return (pOp->getID() == &ID);
}
